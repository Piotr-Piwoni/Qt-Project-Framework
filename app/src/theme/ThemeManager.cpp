#include "ThemeManager.hpp"

namespace Chronicle
{
void ThemeManager::SetTheme(const std::string_view path)
{
	m_ThemeFile.setFileName(path.data());

	if (!m_ThemeFile.exists() || !m_ThemeFile.open(QFile::ReadOnly)) return;
	m_StyleSheet = m_ThemeFile.readAll();
	m_ThemeFile.close();

	emit OnThemeChanged();
}

QString ThemeManager::GetStyleSheet() const
{
	if (m_ThemeFile.exists() && !m_StyleSheet.isEmpty() && !m_StyleSheet.isNull())
		return m_StyleSheet;

	lError("The Style sheet could not be retrieved!");
	return {""};
}
}
