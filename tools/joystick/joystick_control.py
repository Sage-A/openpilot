#!/usr/bin/env python3
import os
import argparse
import threading
import numpy as np
from inputs import UnpluggedError, get_gamepad
from joystick_control import get_slider_value, get_throttle_value, is_cancel_pressed

from cereal import messaging
from openpilot.common.params import Params
from openpilot.common.realtime import Ratekeeper
from openpilot.system.hardware import HARDWARE
from openpilot.tools.lib.kbhit import KBHit

EXPO = 0.4


class Keyboard:
  def __init__(self):
    self.kb = KBHit()
    self.axis_increment = 0.05  # 5% of full actuation each key press
    self.axes_map = {'w': 'gb', 's': 'gb',
                     'a': 'steer', 'd': 'steer'}
    self.axes_values = {'gb': 0., 'steer': 0.}
    self.axes_order = ['gb', 'steer']
    self.cancel = False

  def update(self):
    key = self.kb.getch().lower()
    self.cancel = False
    if key == 'r':
      self.axes_values = {ax: 0. for ax in self.axes_values}
    elif key == 'c':
      self.cancel = True
    elif key in self.axes_map:
      axis = self.axes_map[key]
      incr = self.axis_increment if key in ['w', 'a'] else -self.axis_increment
      self.axes_values[axis] = float(np.clip(self.axes_values[axis] + incr, -1, 1))
    else:
      return False
    return True
    
class SteeringGUI:
  def __init__(self, steer_slider):
      # Refers to the acceleration and steering inputs
      self.accel_axis = 'GUI_ACCEL'
      self.steer_axis = 'GUI_STEER'

      # Acceleration and steering both range from -1.0 to 1.0
      self.min_axis_value = {self.accel_axis: -1.0, self.steer_axis: -1.0}
      self.max_axis_value = {self.accel_axis: 1.0, self.steer_axis: 1.0}

      # Initially, acceleration and steering are set to 0
      self.axes_values = {self.accel_axis: 0., self.steer_axis: 0.}
  
      # Defines the order in which to read/process the axes
      self.axes_order = [self.accel_axis, self.steer_axis]

      # Tracks whether the "cancel" button (or an emergency stop control) has been pressed in the GUI
      self.cancel = False

      # Tracks the previous state of the cancel button
      self._cancel_prev = False

  def update(self):
    # Read input values directly from the GUI
    try:
      accel_raw = get_throttle_value()
      steer_raw = get_slider_value()
      cancel_now = is_cancel_pressed()

    except Exception:
      # If GUI is unavailable or throws error, set neutral state
      self.axes_values = {ax: 0. for ax in self.axes_values}
      return False

    # Update cancel logic joystick-style, detect when a button is pressed or released
    if not self._cancel_prev and cancel_now:
      self.cancel = True  # rising edge
    elif self._cancel_prev and not cancel_now:
      self.cancel = False  # falling edge
    
    # Was the cancel button not pressed last update, but is pressed now?
    self._cancel_prev = cancel_now  

    # Normalizing accel/steer input
    for axis, raw_value in [(self.accel_axis, accel_raw), (self.steer_axis, steer_raw)]:
      norm = -float(np.interp(raw_value, [self.min_axis_value[axis], self.max_axis_value[axis]], [-1., 1.]))
      norm = norm if abs(norm) > 0.03 else 0.  # deadzone
      self.axes_values[axis] = EXPO * norm ** 3 + (1 - EXPO) * norm

    return True

  /  


    

class Joystick:
  def __init__(self):
    # This class supports a PlayStation 5 DualSense controller on the comma 3X
    # TODO: find a way to get this from API or detect gamepad/PC, perhaps "inputs" doesn't support it
    self.cancel_button = 'BTN_NORTH'  # BTN_NORTH=X/triangle
    if HARDWARE.get_device_type() == 'pc':
      accel_axis = 'ABS_Z'
      steer_axis = 'ABS_RX'
      # TODO: once the longcontrol API is finalized, we can replace this with outputting gas/brake and steering
      self.flip_map = {'ABS_RZ': accel_axis}
    else:
      accel_axis = 'ABS_RX'
      steer_axis = 'ABS_Z'
      self.flip_map = {'ABS_RY': accel_axis}

    self.min_axis_value = {accel_axis: 0., steer_axis: 0.}
    self.max_axis_value = {accel_axis: 255., steer_axis: 255.}
    self.axes_values = {accel_axis: 0., steer_axis: 0.}
    self.axes_order = [accel_axis, steer_axis]
    self.cancel = False

  def update(self):
    try:
      joystick_event = get_gamepad()[0]
    except (OSError, UnpluggedError):
      self.axes_values = {ax: 0. for ax in self.axes_values}
      return False

    event = (joystick_event.code, joystick_event.state)

    # flip left trigger to negative accel
    if event[0] in self.flip_map:
      event = (self.flip_map[event[0]], -event[1])

    if event[0] == self.cancel_button:
      if event[1] == 1:
        self.cancel = True
      elif event[1] == 0:   # state 0 is falling edge
        self.cancel = False
    elif event[0] in self.axes_values:
      self.max_axis_value[event[0]] = max(event[1], self.max_axis_value[event[0]])
      self.min_axis_value[event[0]] = min(event[1], self.min_axis_value[event[0]])

      norm = -float(np.interp(event[1], [self.min_axis_value[event[0]], self.max_axis_value[event[0]]], [-1., 1.]))
      norm = norm if abs(norm) > 0.03 else 0.  # center can be noisy, deadzone of 3%
      self.axes_values[event[0]] = EXPO * norm ** 3 + (1 - EXPO) * norm  # less action near center for fine control
    else:
      return False
    return True


def send_thread(joystick):
  pm = messaging.PubMaster(['testJoystick'])

  rk = Ratekeeper(100, print_delay_threshold=None)

  while True:
    if rk.frame % 20 == 0:
      print('\n' + ', '.join(f'{name}: {round(v, 3)}' for name, v in joystick.axes_values.items()))

    joystick_msg = messaging.new_message('testJoystick')
    joystick_msg.valid = True
    joystick_msg.testJoystick.axes = [joystick.axes_values[ax] for ax in joystick.axes_order]

    pm.send('testJoystick', joystick_msg)

    rk.keep_time()


def joystick_control_thread(joystick):
  Params().put_bool('JoystickDebugMode', True)
  threading.Thread(target=send_thread, args=(joystick,), daemon=True).start()
  while True:
    joystick.update()


def main():
  joystick_control_thread(Joystick())


if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Publishes events from your GUI, joystick to control your car.\n' +
                                               'openpilot must be offroad before starting joystick_control. This tool supports ' +
                                               'a PlayStation 5 DualSense controller on the comma 3X.',
                                   formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument('--keyboard', action='store_true', help='Use your keyboard instead of a keyboard joystick')
  parser.add_argument('--gui', action='store_true', help='Use your GUI instead of a joystick')
  args = parser.parse_args()

  if not Params().get_bool("IsOffroad") and "ZMQ" not in os.environ:
    print("The car must be off before running joystick_control.")
    exit()

  print()
  if args.gui:
    print('Using GUI for control (slider + throttle inputs).')
  elif args.keyboard:
    print('Gas/brake control: `W` and `S` keys')
    print('Steering control: `A` and `D` keys')
    print('Buttons')
    print('- `R`: Resets axes')
    print('- `C`: Cancel cruise control')
  else:
    print('Using joystick, make sure to run cereal/messaging/bridge on your device if running over the network!')
    print('If not running on a comma device, the mapping may need to be adjusted.')

  # Controller selection logic
  if args.gui:
    # Replace 'your_module' with the module that has these functions
    from your_module import get_slider_value, get_throttle_value, is_cancel_pressed
    control = SteeringGUI(get_slider_value, get_throttle_value, is_cancel_pressed)
    steering_control_thread(control)
  else:
    joystick = Keyboard() if args.keyboard else Joystick()
    joystick_control_thread(joystick)
