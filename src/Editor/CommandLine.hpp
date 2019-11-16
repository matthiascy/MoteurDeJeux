#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <QWidget>
#include <memory>

class QLabel;
class QSplitter;
class QComboBox;
class QLineEdit;
class QHBoxLayout;

enum class CommandType {
  Cmd, Python, Guile
};

enum class CommandReceiver {
  Editor
};

class CommandLine : public QWidget {

  Q_OBJECT

public:
  explicit CommandLine(QWidget* parent = nullptr);
  ~CommandLine();

  void SetStyle();

private:
  std::unique_ptr<QLineEdit> editField;
  std::unique_ptr<QComboBox> cmdTypeField;
  std::unique_ptr<QComboBox> cmdReceiverField;
  std::unique_ptr<QHBoxLayout> layout;

private:
  void initialize();

signals:

public slots:
};

#endif // COMMANDLINE_HPP
