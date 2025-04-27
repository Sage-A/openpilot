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
  main->addStretch();
  main->setAlignment(Qt::AlignHCenter);
  QLabel *title = new QLabel("Wheel Control");
  turnAngle = new QSpinBox();
  turnAngle->setPrefix("Angle: ");
  title->setStyleSheet(R"(
    QLabel {
      font-size: 40px;
      font-weight: bold;
      color: #CCCCCC;
    }
  )");

  QHBoxLayout *temp = new QHBoxLayout();
  temp->setSpacing(35);
  temp->setAlignment(Qt::AlignCenter);
  temp->addWidget(title);
  temp->addWidget(turnAngle);

  main->addLayout(temp);
  main->setSpacing(20);
  steering_slider = new SliderControl(Qt::Horizontal);
  connect(steering_slider, &SliderControl::valueChanged, turnAngle, &QSpinBox::setValue);
  steering_slider->setStyleSheet(R"(
          QSlider {
          min-height: 200px;
          max-height: 200px;
          background: #000000;
          color: black;
      }
      QSlider::groove:horizontal {
          border: 1px solid #262626;
          height: 150px;
          background: #333333;
          margin: 0 75px;
      }
      QSlider::handle:horizontal {
          background: #CCCCCC;
          width: 100px;
          height: 200px;
          margin: -100px -50px;
      }
          )");

  turnAngle->setStyleSheet(R"(
          QSpinBox {
          background: #000000;
          color: #CCCCCC;
          font-size: 40px;
          font-weight: bold;
      })");

  turnAngle->setReadOnly(true);
  steering_slider->setMinimum(-100);
  steering_slider->setMaximum(100);
  turnAngle->setRange(-100, 100);
  steering_slider->setValue(0);
  main->addWidget(steering_slider);
}
