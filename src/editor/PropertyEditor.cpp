#include "PropertyEditor.hpp"

PropertyEditor::PropertyEditor(QWidget *parent) : QWidget(parent)
{

}

void PropertyEditor::SetStyle()
{
  this->setMinimumHeight(300);
  this->setMinimumWidth(200);
  this->setStyleSheet("background: #414345; color: #EEEEEE;");
}
