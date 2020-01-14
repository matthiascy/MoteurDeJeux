#include <QScreen>
#include <QApplication>
#include <QtCore/QTimer>
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QtGui/QOpenGLWindow>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <GameFramework/Public/GameApp.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <Editor/EditorMainWindow.hpp>
#include <Graphics/Public/SplashScreen.hpp>
#include <GameFramework/Public/EngineWindow.hpp>
#include <GameFramework/Public/Managers/SceneManager.hpp>
#include <GameFramework/Public/Systems.hpp>
#include <Physics/Public/PhysicsDebugDraw.hpp>
#include <Graphics/Public/AnimationSystem.hpp>

/**
 * TODO: a better update mechanism for systems' update according to priority and system inter dependency
 */

GameApp::GameApp(const String& name, const String& description, QSize&& minSize, int argc, char** argv)
  : QObject(nullptr), m_is_initialized{false}, m_is_quit{false},
    m_start_time{0}, m_frames{0}, m_fps{0.0f}, m_elapsed_timer{}, m_win_min_size{minSize}
{
  Q_INIT_RESOURCE(rscs);
  Q_INIT_RESOURCE(Resources);

  qputenv("QT_MESSAGE_PATTERN",
          "\033[32m%{time h:mm:ss.zzz} "
          "%{if-debug}\033[93m[%{type}] \033[34m%{function}:\033[0m %{message}%{endif}"
          "%{if-info}\033[94m[%{type}] \033[34m%{function}:\033[0m %{message}%{endif}"
          "%{if-warning}\033[95m[%{type}] \033[34m%{function}:\033[0m %{message}%{endif}"
          "%{if-fatal}\033[91m[%{type}] \033[34m%{function}:\033[0m %{message}%{endif}");

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

    qInfo() << "Creation : running with editor";
    m_window = makeUnique<EditorMainWindow>(new EngineWindow(this), this);

  } else {

    qInfo() << "Creation : running without editor";
    m_window = makeUnique<EngineWindow>(this);
    m_window->installEventFilter(this);
    for (auto i : m_window->children())
      i->installEventFilter(this);
  }

  auto screenGeometry = QApplication::screens()[0]->availableGeometry();
  m_window->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,m_window->size(), screenGeometry));

  m_window->setMinimumSize(m_win_min_size);
  //m_window->showFullScreen();
  //m_window->showNormal();
  //m_window->setFocus();

  m_window->hide();

  m_splash = makeUnique<SplashScreen>(QPixmap{":/App/loading-engine"}.scaled(m_window->geometry().size()), Qt::WindowStaysOnTopHint);
  m_splash->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, m_splash->size(), QApplication::screens()[0]->availableGeometry()));
  m_splash = makeUnique<SplashScreen>(QPixmap{":/App/loading-engine"}, Qt::WindowStaysOnTopHint);
  m_splash->showFullScreen();

  QTimer::singleShot(3000, m_splash.get(), [this](){
    m_splash->setPixmap(QPixmap{":/App/loading-game"}.scaled(m_window->geometry().size()));
    m_splash->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, m_splash->size(), QApplication::screens()[0]->availableGeometry()));
    QTimer::singleShot(3000, m_splash.get(), &QSplashScreen::close);
  });

  m_window->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,m_window->size(), screenGeometry));

  QTimer::singleShot(5000, m_window.get(), [this](){
    //m_window->showFullScreen();
    //m_window->showMaximized();
    m_window->showNormal();
    m_is_initialized = true;
    m_window->setFocus();
  });
}

GameApp::~GameApp()
{
  qDebug() << "Shutting down...";
  m_splash.reset(nullptr);
  m_window.reset(nullptr);
  m_engine.reset(nullptr);
  m_app.reset(nullptr);
  qDebug() << "Shutting down...[Finished]";
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
  qInfo() << "Starting game loop";

  m_elapsed_timer.start();
  m_start_time = m_elapsed_timer.elapsed();

  double lastTime = (double)m_elapsed_timer.elapsed() / 1000.0;

  while (!m_is_quit) {

    //Scene* scene = m_engine->sceneManager()->sceneAt(m_engine->sceneManager()->activatedScene());

    double currentTime = (double)m_elapsed_timer.elapsed() / 1000.0;

    m_fps = m_frames / (currentTime - m_start_time / 1000.0);

    emit fpsChanged(m_fps);

    double frameTime = currentTime - lastTime;
    lastTime = currentTime;

    /** Systems' pre update */
    m_engine->inputSystem()->preUpdate(frameTime);
    m_engine->renderSystem()->preUpdate(frameTime);
    m_engine->behaviorSystem()->preUpdate(frameTime);
    m_engine->physicsSystem()->preUpdate(frameTime);
    m_engine->animationSystem()->preUpdate(frameTime);


    m_engine->inputSystem()->update(frameTime);
    m_engine->physicsSystem()->update(frameTime);
    m_engine->behaviorSystem()->update(frameTime);
    m_engine->animationSystem()->update(frameTime);

    /** Systems' fixed update */
    while (frameTime > 0.0) {
      float deltaTime = std::min(frameTime, m_dt);
      m_engine->inputSystem()->fixedUpdate(deltaTime);
      m_engine->physicsSystem()->fixedUpdate(deltaTime);
      m_engine->behaviorSystem()->fixedUpdate(deltaTime);
      frameTime -= deltaTime;
    }

    m_engine->renderSystem()->update(frameTime);

    /** Systems' post update */
    m_engine->renderSystem()->postUpdate(frameTime);
    m_engine->inputSystem()->postUpdate(frameTime);
    m_engine->physicsSystem()->postUpdate(frameTime);
    m_engine->behaviorSystem()->postUpdate(frameTime);
    m_engine->animationSystem()->postUpdate(frameTime);

    m_frames++;
    m_window->repaint();

    QCoreApplication::processEvents();
  }

  qInfo() << "Quitting game loop";
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

bool GameApp::eventFilter(QObject* object, QEvent* event)
{
  if (object == m_window.get()) {
    switch (event->type()) {
      case QEvent::KeyPress: {
        auto* e = dynamic_cast<QKeyEvent*>(event);
        if (e->isAutoRepeat())
          event->ignore();
        else {
          m_engine->inputSystem()->_register_key_press(static_cast<Qt::Key>(e->key()));
          if (e->key() == Qt::Key_1) {
            m_engine->physicsSystem()->setIsDebugDrawEnabled(!m_engine->physicsSystem()->isDebugDrawEnabled());
            if (m_engine->physicsSystem()->isDebugDrawEnabled()) {
              //m_engine->physicsSystem()->debugDrawer()->toggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
              m_engine->physicsSystem()->debugDrawer()->toggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
            }
          }
          event->accept();
        }
      } break;

      case QEvent::KeyRelease: {
        auto* e = dynamic_cast<QKeyEvent*>(event);
        if (e->isAutoRepeat())
          event->ignore();
        else {
          m_engine->inputSystem()->_register_key_release(static_cast<Qt::Key>(e->key()));
          event->accept();
        }
      } break;

      case QEvent::MouseButtonPress: {
        m_engine->inputSystem()->_register_mouse_button_press(dynamic_cast<QMouseEvent*>(event)->button());
      } break;

      case QEvent::MouseButtonRelease: {
        m_engine->inputSystem()->_register_mouse_button_release(dynamic_cast<QMouseEvent*>(event)->button());
      } break;

      case QEvent::Wheel: {
        m_engine->inputSystem()->_register_mouse_wheel(dynamic_cast<QWheelEvent*>(event));
      }

      default:
        break;
    }

    return false;

  } else {

    return QObject::eventFilter(object, event);

  }
}

