#pragma once
#include <QApplication>
#include <QWidget>
#include "theme/ThemeManager.hpp"
#include "utilities/QtLoggerAdapter.hpp"

namespace Chronicle
{
class Application
{
public:
	Application(int argc, char* argv[], QSize windSize);

	[[nodiscard]] QWidget& GetWindow() const { return *m_Window; }

private:
	std::unique_ptr<QApplication> m_QtApp{nullptr};
	std::unique_ptr<ThemeManager> m_ThemeManager{nullptr};

	std::unique_ptr<QWidget> m_Window{nullptr};
};
}
