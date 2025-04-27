
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
  main = new QVBoxLayout(this);
  
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void StatusWidget::update(const SubMaster &sm) {
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
  main = new QVBoxLayout(this);
  QHBoxLayout *main2 = new QHBoxLayout();
  QLabel *title = new QLabel("Turn Signals");
  
  QPixmap indL_on = QPixmap("../assets/icons/blinkL_on");
  QPixmap indL_off = QPixmap("../assets/icons/blinkL_off");
  QPixmap indR_on = QPixmap("../assets/icons/blinkR_on");
  QPixmap indR_off = QPixmap("../assets/icons/blinkR_off");

  iconMap[0] = indL_off;
  iconMap[1] = indL_on;
  iconMap[2] = indR_off;
  iconMap[3] = indR_on;
  leftInd = new QLabel();
  leftInd->setScaledContents(true);
  rightInd = new QLabel();
  rightInd->setScaledContents(true);
  
  leftInd->setPixmap(iconMap[0]);
  rightInd->setPixmap(iconMap[2]);
  
  leftInd->setStyleSheet(R"(
    QLabel {
      max-height: 150px;
      max-width: 150px;
      min-height: 150px;
      min-width: 150px;
    }
    )");
  rightInd->setStyleSheet(R"(
    QLabel {
      max-height: 150px;
      max-width: 150px;
      min-height: 150px;
      min-width: 150px;
    }
    )");

  main2->setSpacing(25);
  main2->setAlignment(Qt::AlignCenter);
  main2->addWidget(leftInd);
  main2->addWidget(title);
  main2->addWidget(rightInd);
  main->addLayout(main2);
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 60px;
    font-weight: bold;
    }
    )");
}

void BlinkerStatus::update(const SubMaster &sm){
  if(sm["carState"].getCarState().getLeftBlinker() == true){
    leftInd->setPixmap(iconMap[1]);
  }
  else{
    leftInd->setPixmap(iconMap[0]);
  }

  if(sm["carState"].getCarState().getRightBlinker() == true){
    rightInd->setPixmap(iconMap[3]);
  }
  else{
    rightInd->setPixmap(iconMap[2]);
  }
}

CarStatus::CarStatus(QWidget *parent) :  QWidget(parent) {
  main = new QVBoxLayout(this);
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

DriveStatus::DriveStatus(QWidget *parent) :  QWidget(parent) {
  main = new QVBoxLayout(this);
  QHBoxLayout *hLay = new QHBoxLayout();
  QLabel *fuel_name = new QLabel("Gas Pedal: ");
  QLabel *gas_pressed = new QLabel("Gas Engaged: ");
  QLabel *brake_pressed = new QLabel("Brake Engaged: ");
  gas_engaged = new QLabel("NULL");
  gas_value = new QLabel("NULL");
  brake_engaged = new QLabel("NULL");

  hLay->addWidget(fuel_name);
  hLay->addWidget(gas_value);
  hLay->addWidget(gas_pressed);
  hLay->addWidget(gas_engaged);
  hLay->addWidget(brake_pressed);
  hLay->addWidget(brake_engaged);

  QHBoxLayout *carStat = new QHBoxLayout();
  QLabel *cruiseStat = new QLabel("Cruise Control: ");
  QLabel *gearShift = new QLabel("Gear: ");

  cruise_enabled = new QLabel("NULL");
  gear_value = new QLabel("NULL");

  carStat->addWidget(cruiseStat);
  carStat->addWidget(cruise_enabled);
  carStat->addWidget(gearShift);
  carStat->addWidget(gear_value);

  main->addLayout(hLay);
  main->addLayout(carStat);
setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void DriveStatus::update(const SubMaster &sm){
  auto cs = sm["carState"].getCarState();
  cruise_enabled->setText(QString::number(cs.getCruiseState().getEnabled()));
  gear_value->setText(QString::number(static_cast<int>(cs.getGearShifter())));
  gas_value->setText(QString::number(cs.getGas()));
  gas_engaged->setText(QString::number(cs.getGasPressed()));
  brake_engaged->setText(QString::number(cs.getBrakePressed()));
}

SteerStatus::SteerStatus(QWidget *parent) :  QWidget(parent) {
  main = new QVBoxLayout(this);
  QHBoxLayout *steeringStatus = new QHBoxLayout();
  QLabel *steeringPressed = new QLabel("Steering Engaged: ");
  QLabel *steeringVal = new QLabel("Steering Val: ");
  steer_enabled = new QLabel("NULL");
  steer_value = new QLabel("NULL");

  steeringStatus->addWidget(steeringPressed);
  steeringStatus->addWidget(steer_enabled);
  steeringStatus->addWidget(steeringVal);
  steeringStatus->addWidget(steer_value);

  main->addLayout(steeringStatus);
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void SteerStatus::update(const SubMaster &sm){
  auto cs = sm["carState"].getCarState();
  steer_enabled->setText(QString::number(cs.getSteeringPressed()));
  steer_value->setText(QString::number(cs.getSteeringAngleDeg()));
}
