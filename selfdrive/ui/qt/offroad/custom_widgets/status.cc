
#include <cassert>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>

#include <QDebug>

#include "common/watchdog.h"
#include "common/util.h"
#include "selfdrive/ui/qt/network/networking.h"
#include "selfdrive/ui/qt/offroad/settings.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/prime.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/offroad/developer_panel.h"
#include "selfdrive/ui/qt/offroad/customGUI.h"
#include "selfdrive/ui/qt/offroad/custom_widgets/status.h"

StatusWidget::StatusWidget(QWidget* parent) : QWidget(parent) {
  //Tempory for testing in car on Tuesday
  QVBoxLayout *main = new QVBoxLayout(this);
  QHBoxLayout *fuel = new QHBoxLayout();
  QLabel *fuel_name = new QLabel("Gas Pedal: ");
  QLabel *gas_pressed = new QLabel("Gas Engaged: ");
  QLabel *brake_pressed = new QLabel("Brake Engaged: ");
  fuel_value = new QLabel("NULL");
  gas_value = new QLabel("NULL");
  brake_value = new QLabel("NULL");

  fuel->addWidget(fuel_name);
  fuel->addWidget(fuel_value);
  fuel->addWidget(gas_pressed);
  fuel->addWidget(gas_value);
  fuel->addWidget(brake_pressed);
  fuel->addWidget(brake_value);

  QHBoxLayout *driverStatus = new QHBoxLayout();
  QLabel *door_status = new QLabel("Door Status: ");
  QLabel *seatbelt_stat = new QLabel("Seatbelt: ");
  door_value = new QLabel("NULL");
  seatbelt_value = new QLabel("NULL");

  driverStatus->addWidget(door_status);
  driverStatus->addWidget(door_value);
  driverStatus->addWidget(seatbelt_stat);
  driverStatus->addWidget(seatbelt_value);

  QHBoxLayout *steeringStatus = new QHBoxLayout();
  QLabel *standstill = new QLabel("Standstill: ");
  QLabel *steeringPressed = new QLabel("Steering Engaged: ");
  QLabel *steeringVal = new QLabel("Steering Val: ");
  ss_value = new QLabel("NULL");
  steer_enabled = new QLabel("NULL");
  steer_value = new QLabel("NULL");

  steeringStatus->addWidget(standstill);
  steeringStatus->addWidget(ss_value);
  steeringStatus->addWidget(steeringPressed);
  steeringStatus->addWidget(steer_enabled);
  steeringStatus->addWidget(steeringVal);
  steeringStatus->addWidget(steer_value);

  QHBoxLayout *carStat = new QHBoxLayout();
  QLabel *espEnabled = new QLabel("ESP Enabled: ");
  QLabel *cruiseStat = new QLabel("Cruise Control: ");
  QLabel *gearShift = new QLabel("Gear: ");
  esp_value = new QLabel("NULL");
  cruise_value = new QLabel("NULL");
  gear_value = new QLabel("NULL");

  carStat->addWidget(espEnabled);
  carStat->addWidget(esp_value);
  carStat->addWidget(cruiseStat);
  carStat->addWidget(cruise_value);
  carStat->addWidget(gearShift);
  carStat->addWidget(gear_value);

  
  main->addLayout(fuel);
  main->addLayout(driverStatus);
  main->addLayout(steeringStatus);
  main->addLayout(carStat);
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void StatusWidget::update(const SubMaster &sm) {
  auto cs = sm["carState"].getCarState();
  gear_value->setText(QString::number(static_cast<int>(cs.getGearShifter())));
  fuel_value->setText(QString::number(cs.getGas()));
  gas_value->setText(QString::number(cs.getGasPressed()));
  door_value->setText(QString::number(cs.getDoorOpen()));
  seatbelt_value->setText(QString::number(cs.getSeatbeltUnlatched()));
  brake_value->setText(QString::number(cs.getBrakePressed()));
  ss_value->setText(QString::number(cs.getStandstill()));
  steer_enabled->setText(QString::number(cs.getSteeringPressed()));
  steer_value->setText(QString::number(cs.getSteeringAngleDeg()));
  esp_value->setText(QString::number(cs.getEspDisabled()));
  cruise_value->setText(QString::number(cs.getCruiseState().getEnabled()));
}
