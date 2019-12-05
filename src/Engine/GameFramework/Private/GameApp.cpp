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
#include <QDesktopWidget>
#include <QScreen>

GameApp::GameApp(const String& name, const String& description, QSize&& minSize, int argc, char** argv)
  : QObject(nullptr), m_is_initialized{false}, m_is_quit{false},
    m_start_time{0}, m_frames{0}, m_fps{0.0f}, m_elapsed_timer{}, m_win_min_size{minSize}
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

  m_engine = makeUnique<Engine>(this);
  m_engine->init();

  if (m_is_with_editor) {

    qDebug() << "GameApp creation : running with editor.";
    m_window = makeUnique<EditorMainWindow>(new EngineWindow(this));

  } else {

    qDebug() << "GameApp creation : running without editor.";
    m_window = makeUnique<EngineWindow>(this);

  }

  auto screenGeometry = QApplication::screens()[0]->availableGeometry();

  m_window->setMinimumSize(m_win_min_size);
  m_window->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,m_window->size(), screenGeometry));
  //m_window->showMinimized();
  m_window->show();
  m_window->hide();

  m_splash = makeUnique<SplashScreen>(QPixmap{":/App/loading-engine"}.scaled(m_window->geometry().size()), Qt::WindowStaysOnTopHint);
  m_splash->setMinimumSize(m_win_min_size);
  m_splash->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, m_splash->size(), QApplication::screens()[0]->availableGeometry()));
  //m_splash->showFullScreen();
  //m_splash->showMaximized();
  m_splash->showNormal();

  QTimer::singleShot(3000, m_splash.get(), [this](){
    m_splash->setPixmap(QPixmap{":/App/loading-game"}.scaled(m_window->geometry().size()));
    m_splash->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, m_splash->size(), QApplication::screens()[0]->availableGeometry()));
    QTimer::singleShot(3000, m_splash.get(), &QSplashScreen::close);
  });

  m_window->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,m_window->size(), screenGeometry));

  QTimer::singleShot(5000, m_window.get(), [this](){
    m_window->show();
    m_is_initialized = true;
    m_window->setFocus();
  });

  // TODO::run after loading
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

void GameApp::setMinimumSize(QSize&& size)
{
  m_win_min_size = size;
  m_window->setMinimumSize(m_win_min_size);
}

