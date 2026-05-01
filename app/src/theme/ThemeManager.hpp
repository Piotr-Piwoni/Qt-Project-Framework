#pragma once
#include <QDebug>
#include <QFile>
#include <qobject.h>
#include "utilities/logging/Logger.hpp"

namespace Chronicle
{
class ThemeManager final : public QObject
{
	Q_OBJECT

public:
	void SetTheme(std::string_view path);

	[[nodiscard]] QString GetStyleSheet() const;

signals:
	void OnThemeChanged();

private:
	QFile m_ThemeFile{nullptr};
	QString m_StyleSheet{""};
};
}
