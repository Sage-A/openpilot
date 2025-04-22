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

// ********** custom window + top-level panels **********
class steeringSlider : public QWidget {
  Q_OBJECT
public:
  explicit steeringSlider(QWidget *parent);

private:
  SliderControl *steering_slider;
  QSpinBox *turnAngle;
};

class accelWidget : public QWidget {
  Q_OBJECT
public:
  explicit accelWidget(QWidget *parent);
};

class speedWidget : public QWidget {
  Q_OBJECT
public:
  explicit speedWidget(QWidget *parent);
  void update();

private:
  QLabel *indicator;
  QLabel *unit;
};

class lightWidget : public QWidget {
  Q_OBJECT
public:
  explicit lightWidget(QWidget *parent);
  void update(bool left, bool right);

private:
  QPixmap iconMap[2];
  QLabel *leftInd;
  QLabel *rightInd;
};

class CustomWindow : public QFrame {
  Q_OBJECT

public:
  explicit CustomWindow(QWidget *parent = 0);
  void setCurrentPanel(int index, const QString &param = "");
  void update();

protected:
  void showEvent(QShowEvent *event) override;

signals:
  void closeCustom();

private:
  steeringSlider *ss;
  lightWidget *lW;
  accelWidget *accW;
  speedWidget *spW;
};
