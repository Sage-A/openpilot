## Acceleration
- Control widget for acceleration and braking
- Creates two push buttons and adds them to widget layout
- Outputs signals for starting and stopping acceleration

## Status
- Status label widgets
- Contains *StatusWidget*, an template for new sets of status labels. See **Adding a New Widget** for usage details. 
- *SpeedStatus:* Displays current vehicle speed in MPH or KMH, indicates if car is at a standstill
- *BlinkerStatus:* Indicators for left and right blinkers
- *CarStatus:* Indicator for car door is open, driver’s seatbelt unbuckled, cruise control is enabled, and if ESP has been disabled
- *DriveStatus:* Indicator for the gas/brake pedal being pressed. Displays current value of the gas pedal and gear
- *SteerStatus:* Indicator for steering wheel actively being turned. Displays amount and direct of steering wheeling turn.

## Steering
- A steering control widget
- Creates a *SliderControl* widget, layout, and forwards the valueChanged signal from the slider onwards for use in steering command
