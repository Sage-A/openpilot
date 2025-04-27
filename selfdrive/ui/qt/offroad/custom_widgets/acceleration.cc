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
#include "selfdrive/ui/qt/offroad/custom_widgets/acceleration.h"

AccelerationW::AccelerationW(QWidget* parent) : QWidget(parent) {
  // Create title and control buttons
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
  QObject::connect(&incBtn, &QPushButton::pressed, this, &AccelerationW::accelStart);
  QObject::connect(&incBtn, &QPushButton::released, this, &AccelerationW::accelStop);
  
  QPushButton *decBtn = new QPushButton(tr("V"));
  decBtn->setFixedSize(200, 350);

  // Add widgets to layout
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

  main->setAlignment(Qt::AlignRight);
}
