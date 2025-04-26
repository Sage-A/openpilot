
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
  main = new QVBoxLayout(this);
  
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

  QHBoxLayout *steeringStatus = new QHBoxLayout();
  QLabel *steeringPressed = new QLabel("Steering Engaged: ");
  QLabel *steeringVal = new QLabel("Steering Val: ");
  steer_enabled = new QLabel("NULL");
  steer_value = new QLabel("NULL");

  steeringStatus->addWidget(steeringPressed);
  steeringStatus->addWidget(steer_enabled);
  steeringStatus->addWidget(steeringVal);
  steeringStatus->addWidget(steer_value);

  QHBoxLayout *carStat = new QHBoxLayout();
  QLabel *cruiseStat = new QLabel("Cruise Control: ");
  QLabel *gearShift = new QLabel("Gear: ");

  cruise_value = new QLabel("NULL");
  gear_value = new QLabel("NULL");

  carStat->addWidget(cruiseStat);
  carStat->addWidget(cruise_value);
  carStat->addWidget(gearShift);
  carStat->addWidget(gear_value);

  
  main->addLayout(fuel);
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
  brake_value->setText(QString::number(cs.getBrakePressed()));
  steer_enabled->setText(QString::number(cs.getSteeringPressed()));
  steer_value->setText(QString::number(cs.getSteeringAngleDeg()));

  cruise_value->setText(QString::number(cs.getCruiseState().getEnabled()));
}

SpeedStatus::SpeedStatus(QWidget* parent, int uSel) : QWidget(parent) {
  convFactor = 1;
  QLabel *units;
  if(uSel == 0){
    convFactor = 2.2369;
    units = new QLabel("MPH");
  }
  else if(uSel == 1){
    convFactor = 3.6;
    units = new QLabel("KMH");
  }
      
  main = new QVBoxLayout(this);
  QLabel *title = new QLabel("Speed");
  title->setStyleSheet(R"(
    QLabel {
     font-size: 40px;
     font-weight: bold;
     color: #a9a9a9;
     }
    )");
  
  main->addWidget(title);
  
  speed_value = new QLabel("0");
  QLabel *standstill = new QLabel("Standstill: ");
  ss_value = new QLabel("NULL");
  
  QHBoxLayout *i = new QHBoxLayout();
  QHBoxLayout *i2 = new QHBoxLayout();
  i->addWidget(speed_value);
  i->addWidget(units);
  i2->addWidget(standstill);
  i2->addWidget(ss_value);
  
  main->addLayout(i);
  main->addLayout(i2);
  main->setAlignment(Qt::AlignCenter);
  
  setStyleSheet(R"(
    QLabel {
     font-size: 30px;
     color: #FFFFFF;
     }
    )");
}

void SpeedStatus::update(const SubMaster &sm){
    auto cs = sm["carState"].getCarState();
    ss_value->setText(QString::number(cs.getStandstill()));
    speed_value->setText(QString::number((convFactor+cs.getVEgoCluster())));
}

BlinkerStatus::BlinkerStatus(QWidget *parent) :  QWidget(parent) {
  
}

CarStatus::CarStatus(QWidget *parent) :  QWidget(parent) {
  QHBoxLayout *driverStatus = new QHBoxLayout();
  QLabel *door_status = new QLabel("Door Status: ");
  QLabel *seatbelt_stat = new QLabel("Seatbelt: ");
  QLabel *espEnabled = new QLabel("ESP Enabled: ");
  door_value = new QLabel("NULL");
  seatbelt_value = new QLabel("NULL");
  esp_value = new QLabel("NULL");

  driverStatus->addWidget(door_status);
  driverStatus->addWidget(door_value);
  driverStatus->addWidget(seatbelt_stat);
  driverStatus->addWidget(seatbelt_value);
  driverStatus->addWidget(espEnabled);
  driverStatus->addWidget(esp_value);
  
  main->addLayout(driverStatus);
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void CarStatus::update(const SubMaster &sm){
  auto cs = sm["carState"].getCarState();
  door_value->setText(QString::number(cs.getDoorOpen()));
  seatbelt_value->setText(QString::number(cs.getSeatbeltUnlatched()));
  esp_value->setText(QString::number(cs.getEspDisabled()));
}

DriveStatus::DriveStatus(QWidget *parent) :  QWidget(parent) {}
SteerStatus::SteerStatus(QWidget *parent) :  QWidget(parent) {}

