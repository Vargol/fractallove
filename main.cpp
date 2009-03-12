/*
 *  main.cpp
 *  
 *
 *  Created by David Burnett on 22/04/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "main.h"

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
//	GLWidget window;
	MainWindow window;
        window.resize(800,500);
	window.show();
	return app.exec();
}
