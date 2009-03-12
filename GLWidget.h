/*
 *  GLWidget.h
 *  
 *
 *  Created by David Burnett on 22/04/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget {
		
	Q_OBJECT
		
	public:
		GLWidget(QWidget *parent = 0);
		~GLWidget();
	void initializeRender();

	signals:
		void viewportResized(int width, int height);	
		void renderToScreen();	
		void initializingOpenGL();	
	
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);

	
};

#endif
