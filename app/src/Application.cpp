#include "Application.hpp"

namespace Chronicle
{
Application::Application(int argc, char* argv[], const QSize windSize)
{
	m_QtApp = std::make_unique<QApplication>(argc, argv);
	QApplication::setApplicationName("Chronicle");

	// Enable logging.
	auto filePath = QCoreApplication::applicationDirPath() + "/app.log";
	Utils::QtLoggerAdapter::Init(filePath.toStdString());

	// Create and connect the Theme Manager.
	m_ThemeManager = std::make_unique<ThemeManager>();
	QObject::connect(m_ThemeManager.get(), &ThemeManager::OnThemeChanged, m_QtApp.get(),
					 [this] {
						 m_QtApp->setStyleSheet(m_ThemeManager->GetStyleSheet());
					 });
	m_ThemeManager->SetTheme(":/themes/dark-mode");

	// Create the context window.
	m_Window = std::make_unique<QWidget>();
	m_Window->setObjectName("context");
	m_Window->resize(windSize);
}
}
