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
#include "cereal/gen/cpp/car.capnp.h"

// Bring custom window to the front
void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {
  // Connect custom GUI update to parent uiState update function
  QObject::connect(uiState(), &UIState::uiUpdate, this, &CustomWindow::updateState);

  // Close btn returns to settings page
  QPushButton *close_btn = new QPushButton(tr("×"));
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      border-radius: 75px;
      background-color: #555555;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  close_btn->setFixedSize(150, 150);

  // Status control buttons to hide/show indicator groups
  QPushButton *blink_btn = new QPushButton(tr("Blinker"));
  blink_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 25px;
      padding-bottom: 20px;
      border-radius: 25px;
      background-color: #555555;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  blink_btn->setFixedSize(125, 125);
  
  QPushButton *car_btn = new QPushButton(tr("Car"));
  car_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 30px;
      padding-bottom: 20px;
      border-radius: 25px;
      background-color: #555555;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  car_btn->setFixedSize(125, 125);

  QPushButton *steer_btn = new QPushButton(tr("Steer"));
  steer_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 30px;
      padding-bottom: 20px;
      border-radius: 25px;
      background-color: #555555;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  steer_btn->setFixedSize(125, 125);

  QPushButton *drive_btn = new QPushButton(tr("Drive"));
  drive_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 30px;
      padding-bottom: 20px;
      border-radius: 25px;
      background-color: #555555;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  drive_btn->setFixedSize(125, 125);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(40);
  QVBoxLayout *sidebar = new QVBoxLayout();
  QVBoxLayout *primary = new QVBoxLayout();
  QVBoxLayout *speed_bar = new QVBoxLayout();

  sidebar->addWidget(close_btn);
  sidebar->addWidget(blink_btn);
  sidebar->addWidget(car_btn);
  sidebar->addWidget(steer_btn);
  sidebar->addWidget(drive_btn);
  
  ss = new SteeringSlider(this);
  lW = new BlinkerStatus(this);

  carStat = new CarStatus(this);
  steerStat = new SteerStatus(this);
  driveStat = new DriveStatus(this);
  
  primary->addWidget(lW);
  primary->addWidget(carStat);
  primary->addWidget(steerStat);
  primary->addWidget(driveStat);
  primary->addWidget(ss);

  accW = new AccelerationW(this);
  spW = new SpeedStatus(this, 0);
  speed_bar->addWidget(spW);
  speed_bar->addWidget(accW);

  // Connect buttons to visibility functions + settings close
  QObject::connect(blink_btn, &QPushButton::clicked, [&](){
    lW->setVisible(!lW->isVisible());
  });
  QObject::connect(steer_btn, &QPushButton::clicked, [&](){
    steerStat->setVisible(!steerStat->isVisible());
  });
  QObject::connect(car_btn, &QPushButton::clicked, [&](){
    carStat->setVisible(!carStat->isVisible());
  });
  QObject::connect(drive_btn, &QPushButton::clicked, [&](){
    driveStat->setVisible(!driveStat->isVisible());
  });
  QObject::connect(close_btn, &QPushButton::clicked, this, &CustomWindow::closeCustom);
  try {
    connect(accW, &AccelerationW::accelStart, [=]() { 
      Py_Initalize();
      Py_Finalize();
    });
    connect(accW, &AccelerationW::accelStop, [=]() { emit accelRelease(); });
    connect(ss, &SteeringSlider::valueChange, [=](int val) { emit emitSliderVal(val); });
  }
  catch (...) {}
  main_layout->addLayout(sidebar);
  main_layout->addLayout(primary);
  main_layout->addLayout(speed_bar);
  setStyleSheet(R"(
    CustomWindow {
      background-color: black;
    }
  )");
}

// Update contents of custom window
// Inherits from ui
void CustomWindow::updateState(const UIState &s) {
  const SubMaster &sm = *(s.sm);
  lW->update(sm);
  spW->update(sm);
  carStat->update(sm);
  steerStat->update(sm);
  driveStat->update(sm);
}


