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
#include "selfdrive/ui/qt/offroad/custom_widgets/acceleration.h"
#include "selfdrive/ui/qt/offroad/custom_widgets/steering.h"
#include "selfdrive/ui/qt/offroad/custom_widgets/status.h"

// ********** custom window **********
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
  void emitSliderVal();
  void accelDown();
  void accelRelease();

private:
// All widgets present in custom window
  SteeringSlider *ss;
  BlinkerStatus *lW;
  AccelerationW *accW;
  SpeedStatus *spW;
  SteerStatus *steerStat;
  DriveStatus *driveStat;
  CarStatus *carStat;
};
