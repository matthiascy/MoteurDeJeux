#ifndef EPHEMERAL_ENGINE_LEVEL_EDITOR_MAIN_WINDOW_HPP
#define EPHEMERAL_ENGINE_LEVEL_EDITOR_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QColorDialog>
#include <QSplitter>
#include <QTabWidget>
#include <QColor>
#include <QLabel>
#include <QActionGroup>
#include <QLineEdit>
#include "Editor/Viewport.hpp"
#include "Editor/CommandLine.hpp"
#include "Editor/SimpleOutput.hpp"
#include "Editor/AssetsBrowser.hpp"
#include "Editor/AssetPreviewer.hpp"
#include "Editor/PropertyEditor.hpp"
#include "Editor/Explorer.hpp"
#include "GLWidget.hpp"
#include "Window.hpp"

/*
 * TODO: Color picker style
 */

class WidgetWindow : public QMainWindow, public Window {

  Q_OBJECT

public:
  explicit WidgetWindow(QWidget* parent = nullptr);
  ~WidgetWindow() override;
  void startShowing() override;

protected:
  void closeEvent(QCloseEvent* event) override;
  void updateStatusBar();

private slots:
  void about();
  void fullscreen();
  void show_toolbar();
  void change_scene_bg();
  void windows_preferences();
  void launch();
  void debug();
  void command();

  void actionTriggered(QAction* action);
  void saveLayout();
  void loadLayout();

private:
  void setupLayout();
  void setupMenuBar();
  void setupDocks();
  void setupStatusBar();
  void setupViewport();
  void setupConnections();

  void setupColorPicker();
  void applyStyles();
  void to_delete_tp1();
  void to_delete_tp2();

private:
  //OglContext* _context;
  //Viewport* viewport;
  GLWidget* viewport;
  QToolBar* _toolBar;

  /** Layout */
  // main splitter
  QSplitter* mainSplitter;
  // top-bottom splitter
  QSplitter* botLRSplitter;

  QColorDialog* palette;
  QColor scene_bg_color;

  /** Command line */
  QLabel* commandLabel;
  QWidget* commandline;
  CommandLine* cmd;
  SimpleOutput* output;
  std::unique_ptr<AssetsBrowser> assetsBrowser;
  std::unique_ptr<AssetPreviewer> assetPreviewer;
  std::unique_ptr<PropertyEditor> propertyEditor;
  std::unique_ptr<Explorer> explorer;

  QMenu* menus[8];

  QAction* fullscreen_toggle_act;
  QAction* toolbar_toggle_act;
  QAction* scene_bg_color_act;
  QAction* new_level_act;
  QAction* open_level_act;
  QAction* save_level_act;
  QAction* windows_preferences_act;
  QAction* launch_act;
  QAction* cmd_act;

  QActionGroup* edit_group;
  QActionGroup* select_group;
  QActionGroup* transform_group;
  QActionGroup* global_display_group;
  QActionGroup* view_group;
  QActionGroup* story_group;
  QActionGroup* level_testing;
  QActionGroup* engine;
};

#endif  /* EPHEMERAL_ENGINE_LEVEL_EDITOR_MAIN_WINDOW_HPP */