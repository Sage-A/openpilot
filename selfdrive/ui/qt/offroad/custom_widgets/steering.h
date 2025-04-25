#pragma once

#include <map>
#include <string>

#include <QButtonGroup>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QSpinBox>
#include <QPixmap>

#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"

class steeringSlider : public QWidget {
  Q_OBJECT
public:
  explicit steeringSlider(QWidget *parent);

private:
  SliderControl *steering_slider;
  QSpinBox *turnAngle;
};
