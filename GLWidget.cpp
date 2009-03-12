/*
 *  GLWidget.cpp
 *  
 *
 *  Created by David Burnett on 22/04/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "GLWidget.h"

using namespace std;

GLWidget::GLWidget(QWidget *parent)
		: QGLWidget(parent) {

}


GLWidget::~GLWidget() {
	
}


void GLWidget::initializeGL()
{

	
		
    glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
    emit initializingOpenGL();

    cout << "initializeGL 1" << gluErrorString(glGetError()) << endl;

}



void GLWidget::paintGL()
{

    if(!isVisible()) {
            return;
    }

    cout << "paint 0:" << gluErrorString(glGetError()) << endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPolygonMode(GL_FRONT,GL_FILL);
    glMatrixMode(GL_MODELVIEW);

    cout << "paint 1:" << gluErrorString(glGetError()) << endl;

    emit renderToScreen();

    cout << "paint 2:" << gluErrorString(glGetError()) << endl;
	
}

void GLWidget::resizeGL(int width, int height) {

    cout << "resizeGL: " << width << "," << height << endl;

    glViewport(0, 0, width, height);

    emit viewportResized(width, height);

}



