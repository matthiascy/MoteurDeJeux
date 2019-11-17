#include "SimpleOutput.hpp"

SimpleOutput::SimpleOutput(QWidget *parent)
  : QLabel(parent)
{
  initialize();
}

void SimpleOutput::initialize()
{
  this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  this->setMinimumWidth(200);
  this->setText("output");
  this->SetStyle();
}

void SimpleOutput::SetStyle()
{
  this->setStyleSheet("background: #414245; color: #EEEEEE; padding-left: 20");
}
