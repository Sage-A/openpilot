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

lightWidget::lightWidget(QWidget* parent) : QWidget(parent) {
  QHBoxLayout *main = new QHBoxLayout(this);
  QLabel *leftBlinker = new QLabel("Left Blinker");
  QLabel *rightBlinker = new QLabel("Right Blinker");
  
  QPixmap ind_on = QPixmap("../assets/icons/indicator_on");
  QPixmap ind_off = QPixmap("../assets/icons/indicator_off");

  iconMap[0] = ind_off;
  iconMap[1] = ind_on;
  leftInd = new QLabel();
  leftInd->setScaledContents(true);
  rightInd = new QLabel();
  rightInd->setScaledContents(true);
  
  leftInd->setPixmap(iconMap[0]);
  rightInd->setPixmap(iconMap[1]);

  leftInd->setStyleSheet(R"(
    QLabel {
      max-height: 50px;
      max-width: 50px;
      min-height: 50px;
      min-width: 50px;
    }
    )");
  rightInd->setStyleSheet(R"(
    QLabel {
      max-height: 50px;
      max-width: 50px;
      min-height: 50px;
      min-width: 50px;
    }
    )");
  main->addWidget(leftBlinker);
  main->addWidget(leftInd);
  main->addWidget(rightBlinker);
  main->addWidget(rightInd);
  setStyleSheet(R"(
    QLabel {
    color: #BBBBBB;
    font-size: 30px;
    }
    )");
}

void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {
   
}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {

  QObject::connect(uiState(), &UIState::uiUpdate, this, &CustomWindow::updateState);
  
  QPushButton *close_btn = new QPushButton(tr("×"));
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      border-radius: 75px;
      background-color: #292929;
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
  lW = new lightWidget(this);
  statusW = new StatusWidget(this);
  primary->addWidget(lW);
  primary->addWidget(statusW);
  primary->addWidget(ss);

  accW = new AccelerationW(this);
  spW = new SpeedStatus(this, 1);
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
  statusW->update(sm);
}

void lightWidget::update(const SubMaster &sm){
  if(sm["carState"].getCarState().getLeftBlinker() == true){
    leftInd->setPixmap(iconMap[1]);
  }
  else{
    leftInd->setPixmap(iconMap[0]);
  }

  if(sm["carState"].getCarState().getRightBlinker() == true){
    rightInd->setPixmap(iconMap[1]);
  }
  else{
    rightInd->setPixmap(iconMap[0]);
  }
}
