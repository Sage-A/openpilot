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

class StatusWidget : public QWidget {
  Q_OBJECT
public:
  explicit StatusWidget(QWidget *parent);
  void update(const SubMaster &sm);

private:
  QLabel *fuel_value;
  QLabel *door_value;
  QLabel *seatbelt_value;
  QLabel *gas_value;
  QLabel *brake_value;
  QLabel *ss_value;
  QLabel *steer_enabled;
  QLabel *steer_value;
  QLabel *esp_value;
  QLabel *cruise_value;
  QLabel *gear_value
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
  void update(const SubMaster &sm);

private:
  QLabel *indicator;
  QLabel *unit;
};

class lightWidget : public QWidget {
  Q_OBJECT
public:
  explicit lightWidget(QWidget *parent);
  void update(const SubMaster &sm);

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
  void updateState(const UIState &s);

protected:
  void showEvent(QShowEvent *event) override;

signals:
  void closeCustom();

private:
  steeringSlider *ss;
  lightWidget *lW;
  accelWidget *accW;
  speedWidget *spW;
  StatusWidget *statusW;
};
