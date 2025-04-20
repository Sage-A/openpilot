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
  main->addStretch();
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
}

accelWidget::accelWidget(QWidget* parent) : QWidget(parent) {
  QLabel *title = new QLabel("Acceleration");
  QPushButton *incBtn = new QPushButton();
  incBtn->setFixedSize(250, 400);
  incBtn->setIconSize(incBtn->Size());
  incBtn->setIcon(QPixmap("../assets/images/arrow.png"));
  
  QPushButton *decBtn = new QPushButton(tr("Down"));
  decBtn->setFixedSize(250, 400);

  QVBoxLayout *main = new QVBoxLayout(this);
  
  main->addWidget(title);
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
}

void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {
 
}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {
  QPushButton *close_btn = new QPushButton(tr("×"));
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      border-radius: 100px;
      background-color: #292929;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  close_btn->setFixedSize(200, 200);
  QObject::connect(close_btn, &QPushButton::clicked, this, &CustomWindow::closeCustom);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  QVBoxLayout *sidebar = new QVBoxLayout();
  QVBoxLayout *primary = new QVBoxLayout();
  QVBoxLayout *speed_bar = new QVBoxLayout();

  sidebar->addWidget(close_btn);
  
  steeringSlider *ss = new steeringSlider(this);
  primary->addWidget(ss);

  accelWidget *accW = new accelWidget(this);
  speed_bar->addWidget(accW);

  main_layout->addLayout(sidebar);
  main_layout->addLayout(primary);
  main_layout->addLayout(speed_bar);
  
  setStyleSheet(R"(
    CustomWindow {
      background-color: black;
    }
    QStackedWidget, ScrollView {
      background-color: #292929;
      border-radius: 30px;
    }
  )");
}
