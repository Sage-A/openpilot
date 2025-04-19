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

speedSlider::speedSlider(QWidget* parent) : QWidget(parent) {
  QHBoxLayout *main = new QHBoxLayout(this);
  QLabel *title = new QLabel("Speed Control");

  main->addWidget(title);
  SliderControl *speed_slider = new SliderControl(Qt::Horizontal);
  speed_slider->setStyleSheet(R"(
          QSlider {
          min-height: 100px;
          max-height: 100px;
          background: #5F4141;
      }
      QSlider::groove:horizontal {
          border: 1px solid #262626;
          height: 25px;
          background: #FF0000;
          margin: 0 12px;
      }
      QSlider::handle:horizontal {
          background: #22B14C;
          border: 5px solid #B5E61D;
          width: 30px;
          height: 150px;
          margin: -24px -12px;
      }
          )");
  main->addWidget(speed_slider);
}

void CustomWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void CustomWindow::setCurrentPanel(int index, const QString &param) {
 
}

CustomWindow::CustomWindow(QWidget *parent) : QFrame(parent) {
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
          height: 25px;
          background: #FF0000;
          margin: 0 12px;
      }
      QSlider::handle:horizontal {
          background: #22B14C;
          border: 5px solid #B5E61D;
          width: 30px;
          height: 150px;
          margin: -24px -12px;
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

  speedSlider ss = new speedSlider(this);
  primary->addWidget(ss);

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
