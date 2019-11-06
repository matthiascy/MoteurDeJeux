#ifndef SIMPLEOUTPUT_HPP
#define SIMPLEOUTPUT_HPP

#include <QLabel>

class SimpleOutput : public QLabel {

  Q_OBJECT

public:
  explicit SimpleOutput(QWidget *parent = nullptr);
  ~SimpleOutput() { }

  void SetStyle();

private:
  void initialize();

signals:

public slots:
};

#endif // SIMPLEOUTPUT_HPP
