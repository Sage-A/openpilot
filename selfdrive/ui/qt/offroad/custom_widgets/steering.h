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

class SteeringSlider : public QWidget {
  Q_OBJECT
public:
  explicit SteeringSlider(QWidget *parent);

private:
  SliderControl *steering_slider;
  QSpinBox *turnAngle;
};
