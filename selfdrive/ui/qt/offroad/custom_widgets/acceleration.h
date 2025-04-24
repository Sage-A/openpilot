#pragma once

#include <map>
#include <string>

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QPixmap>

#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"

class AccelerationW : public QWidget {
  Q_OBJECT
public:
  explicit AccelerationW(QWidget *parent);
};
