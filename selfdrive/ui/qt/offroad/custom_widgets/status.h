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

class StatusWidget : public QWidget {
   Q_OBJECT
 public:
   explicit StatusWidget(QWidget *parent);
   void update(const SubMaster &sm);
 
 private:
   QVBoxLayout *main;
 };

class SpeedStatus : public QWidget {
   Q_OBJECT
 public:
   explicit SpeedStatus(QWidget *parent, int uSel);
   void update(const SubMaster &sm);
 
 private: // Current speed of car, standstill indicator
   QVBoxLayout *main;
   QLabel *speed_value;
   QLabel *unit;
   QLabel *ss_value;
   float convFactor;
};

class BlinkerStatus : public QWidget {
   Q_OBJECT
 public:
   explicit BlinkerStatus(QWidget *parent);
   void update(const SubMaster &sm);
 
 private: // left/right blinkers on and off
   QVBoxLayout *main;
   QPixmap iconMap[2];
   QLabel *leftInd;
   QLabel *rightInd;
};

class CarStatus : public QWidget {
   Q_OBJECT
 public:
   explicit CarStatus(QWidget *parent);
   void update(const SubMaster &sm);
 
 private: // doors open, seatbelt unlatched, esp enabled
   QVBoxLayout *main;
   QLabel *door_value;
   QLabel *seatbelt_value;
   QLabel *esp_value;
};

class DriveStatus : public QWidget {
   Q_OBJECT
 public:
   explicit DriveStatus(QWidget *parent);
   void update(const SubMaster &sm);
 
 private: // Gas pedal, gas pressed, brake pressed, current gear, cruise control enabled
   QVBoxLayout *main;
   QLabel *gas_engaged;
   QLabel *gas_value;
   QLabel *brake_engaged;
   QLabel *cruise_enabled;
   QLabel *gear_value;
};

class SteerStatus : public QWidget {
   Q_OBJECT
 public:
   explicit SteerStatus(QWidget *parent);
   void update(const SubMaster &sm);
 
 private: // Steering value, steering engaged
   QVBoxLayout *main;
   QLabel *steer_enabled;
   QLabel *steer_value;
};
