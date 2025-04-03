#include "selfdrive/ui/qt/offroad/settings.h"

#include <cassert>
#include <cmath>
#include <string>

#include <QDebug>
#include <QLabel>

#include "common/params.h"
#include "common/util.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "system/hardware/hw.h"
#include "selfdrive/ui/qt/offroad/settings.h"


void CustomPanel::checkForUpdates() {
  std::system("pkill -SIGUSR1 -f system.updated.updated");
}

CustomPanel::CustomPanel(QWidget* parent) : ListWidget(parent) {
  
  //Custom button
  enableGui = new ButtonControl(tr("Drive-by-wire GUI"), tr("ENABLE"));
  connect(enableGui, &ButtonControl::clicked, this, &CustomPanel::openCustom);
  addItem(enableGui);

  updateLabels();
}

void CustomPanel::showEvent(QShowEvent *event) {
  // nice for testing on PC

  updateLabels();
}

void CustomPanel::updateLabels() {
  if (!isVisible()) {
    return;
  }
  update();
}
