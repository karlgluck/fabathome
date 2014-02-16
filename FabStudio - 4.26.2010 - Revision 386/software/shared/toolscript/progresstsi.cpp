#include "progresstsi.h"

ProgressTSI::ProgressTSI() {
  step_count_ = 0;
}

ProgressTSI::~ProgressTSI() {
}


void ProgressTSI::setSteps(int count) {
  step_count_ = 0;
  emit setRange(0, count);
  emit setProgress(0);
}

void ProgressTSI::step() {
  emit setProgress(++step_count_);
}

void ProgressTSI::finish() {
  emit setRange(0, 1);
  emit setProgress(1);
}

void ProgressTSI::log(QString message) {
  emit outputLogEntry(message);
}
