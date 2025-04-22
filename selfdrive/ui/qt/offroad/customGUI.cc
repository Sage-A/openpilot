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

steeringSlider::steeringSlider(QWidget* parent) : QWidget(parent) {
  QVBoxLayout *main = new QVBoxLayout(this);
  QLabel *title = new QLabel("Wheel Control");
  turnAngle = new QSpinBox();
  turnAngle->setPrefix("Angle:");
  title->setStyleSheet(R"(
    QLabel {
      font-size: 40px;
      font-weight: bold;
      color: #a9a9a9;
    }
  )");

  
  main->addWidget(title);
  main->addWidget(turnAngle);
  
  steering_slider = new SliderControl(Qt::Horizontal);
  connect(steering_slider, &SliderControl::valueChanged, turnAngle, &QSpinBox::setValue);
  steering_slider->setStyleSheet(R"(
          QSlider {
          min-height: 100px;
          max-height: 100px;
          background: #FFFFFF;
          color: black;
      }
      QSlider::groove:horizontal {
          border: 1px solid #262626;
          height: 25px;
          background: #FF0000;
          margin: 0 12px;
      }
      QSlider::handle:horizontal {
          background: #0000ff;
          width: 30px;
          height: 150px;
          margin: -24px -12px;
      }
          )");

  turnAngle->setStyleSheet(R"(
          QSpinBox {
          font-size: 25px;
      })");

  turnAngle->setReadOnly(true);
  steering_slider->setTickStyle(QSlider::TicksBothSides);
  steering_slider->setMinimum(-100);
  steering_slider->setMaximum(100);
  turnAngle->setRange(-100, 100);
  steering_slider->setValue(0);
  steering_slider->setTickInterval(10);
  main->addWidget(steering_slider);
  main->setAlignment(Qt::AlignCenter);
}

accelWidget::accelWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout *main = new QVBoxLayout(this);
  QLabel *title2 = new QLabel("Acceleration");
  title2->setStyleSheet(R"(
    QLabel {
     font-size: 40px;
     font-weight: bold;
     color: #a9a9a9;
     }
    )");
  
  QPushButton *incBtn = new QPushButton(tr("^"));
  incBtn->setFixedSize(200, 350);
  
  QPushButton *decBtn = new QPushButton(tr("V"));
  decBtn->setFixedSize(200, 350);
  
  main->addWidget(title2);
  main->addWidget(incBtn);
  main->addWidget(decBtn);

   setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      background-color: #DDDDDD;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");

  main->setAlignment(Qt::AlignCenter);
}

speedWidget::speedWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout *main = new QVBoxLayout(this);
  QLabel *title = new QLabel("Speed");
  title->setStyleSheet(R"(
    QLabel {
     font-size: 40px;
     font-weight: bold;
     color: #a9a9a9;
     }
    )");
  
  main->addWidget(title);

  indicator = new QLabel("0");
  unit = new QLabel(" mph");

  indicator->setStyleSheet(R"(
    QLabel {
     font-size: 30px;
     color: #FFFFFF;
     }
    )");
  unit->setStyleSheet(R"(
    QLabel {
     font-size: 30px;
     color: #FFFFFF;
     }
    )");
  QHBoxLayout *internal = new QHBoxLayout();
  internal->addWidget(indicator);
  internal->addWidget(unit);

  main->addLayout(internal);
  main->setAlignment(Qt::AlignCenter);
}

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
  
  ss = new steeringSlider(this);
  lW = new lightWidget(this);
  primary->addWidget(lW);
  primary->addWidget(ss);

  accW = new accelWidget(this);
  spW = new speedWidget(this);
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

void speedWidget::update(const SubMaster &sm){
  //Fetch current speed from car
  indicator->setText(QString::number(sm["carState"].getCarState().getVEgoCluster()));
}
