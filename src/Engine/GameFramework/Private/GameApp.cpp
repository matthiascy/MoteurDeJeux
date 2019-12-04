#include <GameFramework/GameApp.hpp>
#include <GameFramework/Engine.hpp>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtGui/QOpenGLWindow>
#include <QtGui/QGuiApplication>
#include <QApplication>
#include <Editor/EditorMainWindow.hpp>
#include <QtWidgets/QSplashScreen>
#include <QtCore/QTimer>
#include <Graphics/SplashScreen.hpp>
#include <GameFramework/EngineWindow.hpp>

GameApp::GameApp(const String& name, const String& description, int argc, char** argv)
  : QObject(nullptr), m_is_initialized{false},  m_is_quit{false},
    m_start_time{0}, m_frames{0}, m_fps{0.0f}, m_elapsed_timer{}
{
  Q_INIT_RESOURCE(resources);

  QCoreApplication::setApplicationName(name);
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 0);
  format.setDepthBufferSize(24);
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

  m_app = makeUnique<QApplication>(argc, argv);

  {
    QCommandLineParser parser;
    parser.setApplicationDescription(description);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption withEditorOpt = {{"e", "with-editor"}, "Run with in-game editor."};
    parser.addOption(withEditorOpt);

    parser.process(*m_app);
    m_is_with_editor = parser.isSet(withEditorOpt);
  }

  if (m_is_with_editor) {
    qDebug() << "GameApp creation : running with editor.";
  } else {
    qDebug() << "GameApp creation : running without editor.";
  }

  m_engine = makeUnique<Engine>(this);
  m_engine->init();

  auto engine_window = new EngineWindow(this);

  if (m_is_with_editor) {
    m_window = makeUnique<EditorMainWindow>(engine_window);
  } else {
    m_window = std::unique_ptr<EngineWindow>(engine_window);
  }

  m_window->showMaximized();
  //m_engine->window()->showFullscreen();
  m_window->hide();

  m_splash = makeUnique<SplashScreen>(QPixmap{":/App/loading-engine"}, Qt::WindowStaysOnTopHint);
  m_splash->showFullScreen();
  m_splash->showMinimized();
  QTimer::singleShot(3000, m_splash.get(), [this](){
    m_splash->setPixmap(QPixmap{":/App/loading-game"});
    QTimer::singleShot(3000, m_splash.get(), &QSplashScreen::close);
  });
  QTimer::singleShot(5000, m_window.get(), &QWidget::showMaximized);

  QObject::connect(m_window.get(), &QWidget::destroyed, m_app.get(), &QCoreApplication::quit);

  m_is_initialized = true;
}

GameApp::~GameApp()
{
  qDebug() << "Shut down GameApp...";
  m_splash.reset(nullptr);
  m_window.reset(nullptr);
  m_engine.reset(nullptr);
  m_app.reset(nullptr);
  qDebug() << "Shut donw GameApp... [Done]";
}

bool GameApp::isEditorEnabled() const
{
  return m_is_with_editor;
}

StringView GameApp::name() const
{
  return QCoreApplication::applicationName();
}

void GameApp::run()
{
  qInfo() << "... Game loop running ...";

  m_elapsed_timer.start();
  m_start_time = m_elapsed_timer.elapsed();

  double dt = 1 / 60.0;
  double lastTime = (double)m_elapsed_timer.elapsed() / 1000.0;

  while (!m_is_quit) {

    Scene* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());

    double currentTime = (double)m_elapsed_timer.elapsed() / 1000.0;

    m_fps = m_frames / (currentTime - m_start_time / 1000.0);

    emit fpsChanged(m_fps);

    double frameTime = currentTime - lastTime;
    lastTime = currentTime;

    m_engine->inputSystem()->preUpdate(frameTime);
    m_engine->renderSystem()->preUpdate(frameTime);
    m_engine->inputSystem()->update(frameTime);

    // Fixed update
    while (frameTime > 0.0) {
      float deltaTime = std::min(frameTime, dt);
      m_engine->inputSystem()->fixedUpdate(deltaTime);
      //physicsSystem()->fixedUpdate()
      frameTime -= deltaTime;
    }

    m_engine->renderSystem()->renderScene(scene);

    m_frames++;

    QCoreApplication::processEvents();
  }

  qInfo() << "... Quit game loop ...";
}

void GameApp::quit()
{
  m_is_quit = true;
}


