#include "Explorer.hpp"

Explorer::Explorer(QWidget *parent) : QWidget(parent)
{

}

void Explorer::SetStyle()
{
  this->setMinimumHeight(240);
  this->setStyleSheet("background: #414345; color: #EEEEEE;");
}
