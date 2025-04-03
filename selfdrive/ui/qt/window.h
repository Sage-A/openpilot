#pragma once

#include <QStackedLayout>
#include <QWidget>

#include "selfdrive/ui/qt/home.h"
#include "selfdrive/ui/qt/offroad/onboarding.h"
#include "selfdrive/ui/qt/offroad/settings.h"
#include "selfdrive/ui/qt/offroad/customGUI.h"

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);

private:
  bool eventFilter(QObject *obj, QEvent *event) override;
  void openSettings(int index = 0, const QString &param = "");
  void closeSettings();
  void openCustom(int index = 0, const QString &param = "");
  void closeCustom();

  QStackedLayout *main_layout;
  HomeWindow *homeWindow;
  SettingsWindow *settingsWindow;
  OnboardingWindow *onboardingWindow;
  CustomWindow *customWindow;
};
