#include <QComboBox>
#include <QLineEdit>
#include <QSplitter>
#include <QHBoxLayout>
#include "CommandLine.hpp"

CommandLine::CommandLine(QWidget* parent)
  : QWidget{parent}, editField{nullptr}, cmdTypeField{nullptr},
    cmdReceiverField{nullptr}, layout{nullptr}
{
  initialize();
}

CommandLine::~CommandLine()
{
  editField.reset(nullptr);
  cmdTypeField.reset(nullptr);
  cmdReceiverField.reset(nullptr);
  layout.reset(nullptr);
}

void CommandLine::initialize()
{
  editField = std::make_unique<QLineEdit>();
  cmdTypeField = std::make_unique<QComboBox>();
  layout = std::make_unique<QHBoxLayout>(this);
  cmdReceiverField = std::make_unique<QComboBox>();

  editField->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  cmdTypeField->addItems({"Command", "Python", "Guile"});
  cmdTypeField->setMinimumWidth(100);
  cmdReceiverField->addItems({"Editor"});
  cmdReceiverField->setMinimumWidth(70);

  layout->addWidget(cmdTypeField.get());
  layout->addSpacing(10);
  layout->addWidget(editField.get());
  layout->addSpacing(10);
  layout->addWidget(cmdReceiverField.get());
  this->setMaximumHeight(37);
}

void CommandLine::SetStyle()
{
  this->editField->setStyleSheet("background: #292A2B; color: #EEEEEE");
  this->cmdTypeField->setStyleSheet("background: #5A5C5F; color: #EEEEEE");
}
