#include <QApplication>
#include <QMessageBox>
#include <exception>

#include <forms/MapForm.hpp>


INITIALIZE_EASYLOGGINGPP


int main(int argc, char* argv[])
{
	using namespace map_editor;

	nya::init_logs("config/log.conf");

	QApplication app(argc, argv);
	try
	{
		MapForm mainForm;
		mainForm.show();
		return app.exec();
	}
	catch (std::exception& e)
	{
		QMessageBox::warning(nullptr, "Warning!", QString::fromUtf8(e.what()), QMessageBox::Cancel);
		return 1;
	}
}
