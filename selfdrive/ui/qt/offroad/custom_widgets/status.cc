
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
    color: #CCCCCC;
    font-size: 30px;
    }
    )");
}

void StatusWidget::update(const SubMaster &sm) {
}

SpeedStatus::SpeedStatus(QWidget* parent, int uSel) : QWidget(parent) {
  convFactor = 1;
  QLabel *units;
  QPixmap indOff = QPixmap("../assets/icons/indicatorC_off");
  QPixmap indOn = QPixmap("../assets/icons/indicatorC_alert");
  iconMap[0] = indOff;
  iconMap[1] = indOn;
  
  if(uSel == 0){
    convFactor = 2.2369;
    units = new QLabel("MPH");
  }
  else if(uSel == 1){
    convFactor = 3.6;
    units = new QLabel("KMH");
  }
      
  main = new QVBoxLayout(this);
  
  speed_value = new QLabel("0");
  QLabel *standstill = new QLabel("Standstill");
  ss_value = new QLabel();
  ss_value->setScaledContents(true);
  ss_value->setPixmap(iconMap[0]);
  ss_value->setStyleSheet(R"(
    QLabel {
      max-height: 35px;
      max-width: 35px;
      min-height: 35px;
      min-width: 35px;
    }
    )");
  standstill->setStyleSheet(R"(
    QLabel {
    font-size: 35px;
    }
  )");
  
  QHBoxLayout *i = new QHBoxLayout();
  QHBoxLayout *i2 = new QHBoxLayout();
  i2->setAlignment(Qt::AlignCenter);
  i2->setSpacing(35);
  
  i->addWidget(speed_value);
  i->addWidget(units);
  
  i2->addWidget(standstill);
  i2->addWidget(ss_value);
  
  main->addLayout(i);
  main->addLayout(i2);  
  setStyleSheet(R"(
    QLabel {
     font-size: 50px;
     font-weight: bold;
     color: #CCCCCC;
     }
    )");
}

void SpeedStatus::update(const SubMaster &sm){
    auto cs = sm["carState"].getCarState();
    speed_value->setText(QString::number((static_cast<int>(convFactor+cs.getVEgoCluster()))));
  
    if(cs.getStandstill() == 1){
      ss_value->setPixmap(iconMap[1]);
    }
    else{
      ss_value->setPixmap(iconMap[0]);
    }
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
    color: #CCCCCC;
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
  QPixmap indOff = QPixmap("../assets/icons/indicatorC_off");
  QPixmap indAlert = QPixmap("../assets/icons/indicatorC_alert");
  QPixmap indWarn = QPixmap("../assets/icons/indicatorC_warn");
  iconMap[0] = indOff;
  iconMap[1] = indAlert;
  iconMap[2] = indWarn;
  
  QHBoxLayout *driverStatus = new QHBoxLayout();
  driverStatus->setSpacing(30);
  driverStatus->setAlignment(Qt::AlignLeft);
  
  QLabel *door_status = new QLabel("Door Open");
  QLabel *seatbelt_stat = new QLabel("Seatbelt Unbuckled");
  QLabel *espEnabled = new QLabel("ESP Disabled");
  
  door_value = new QLabel();
  door_value->setScaledContents(true);
  door_value->setPixmap(iconMap[0]);
  door_value->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");
  seatbelt_value = new QLabel();
  seatbelt_value->setPixmap(iconMap[0]);
  seatbelt_value->setScaledContents(true);
  seatbelt_value->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");
  
  esp_value = new QLabel();
  esp_value->setPixmap(iconMap[0]);
  esp_value->setScaledContents(true);
  esp_value->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");

  driverStatus->addWidget(door_status);
  driverStatus->addWidget(door_value);
  driverStatus->addStretch();
  driverStatus->addWidget(seatbelt_stat);
  driverStatus->addWidget(seatbelt_value);
  main->addLayout(driverStatus);
  QHBoxLayout *temp = new QHBoxLayout();
  temp->setAlignment(Qt::AlignLeft);
  temp->setSpacing(30);
  temp->addWidget(espEnabled);
  temp->addWidget(esp_value);
 
  main->addLayout(temp);
  setStyleSheet(R"(
    QLabel {
      color: #CCCCCC;
      font-size: 55px;
      font-weight: bold;
    }
    )");
}

void CarStatus::update(const SubMaster &sm){
  auto cs = sm["carState"].getCarState();
  if(cs.getDoorOpen() == 1){
    // Door is open in car, turn on warning light
    door_value->setPixmap(iconMap[2]);
  }
  else{
    door_value->setPixmap(iconMap[0]);
  }

  if(cs.getSeatbeltUnlatched() == 1){
    // Seatbelt unbuckled
    seatbelt_value->setPixmap(iconMap[2]);
  }
  else{
    seatbelt_value->setPixmap(iconMap[0]);
  }

  if(cs.getEspDisabled() == 0){
    // ESP is off
    esp_value->setPixmap(iconMap[1]);
  }
  else{
    esp_value->setPixmap(iconMap[0]);
  }
}

DriveStatus::DriveStatus(QWidget *parent) :  QWidget(parent) {
  main = new QVBoxLayout(this);
  QPixmap indOff = QPixmap("../assets/icons/indicatorC_off");
  QPixmap indOn = QPixmap("../assets/icons/indicatorC_on");
  iconMap[0] = indOff;
  iconMap[1] = indOn;
  QHBoxLayout *hLay = new QHBoxLayout();
  hLay->setSpacing(35);
  hLay->setAlignment(Qt::AlignLeft);
  
  QLabel *fuel_name = new QLabel("Gas Pedal  %");
  QLabel *gas_pressed = new QLabel("Gas Engaged");
  QLabel *brake_pressed = new QLabel("Brake Engaged");
  gas_engaged = new QLabel();
  gas_engaged->setScaledContents(true);
  gas_engaged->setPixmap(iconMap[0]);
  gas_engaged->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");
  gas_value = new QLabel();
  brake_engaged = new QLabel();
  brake_engaged->setScaledContents(true);
  brake_engaged->setPixmap(iconMap[0]);
  brake_engaged->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");

  hLay->addWidget(gas_pressed);
  hLay->addWidget(gas_engaged);
  hLay->addStretch();
  hLay->addWidget(brake_pressed);
  hLay->addWidget(brake_engaged);

  QHBoxLayout *carStat = new QHBoxLayout();
  carStat->setSpacing(35);
  carStat->setAlignment(Qt::AlignLeft);
  QLabel *cruiseStat = new QLabel("Cruise Control");
  QLabel *gearShift = new QLabel("Gear: ");

  cruise_enabled = new QLabel();
  cruise_enabled->setScaledContents(true);
  cruise_enabled->setPixmap(iconMap[0]);
  cruise_enabled->setStyleSheet(R"(
    QLabel {
      max-height: 60px;
      max-width: 60px;
      min-height: 60px;
      min-width: 60px; } )");
  gear_value = new QLabel();

  carStat->addWidget(cruiseStat);
  carStat->addWidget(cruise_enabled);
  carStat->addStretch();
  carStat->addWidget(gearShift);
  carStat->addWidget(gear_value);
  carStat->addStretch();
  carStat->addWidget(fuel_name);
  carStat->addWidget(gas_value);

  main->addLayout(hLay);
  main->addLayout(carStat);
setStyleSheet(R"(
    QLabel {
    color: #CCCCCC;
    font-size: 55px;
    font-weight: bold;
    }
    )");
}

void DriveStatus::update(const SubMaster &sm){
  auto cs = sm["carState"].getCarState();
  if(cs.getCruiseState().getEnabled() == 1){
    cruise_enabled->setPixmap(iconMap[1]);
  }
  else{
    cruise_enabled->setPixmap(iconMap[0]);
  }

  if(cs.getGasPressed() == 1){
    gas_engaged->setPixmap(iconMap[1]);
  }
  else{
    gas_engaged->setPixmap(iconMap[0]);
  }

  if(cs.getBrakePressed() == 1){
    brake_engaged->setPixmap(iconMap[1]);
  }
  else{
    brake_engaged->setPixmap(iconMap[0]);
  }
  
  if(static_cast<int>(cs.getGearShifter()) == 1){
    gear_value->setText("Park");
  }
  else if(static_cast<int>(cs.getGearShifter()) == 2){
    gear_value->setText("Drive");
  }
  else if(static_cast<int>(cs.getGearShifter()) == 4){
    gear_value->setText("Reverse");
  }
  else{
    gear_value->setText("Unknown");
  }

  gas_value->setText(static_cast<int>(QString::number(cs.getGas()) / 250));
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
