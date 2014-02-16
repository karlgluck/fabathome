#pragma once

#include <QObject>

/**
 * This class is referenced in a tool-script to provide UI feedback about the
 * progress of the print.  It is accessible via the "progress" object in the
 * script itself.
 */
class ProgressTSI : public QObject {
  Q_OBJECT

public:
  ProgressTSI();
  virtual ~ProgressTSI();

public slots:

  /**
   * Invoked from a tool-script to indicate how many times step() will be called
   * before the print job has finished.  Calling this method resets the progress
   * bar to empty.
   */
  void setSteps(int count);

  /**
   * Increments the progress bar
   */
  void step();

  /**
   * Called after the last step to ensure that the progress bar is full
   */
  void finish();

  void log(QString message);

signals:
  // These signals are set up to be directly connected to the main printing
  // progress bar.
  void setRange(int min, int max);
  void setProgress(int value);

  // this signal lets the TSI send out messages
  void outputLogEntry(QString message);

private:
  int step_count_;
};

