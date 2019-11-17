#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include <QWidget>

class Explorer : public QWidget
{
  Q_OBJECT
public:
  explicit Explorer(QWidget *parent = nullptr);
  ~Explorer() {}

  void SetStyle();

signals:

public slots:
};

#endif // EXPLORER_HPP
