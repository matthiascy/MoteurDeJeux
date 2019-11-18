#include <QtWidgets>
#include <iostream>
#include <QHBoxLayout>
#include "EditorMainWindow.hpp"
#include "Viewport.hpp"

EditorMainWindow::EditorMainWindow(QWidget *parent)
  : QMainWindow(parent),
    viewport(nullptr),
    _toolBar(nullptr),
    mainSplitter{nullptr},
    botLRSplitter{nullptr},
    palette(nullptr),
    commandline(nullptr),
    cmd{nullptr},
    output{nullptr},
    assetsBrowser{nullptr},
    assetPreviewer{nullptr},
    propertyEditor{nullptr},
    explorer{nullptr},
    menus{},
    fullscreen_toggle_act(nullptr),
    toolbar_toggle_act(nullptr),
    scene_bg_color_act(nullptr),
    new_level_act(nullptr),
    open_level_act(nullptr),
    save_level_act(nullptr),
    edit_group(nullptr),
    select_group(nullptr),
    transform_group(nullptr),
    global_display_group(nullptr),
    view_group(nullptr),
    story_group(nullptr),
    level_testing(nullptr),
    engine(nullptr)
{
  setupDocks();
  setupViewport();
  setupMenuBar();
  setupStatusBar();
  setupConnections();
  scene_bg_color = {42, 42, 50, 255};

  this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
  this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
  applyStyles();
  viewport->setFocus();
}

EditorMainWindow::~EditorMainWindow()
{
  for (int i = 0; i < 8; ++i) {
    if (menus[i])
      delete menus[i];

    menus[i] = nullptr;
  }

  if (_toolBar)
    delete _toolBar;

  if (viewport)
    delete viewport;

  if (palette)
    delete palette;

  if (new_level_act)
    delete new_level_act;

  if (open_level_act)
    delete open_level_act;

  if (edit_group)
    delete edit_group;

  assetsBrowser.reset(nullptr);
  assetPreviewer.reset(nullptr);
  propertyEditor.reset(nullptr);
  explorer.reset(nullptr);

  _toolBar = nullptr;
  //left_sidebar = nullptr;
  viewport = nullptr;
  palette = nullptr;

  fullscreen_toggle_act = nullptr;
  toolbar_toggle_act = nullptr;
  scene_bg_color_act = nullptr;
  new_level_act = nullptr;
  open_level_act = nullptr;
  cmd_act = nullptr;
  edit_group = nullptr;
}


/** Private functions */

void EditorMainWindow::setupLayout()
{
  mainSplitter = new QSplitter(Qt::Vertical);
  botLRSplitter = new QSplitter(Qt::Horizontal);

  this->botLRSplitter->addWidget(this->cmd);
  this->botLRSplitter->addWidget(this->output);
  QWidget* blank = new QWidget;
  blank->setMaximumWidth(10);
  blank->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  this->botLRSplitter->addWidget(blank);

  this->mainSplitter->addWidget(viewport);

  this->setCentralWidget(mainSplitter);
}

void EditorMainWindow::setupStatusBar()
{
  cmd = new CommandLine;
  cmd->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  output = new SimpleOutput;

  setupLayout();

  this->statusBar()->addPermanentWidget(botLRSplitter, 5);
  this->statusBar()->show();
  this->statusBar()->setStyleSheet("QStatusBar::item {border: none;}");
}

void EditorMainWindow::setupViewport()
{
  //viewport = new Exercise02;
  //viewport = new Viewport;
  viewport = new QOpenGLWidget;
  //viewport = new GLWidget;
  //viewport->SetClearColor(scene_bg_color);
}

void EditorMainWindow::setupMenuBar()
{
  _toolBar = new QToolBar(tr("File"));
  // ----------------------
  // MenuFile - 0
  menus[0] = menuBar()->addMenu(tr("&File"));
  new_level_act = new QAction(QIcon(":/Assets/newlight.png"), tr("New Level"), this);
  new_level_act->setShortcuts(QKeySequence::New);
  new_level_act->setStatusTip(tr("Create a new level"));
  //connect(newAct, &QAction::triggered, this, &EditorMainWindow::newFile);
  menus[0]->addAction(new_level_act);
  _toolBar->addAction(new_level_act);

  open_level_act = new QAction(QIcon(":/Assets/openlight.png"), tr("Open Level"), this);
  open_level_act->setShortcuts(QKeySequence::Open);
  open_level_act->setStatusTip(tr("Open a level"));
  //    connect(openAct, &QAction::triggered, this, &EditorMainWindow::open);
  menus[0]->addAction(open_level_act);
  _toolBar->addAction(open_level_act);

  save_level_act = new QAction(QIcon(":/Assets/savelight.png"), tr("Save Level"), this);
  save_level_act->setShortcuts(QKeySequence::Save);
  save_level_act->setStatusTip(tr("Save current level to disk"));
  //connect(saveAct, &QAction::triggered, this, &EditorMainWindow::save);
  menus[0]->addAction(save_level_act);
  _toolBar->addAction(save_level_act);

  QAction *saveAsAct = menus[0]->addAction(QIcon(":/Assets/saveaslight"), tr("Save Level As"), this, &EditorMainWindow::about);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the level under a new name"));
  _toolBar->addAction(saveAsAct);

  menus[0]->addAction("Save All");
  menus[0]->addAction("Project Manager");
  menus[0]->addSeparator();
  menus[0]->addAction("Import");
  menus[0]->addAction("Batch Import");
  menus[0]->addAction("Export");
  menus[0]->addSeparator();

  QMenu* menu_file_settings = menus[0]->addMenu("Settings");
  menu_file_settings->addAction("Project settings");
  menu_file_settings->addAction("Editor Settings");

  QMenu* menu_load = menus[0]->addMenu(QIcon(":/Assets/loadlight.png"), tr("&Load"));
  QMenu* menu_tp = menu_load->addMenu(tr("TPs"));
  QAction* tp1_act = new QAction(tr("TP 1"));
  tp1_act->setStatusTip(tr("Load TP 1 - HMIN317"));
  QAction* tp2_act = new QAction(tr("TP 2"));
  tp2_act->setStatusTip(tr("Load TP 2 - HMIN317"));
  menu_tp->addAction(tp1_act);
  menu_tp->addAction(tp2_act);
  connect(tp1_act, &QAction::triggered, this, &EditorMainWindow::to_delete_tp1);
  connect(tp2_act, &QAction::triggered, this, &EditorMainWindow::to_delete_tp2);
  QMenu* menu_examples = menu_load->addMenu(tr("Examples"));
  QMenu* menu_subdivision = menu_examples->addMenu(tr("Subdivisions"));
  QAction* sub_catmull_clark_act = new QAction(tr("Catmull–Clark subdivision"));
  menu_subdivision->addAction(sub_catmull_clark_act);

  menus[0]->addSeparator();

  QAction *exitAct = menus[0]->addAction(QIcon(":/Assets/shutdownlight.png"), tr("Exit"), this, &QWidget::close);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));

  // -----------------
  // Edit - 1
  menus[1] = menuBar()->addMenu(tr("&Edit"));
  menus[1]->addAction("Duplicate");
  menus[1]->addAction("Cut");
  menus[1]->addAction("Paste");
  menus[1]->addAction("Delete");
  menus[1]->addSeparator();
  // TODO:: check state change
  QAction* selectAct = menus[1]->addAction(QIcon(":/Assets/selectlight.png"), tr("Select"));
  selectAct->setCheckable(true);
  menus[1]->addAction("Deselect All");
  menus[1]->addSeparator();
  QAction* moveAct   = menus[1]->addAction(QIcon(":/Assets/movelight.png"), tr("Move"));
  moveAct->setCheckable(true);
  QAction* rotateAct = menus[1]->addAction(QIcon(":/Assets/rotatelight.png"), tr("Rotate"));
  rotateAct->setCheckable(true);
  QAction* scaleAct  = menus[1]->addAction(QIcon(":/Assets/scalelight.png"), tr("Scale"));
  scaleAct->setCheckable(true);
  QAction* placementAct = menus[1]->addAction(QIcon(":/Assets/placelight.png"), tr("Placement"));
  placementAct->setCheckable(true);
  _toolBar->addSeparator();
  _toolBar->addAction(selectAct);
  _toolBar->addAction(moveAct);
  _toolBar->addAction(rotateAct);
  _toolBar->addAction(scaleAct);
  _toolBar->addAction(placementAct);
  _toolBar->addSeparator();
  menus[1]->addSeparator();

  QMenu* engine = menus[1]->addMenu("PikaEngine");
  engine->addAction("Restart PikaEngine");
  engine->addAction("Terminate PikaEngine");

  // -------------------
  // Settings - 2

  menus[2] = menuBar()->addMenu(tr("&Settings"));
  menus[2]->addMenu(tr("L&ighting"));

  // --------------------
  // Tools - 3
  menus[3] = menuBar()->addMenu(tr("&Tools"));
  menus[3]->addAction("Asset Browser");
  menus[3]->addAction("Asset Preview");
  menus[3]->addSeparator();
  menus[3]->addAction("Material Editor");
  menus[3]->addAction("Particle Editor");
  menus[3]->addAction("Unit Editor");
  menus[3]->addAction("Texture Manager");
  menus[3]->addAction("Level Editor");
  menus[3]->addAction("State Machine Editor");
  menus[3]->addAction("Animation Manager");
  menus[3]->addAction("Lighting Editor");
  menus[3]->addAction("Script Editor");
  menus[3]->addAction("Shader Graph Editor");
  menus[3]->addAction("Scene Explorer");
  menus[3]->addSeparator();
  menus[3]->addAction("Dependency");
  menus[3]->addAction("Explorer");
  menus[3]->addAction("External Console");
  menus[3]->addAction("Level Flow");
  menus[3]->addAction("Log Console");
  menus[3]->addSeparator();
  menus[3]->addAction("Grids and Snaps");

  // --------------
  // Views - 4
  menus[4] = menuBar()->addMenu(tr("&Views"));
  menus[4]->addAction("Viewport Configurations");
  QMenu* active_viewport = menus[4]->addMenu("Set Active Viewport");
  active_viewport->addAction("Perspective");
  active_viewport->addAction("Orthographic");
  active_viewport->addAction("Front");
  active_viewport->addAction("Back");
  active_viewport->addAction("Top");
  active_viewport->addAction("Bottom");
  active_viewport->addAction("Left");
  active_viewport->addAction("Right");
  menus[4]->addSeparator();

  QMenu* menu_view_cube = menus[4]->addMenu("ViewCube");
  QAction* viewcube = menu_view_cube->addAction("Show the ViewCube");
  viewcube->setCheckable(true);
  viewcube->setChecked(true);

  menus[4]->addSeparator();

  scene_bg_color_act = new QAction(QIcon(":/Assets/palettelight.png"), tr("&Viewport Background Color"), this);
  scene_bg_color_act->setStatusTip("Change viewport background color");
  connect(scene_bg_color_act, &QAction::triggered, this, &EditorMainWindow::change_scene_bg);
  menus[4]->addAction(scene_bg_color_act);
  _toolBar->addAction(scene_bg_color_act);

  // ----------------------
  // MenuWindows - 5

  menus[5] = menuBar()->addMenu(tr("&Windows"));
  menus[5]->addAction("New Window");
  QAction* saveLayoutAct = menus[5]->addAction(tr("Save Layout"));
  connect(saveLayoutAct, &QAction::triggered, this, &EditorMainWindow::saveLayout);
  QAction* loadLayoutAct = menus[5]->addAction("Load Layout");
  connect(loadLayoutAct, &QAction::triggered, this, &EditorMainWindow::loadLayout);
  menus[5]->addAction("Reset Layout");
  menus[5]->addAction("Close Windows");
  menus[5]->addSeparator();
  windows_preferences_act = menus[5]->addAction(tr("Settings/Preferences"), this, &EditorMainWindow::windows_preferences);
  menus[5]->addSeparator();
  fullscreen_toggle_act = menus[5]->addAction(tr("Fullscreen"), this, &EditorMainWindow::fullscreen);
  fullscreen_toggle_act->setCheckable(true);
  fullscreen_toggle_act->setChecked(false);
  toolbar_toggle_act = menus[5]->addAction(tr("Show Tool Bar"), this, &EditorMainWindow::show_toolbar);
  toolbar_toggle_act->setCheckable(true);
  toolbar_toggle_act->setChecked(true);

  // ---------------------------
  // MenuLearning - 6
  menus[6] = menuBar()->addMenu(tr("&Learning"));
  QMenu* opengl = menus[6]->addMenu(tr("OpenGL 3.1+"));
  //QMenu* opengl =
  menus[6]->addMenu(tr("Vulkan"));
  QMenu* debutant = opengl->addMenu(tr("Getting Started"));
  QMenu* lighting = opengl->addMenu(tr("Lighting"));
  QMenu* model_loading = opengl->addMenu(tr("Model Loading"));
  QMenu* advanced_opengl = opengl->addMenu(tr("Advanced OpenGL"));
  QMenu* advanced_lighting = opengl->addMenu(tr("Advanced Lighting"));
  QMenu* pbr = opengl->addMenu(tr("PBR"));
  //QAction* hello_triangle_act =
  debutant->addAction(tr("Hello Triangle"));
  //QAction* shaders_basics_act =
  debutant->addAction(tr("Shader Basics"));
  //QAction* textures_basics_act =
  debutant->addAction(tr("Textures Basics"));
  //QAction* transformations_act =
  debutant->addAction(tr("Transformations"));
  debutant->addAction(tr("Coordinate Systems"));
  debutant->addAction(tr("Camera"));
  lighting->addAction(tr("Colors"));
  lighting->addAction(tr("Basic Lighting"));
  lighting->addAction(tr("Materials"));
  lighting->addAction(tr("Lighting Maps"));
  lighting->addAction(tr("Light Casters"));
  lighting->addAction(tr("Multiple Lights"));
  model_loading->addAction(tr("Mesh"));
  model_loading->addAction(tr("Model"));
  advanced_opengl->addAction(tr("Depth Testing"));
  advanced_opengl->addAction(tr("Stencil Testing"));
  advanced_opengl->addAction(tr("Blending"));
  advanced_opengl->addAction(tr("Face Culling"));
  advanced_opengl->addAction(tr("Framebuffers"));
  advanced_opengl->addAction(tr("Cubemaps"));
  advanced_opengl->addAction(tr("Advanced Data"));
  advanced_opengl->addAction(tr("Advanced GLSL"));
  advanced_opengl->addAction(tr("Geometry Shader"));
  advanced_opengl->addAction(tr("Instancing"));
  advanced_opengl->addAction(tr("Anti Aliasing"));
  advanced_lighting->addAction(tr("Gamma Correction"));
  QMenu* shadows = advanced_lighting->addMenu(tr("Shadows"));
  shadows->addAction(tr("Shadow Mapping"));
  shadows->addAction(tr("Point Shadows"));
  advanced_lighting->addAction("Normal Mapping");
  advanced_lighting->addAction(tr("Parallax Mapping"));
  advanced_lighting->addAction(tr("HDR"));
  advanced_lighting->addAction(tr("Bloom"));
  advanced_lighting->addAction(tr("Deferred Shading"));
  advanced_lighting->addAction(tr("SSAO"));
  pbr->addAction(tr("Lighting"));
  pbr->addAction(tr("IBL"));
  menus[6]->addMenu("Shaders");

  // --------------------------------
  // MenuHelp - 7

  menus[7] = menuBar()->addMenu(tr("&Help"));
  //QAction* aboutAction =
  QMenu* doc = menus[7]->addMenu(tr("Documentation"));
  doc->addAction(tr("C++"));
  doc->addAction(tr("Rust"));
  doc->addAction(tr("Guile"));
  menus[7]->addAction(tr("&About"), this, &EditorMainWindow::about);

  cmd_act = _toolBar->addAction(QIcon(":/Assets/cmdlight.png"), tr("Command"), this, &EditorMainWindow::command);

  /** Launch/debug button */
  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _toolBar->addWidget(spacer);
  _toolBar->addSeparator();
  launch_act = new QAction(QIcon(":/Assets/launch.png"), tr("&New Scene"), this);
  connect(launch_act, &QAction::triggered, this, &EditorMainWindow::launch);
  _toolBar->addAction(launch_act);
  _toolBar->addSeparator();
  //QAction* debug_act =
  _toolBar->addAction(QIcon(":/Assets/debuglight.png"), tr("Debug"), this, &EditorMainWindow::debug);

  this->addToolBar(Qt::LeftToolBarArea, _toolBar);
}

void EditorMainWindow::setupDocks()
{
  this->assetsBrowser = std::make_unique<AssetsBrowser>();
  QDockWidget* dockAssets = new QDockWidget(tr("Assets Browser"));
  dockAssets->setWidget(assetsBrowser.get());
  dockAssets->setStyleSheet("color: #EEEEEE; background: #414345");
  this->addDockWidget(Qt::BottomDockWidgetArea, dockAssets);

  this->assetPreviewer = std::make_unique<AssetPreviewer>();
  QDockWidget* dockPreviewer = new QDockWidget(tr("Asset Preview"));
  dockPreviewer->setWidget(assetPreviewer.get());
  dockPreviewer->setStyleSheet("color: #EEEEEE; background: #414345");
  this->addDockWidget(Qt::BottomDockWidgetArea, dockPreviewer);

  this->propertyEditor = std::make_unique<PropertyEditor>();
  QDockWidget* dockProperty = new QDockWidget(tr("Property Editor"));
  dockProperty->setWidget(propertyEditor.get());
  dockProperty->setStyleSheet("color: #EEEEEE; background: #414345;");
  this->addDockWidget(Qt::RightDockWidgetArea, dockProperty);

  this->explorer = std::make_unique<Explorer>();
  QDockWidget* dockExplorer = new QDockWidget(tr("Explorer"));
  dockExplorer->setWidget(explorer.get());
  dockExplorer->setStyleSheet("color: #EEEEEE; background: #414345;");
  this->addDockWidget(Qt::RightDockWidgetArea, dockExplorer);
}

void EditorMainWindow::setupColorPicker()
{
  palette = new QColorDialog(this);
  palette->setCurrentColor(scene_bg_color);
  palette->setOption(QColorDialog::ShowAlphaChannel);
}

void EditorMainWindow::applyStyles()
{
  // this->setWindowFlags(Qt::SplashScreen);
  // TODO: isolate EditorMainWindow's style with docked widgets' style.
  this->setStyleSheet("background-color: #303133; QTabBar::tab {background-color: #414345; color: #EEEEEE;};");
  menuBar()->setStyleSheet("QMenuBar { color: #EEEEEE; background-color: #414345; }"
                           "QMenuBar::item:selected:enabled{ background:#4286f4; }"
                           "QMenuBar::item:selected:!enabled{ background:transparent;}");

  for (int i = 0; i < 8; ++i) {
    menus[i]->setStyleSheet("QMenu{ color: #EEEEEE; padding:5px; border:1px solid gray;}"
                            "QMenu::item{ padding:0px 15px 0px 25px; height:25px;}"
                            "QMenu::item:selected:enabled{ background:#4286f4;}"
                            "QMenu::item:selected:!enabled{ background:transparent;}");
  }

  this->viewport->setStyleSheet("QMenuBar { color: #EEEEEE; background-color: #414345;}"
                                "QMenuBar::item:selected:enabled{ background:#4286f4; }"
                                "QMenuBar::item:selected:!enabled{ background:transparent;}"
                                "QMenu{ color: #EEEEEE; padding:5px; border:1px solid gray;}"
                                "QMenu::item{ padding:0px 15px 0px 25px; height:25px;}"
                                "QMenu::item:selected:enabled{ background:#4286f4;}"
                                "QMenu::item:selected:!enabled{ background:transparent;}");

  statusBar()->setStyleSheet("color: #EEEEEE; background-color: #414345; border: none;");
  _toolBar->setStyleSheet("QToolBar { color: #EEEEEE; background-color: #414345; border: none; spacing: 3px}");
  _toolBar->setMovable(false);
  this->cmd->SetStyle();
  this->assetsBrowser->SetStyle();
  this->assetPreviewer->SetStyle();
  this->propertyEditor->SetStyle();
  this->explorer->SetStyle();
}

void EditorMainWindow::setupConnections()
{
  QObject::connect(this->assetsBrowser->ListView(), &QListView::clicked,
                   this->assetPreviewer.get(),
                   [&](const QModelIndex& idx){
      assetPreviewer->preview(assetsBrowser->ListFileItem(idx));
    });
}

/** Slots */

void EditorMainWindow::about()
{
  QMessageBox* about = new QMessageBox(this);
  about->setWindowTitle("About GemTD-Knockoff");
  about->setStyleSheet("color: #EEEEEE; background-color: #5D5D5D; QMessageBox::icon { width: 0px; height: 0px; }");
  about->setWindowFlag(Qt::FramelessWindowHint);
  about->setTextFormat(Qt::RichText);
  about->setText(tr("<p align='center'>GemTD <br> Knockoff version of a Dota2 RPG map.</p>"));
  about->addButton(QMessageBox::Ok);
  about->button(QMessageBox::Ok)->setStyleSheet("border: none; background-color: #6D6D6D;");
  about->exec();
}

void EditorMainWindow::fullscreen()
{
  if (windowState() == Qt::WindowFullScreen)
    this->setWindowState(Qt::WindowMaximized);
  else
    this->setWindowState(Qt::WindowFullScreen);
}

void EditorMainWindow::show_toolbar()
{
  if (_toolBar->isHidden()) {
    _toolBar->show();
  } else {
    _toolBar->hide();
  }
}

void EditorMainWindow::change_scene_bg()
{
  QColor color = palette->getColor(scene_bg_color, this,
                                       tr("Scene Background Color"),
                                       QColorDialog::DontUseNativeDialog);
  if (color.isValid()) {
    scene_bg_color = color;
    //viewport->SetClearColor(scene_bg_color);
  }

  viewport->update();
}

void EditorMainWindow::windows_preferences()
{

}

void EditorMainWindow::launch()
{

}

void EditorMainWindow::debug()
{

}

void EditorMainWindow::command()
{

}

void EditorMainWindow::to_delete_tp1()
{

}

void EditorMainWindow::to_delete_tp2()
{

}

void EditorMainWindow::actionTriggered(QAction* action)
{
  this->output->setText(action->text());
}

void EditorMainWindow::closeEvent(QCloseEvent* event)
{
  event->accept();
}

void EditorMainWindow::saveLayout()
{
  QString filename = QFileDialog::getSaveFileName(this, tr("Save layout"));

  if (filename.isEmpty())
    return;

  QFile file(filename);
  if (!file.open(QFile::WriteOnly)) {
    QString msg = tr("Failed to open %1\n%2").
                  arg(QDir::toNativeSeparators(filename),
                      file.errorString());
    QMessageBox::warning(this, tr("Error"), msg);
    return;
  }

  QByteArray geo_data = saveGeometry();
  QByteArray layout_data = saveState();

  bool ok = file.putChar(static_cast<char>(geo_data.size()));
  if (ok)
    ok = file.write(geo_data) == geo_data.size();
  if (ok)
    ok = file.write(layout_data) == layout_data.size();

  if (!ok) {
    QString msg = tr("Error writing to %1\n%2")
                  .arg(QDir::toNativeSeparators(filename),
                       file.errorString());
    QMessageBox::warning(this, tr("Error"), msg);
    return;
  }
}

void EditorMainWindow::loadLayout()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load layout"));
  if (fileName.isEmpty())
    return;
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) {
    QString msg = tr("Failed to open %1\n%2")
                  .arg(QDir::toNativeSeparators(fileName), file.errorString());
    QMessageBox::warning(this, tr("Error"), msg);
    return;
  }

  char geo_size;
  QByteArray geo_data;
  QByteArray layout_data;

  bool ok = file.getChar(&geo_size);
  if (ok) {
    geo_data = file.read(geo_size);
    ok = geo_data.size() == geo_size;
  }
  if (ok) {
    layout_data = file.readAll();
    ok = layout_data.size() > 0;
  }

  if (ok)
    ok = restoreGeometry(geo_data);
  if (ok)
    ok = restoreState(layout_data);

  if (!ok) {
    QString msg = tr("Error reading %1").arg(QDir::toNativeSeparators(fileName));
    QMessageBox::warning(this, tr("Error"), msg);
    return;
  }
}
