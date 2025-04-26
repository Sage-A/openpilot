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
#include "selfdrive/ui/qt/offroad/custom_widgets/steering.h"

SteeringSlider::SteeringSlider(QWidget* parent) : QWidget(parent) {
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
