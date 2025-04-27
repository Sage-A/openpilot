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

void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {

  QObject::connect(uiState(), &UIState::uiUpdate, this, &CustomWindow::updateState);
  
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
  QObject::connect(close_btn, &QPushButton::clicked, this, &CustomWindow::closeCustom);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  QVBoxLayout *sidebar = new QVBoxLayout();
  QVBoxLayout *primary = new QVBoxLayout();
  QVBoxLayout *speed_bar = new QVBoxLayout();

  sidebar->addWidget(close_btn);
  
  ss = new SteeringSlider(this);
  lW = new BlinkerStatus(this);

  carStat = new CarStatus(this);
  steerStat = new SteerStatus(this);
  driveStat = new DriveStatus(this);
  
  primary->addWidget(lW);
  //primary->addWidget(carStat);
  //primary->addWidget(steerStat);
 // primary->addWidget(driveStat);
  primary->addWidget(ss);

  accW = new AccelerationW(this);
  spW = new SpeedStatus(this, 0);
  speed_bar->addWidget(spW);
  speed_bar->addWidget(accW);

  main_layout->addLayout(sidebar);
  main_layout->addLayout(primary);
  main_layout->addLayout(speed_bar);
  
  setStyleSheet(R"(
    CustomWindow {
      background-color: black;
    }
  )");
}

void CustomWindow::updateState(const UIState &s) {
  const SubMaster &sm = *(s.sm);
  lW->update(sm);
  spW->update(sm);
  carStat->update(sm);
  steerStat->update(sm);
  driveStat->update(sm);
}


