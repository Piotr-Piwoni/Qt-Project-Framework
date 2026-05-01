#include <QHBoxLayout>
#include "Application.hpp"

int main(const int argc, char* argv[])
{
	Chronicle::Application app{argc, argv, {800, 500}};

	// ReSharper disable once CppDFAMemoryLeak
	auto* characterSheetLayout = new QVBoxLayout(&app.GetWindow());
	// ReSharper disable once CppDFAMemoryLeak
	auto header = new QWidget();
	header->setMinimumHeight(50);
	header->setMaximumHeight(112);
	header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	characterSheetLayout->addWidget(header);


	// ReSharper disable once CppDFAMemoryLeak
	auto contentLayout = new QHBoxLayout();
	characterSheetLayout->addLayout(contentLayout);

	// Left Side.
	// ReSharper disable once CppDFAMemoryLeak
	auto infoLayout = new QVBoxLayout();
	contentLayout->addLayout(infoLayout, 1);

	// ReSharper disable once CppDFAMemoryLeak
	auto* infoBox1 = new QWidget();
	// ReSharper disable once CppDFAMemoryLeak
	auto infoBox2 = new QWidget();
	infoLayout->addWidget(infoBox1, 1);
	infoLayout->addWidget(infoBox2, 1);

	// Right Side.
	// ReSharper disable once CppDFAMemoryLeak
	auto skillBox = new QWidget();
	contentLayout->addWidget(skillBox, 2);

	characterSheetLayout->setContentsMargins(10, 10, 10, 10);
	characterSheetLayout->setSpacing(10);

	app.GetWindow().show();
	return QApplication::exec();
}
