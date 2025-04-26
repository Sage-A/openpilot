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

// ********** custom window + top-level panels **********
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
  SteeringSlider *ss;
  lightWidget *lW;
  AccelerationW *accW;
  SpeedStatus *spW;
  StatusWidget *statusW;
};
