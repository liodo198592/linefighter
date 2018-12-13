#include "global.h"
#include "MainWindow.h"
#include "QtCore/QTextCodec"
int main(int argc,char** argv)
{
	QApplication app(argc,argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());


	CMainWindow m;
	//m.show();


	return app.exec();
}