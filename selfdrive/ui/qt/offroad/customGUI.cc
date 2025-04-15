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

void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {
 
}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {
  // setup two main layouts
  QPushButton *close_btn = new QPushButton(tr("×"));
  SliderControl *speed_slider = new SliderControl(Qt::Horizontal);
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
    speed_slider->setStyleSheet(R"(
              QSlider {
          min-height: 100px;
          max-height: 100px;
          background: #5F4141;
      }
      QSlider::groove:horizontal {
          border: 1px solid #262626;
          height: 5px;
          background: #393939;
          margin: 0 12px;
      }
      QSlider::handle:horizontal {
          background: #22B14C;
          border: 5px solid #B5E61D;
          width: 23px;
          height: 100px;
          margin: -24px -12px;
      }
          )");
  close_btn->setFixedSize(200, 200);
  QObject::connect(close_btn, &QPushButton::clicked, this, &CustomWindow::closeCustom);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  main_layout->addWidget(close_btn);
  main_layout->addWidget(speed_slider);

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
