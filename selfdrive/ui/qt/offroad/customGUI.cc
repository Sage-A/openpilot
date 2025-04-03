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
  main_layout->addWidget(close_btn);

  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    CustomWindow {
      background-color: black;
    }
    QStackedWidget, ScrollView {
      background-color: #292929;
      border-radius: 30px;
    }
  )");
}
