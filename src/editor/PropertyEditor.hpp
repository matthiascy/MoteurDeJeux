#ifndef PROPERTYEDITOR_HPP
#define PROPERTYEDITOR_HPP

#include <QWidget>

class PropertyEditor : public QWidget {

  Q_OBJECT

public:
  explicit PropertyEditor(QWidget *parent = nullptr);

  void SetStyle();

signals:

public slots:
};

#endif // PROPERTYEDITOR_HPP
