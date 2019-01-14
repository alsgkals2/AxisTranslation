#include "AxisTransTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AxisTransTool w;
	w.show();
	return a.exec();
}
