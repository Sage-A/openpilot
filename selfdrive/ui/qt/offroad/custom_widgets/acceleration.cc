#include <string>
#include <QDebug>

#include "selfdrive/ui/qt/offroad/custom_widgets/acceleration.h"

AccelerationW::AccelerationW(QWidget* parent) : QWidget(parent) {
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
