/*
 *  MandelbrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <QImage>

#include "JuliaOrbitalTrap.h"

JuliaOrbitalTrap::JuliaOrbitalTrap () {

        _glw->makeCurrent();
        glGenTextures(1, &_trapTexture);
        glActiveTexture(GL_TEXTURE1);
        createJuliaShader();
        glGenFramebuffersEXT(1, &_fbo);
        _imageBuffer = NULL;
}

bool JuliaOrbitalTrap::initializeShader() {
	
        QImage file = QImage("basic00.jpg");
	QImage trap = QGLWidget::convertToGLFormat(file);
	
	std::cout << "file:" << file.depth();
	std::cout << "trap:" << trap.depth();
		
        createRenderTexture(_trapTexture, trap.width(), trap.height(), trap.bits());

//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _trapTexture, 0);


	return true;

	
}

bool JuliaOrbitalTrap::preRenderInitialization() {

	// things thats need to be done once just before a render.

	
        glUseProgram(_shaderProgram);
	
	glActiveTexture(GL_TEXTURE1);	
        glBindTexture(GL_TEXTURE_2D, _trapTexture);
	
        glUniform1i(glGetUniformLocation(_shaderProgram,"trap"), 1); // texunit 1
        glUniform2f(glGetUniformLocation(_shaderProgram,"zoom"), _zoom, _zoom); // texunit 1
        glUniform2f(glGetUniformLocation(_shaderProgram,"offset"), _xOffset, _yOffset); // texunit 1

	return true;

} 

bool JuliaOrbitalTrap::postRendertidyUp() {

	
        glDeleteTextures(1, &_trapTexture);
        glDetachShader(_shaderProgram, _fragShader);
        glDeleteShader(_fragShader);
        glDeleteProgram(_shaderProgram);
	
	return true;

}

void JuliaOrbitalTrap::createJuliaShader() {
	
	
	
	std::string shader = 
	" uniform sampler2D trap;"
	" uniform vec2 zoom;"
	" uniform vec2 offset;"
	"void main ()"
	"{"
	"	vec2 z = gl_TexCoord[0].xy;"
	"     z -= offset;"
	"     z *= zoom;"
	"	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);" 
	"	for (float i = 0.0; i < 1000.0; i += 1.0)"
	"	{"
	"		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + vec2(0.6, 0.7);"
//	"		if (all(greaterThanEqual(z, vec2(-0.5, -0.5))) && all(lessThanEqual(z, vec2(0.5, 0.5))))"
	"		if (dot(z, z) < 0.5)"
	"		{"
	"			gl_FragColor = texture2D(trap, z + vec2(0.5, 0.5)); "
	"			break;"
	"		}"
	"	}"
	"}";
	
	
        _fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar *shader_string =  (GLchar *)shader.data();
        glShaderSource(_fragShader, 1, &shader_string, NULL);
        glCompileShader(_fragShader);
	
        printShaderInfoLog(_fragShader);
	
        _shaderProgram = glCreateProgram();
        glAttachShader(_shaderProgram, _fragShader);
        glLinkProgram(_shaderProgram);
        glValidateProgram(_shaderProgram);

	
        printShaderInfoLog(_fragShader);
	
        printProgramInfoLog(_shaderProgram);
}

void JuliaOrbitalTrap::setRenderSize(unsigned int width, unsigned int height) {

    width = width + 4 - (width & 3);
    if(_textureWidth != width || _textureHeight != height) {

        //            we need to resize the texture and the FBO
        glDeleteTextures(1, &_renderToTexture);
        glGenTextures(1, &_renderToTexture);
        glBindTexture(GL_TEXTURE_2D, _renderToTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, _renderToTexture, 0);

        if(_imageBuffer != NULL) {
            free(_imageBuffer);
        }
       _imageBuffer = (unsigned char *)malloc(4 * height * width * sizeof(unsigned char));
       if(_imageBuffer == NULL) {
           std::cout << "Failed to allocate image buffer size " << (4 * height * width * sizeof(unsigned char));
       }
    }
    _textureWidth = width;
    _textureHeight = height;

}

void JuliaOrbitalTrap::render () {

       _glw->makeCurrent();

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);

        preRenderInitialization();

        glPolygonMode(GL_FRONT,GL_FILL);

        glBegin(GL_QUADS);
        //	glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
        glTexCoord2f(0.0, 0.0);
        glVertex2f(0.0, _textureHeight);

        //	glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Red
        glTexCoord2f(1.0, 0.0);
        glVertex2f(_textureWidth, _textureHeight);

        //	glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Red
        glTexCoord2f(1.0, 1.0);
        glVertex2f(_textureWidth, 0.0);

        //	glColor3f(0.0f,0.0f,0.0f);			// Set The Color To Red
        glTexCoord2f(0.0, 1.0);
        glVertex2f(0.0, 0.0);
        glEnd();

        glGetTexImage( _renderToTexture, 0, GL_RGBA, GL_UNSIGNED_BYTE, _imageBuffer);

       _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);

        QString str = "/home/david/test.png";

        bool saved = _image->save(str);

        std::cout << "Saved image " << saved <<  std::endl;

}

QLayout *JuliaOrbitalTrap::getParameterLayout(void) {



        QGridLayout *parameterLayout = new QGridLayout;

  //      parameterLayout->addWidget(new QLabel(tr("Fractal: ")), 1,0);
  ///      parameterLayout->addWidget(_fractals, 1,1);


        parameterLayout->addWidget(new QLabel(tr("Centre x: ")), 1, 0);
        parameterLayout->addWidget(new QLabel(tr("Centre y: ")), 2, 0);
        parameterLayout->addWidget(new QLabel(tr("Zoom: ")), 3, 0);

       QDoubleSpinBox *_xOffsetSpinBox = new QDoubleSpinBox;
        _xOffsetSpinBox->setMinimum(-99.9);
        _xOffsetSpinBox->setDecimals(9);
        _xOffsetSpinBox->setValue(_xOffset);
        parameterLayout->addWidget(_xOffsetSpinBox, 1, 1);

        QDoubleSpinBox *_yOffsetSpinBox = new QDoubleSpinBox;
        _yOffsetSpinBox->setMinimum(-99.9);
        _yOffsetSpinBox->setDecimals(9);
        _yOffsetSpinBox->setValue(_yOffset);
        parameterLayout->addWidget(_yOffsetSpinBox, 2, 1);



        QDoubleSpinBox *_zoomSpinBox = new QDoubleSpinBox;
        _zoomSpinBox->setMinimum(-99.9);
        _zoomSpinBox->setDecimals(9);
        _zoomSpinBox->setValue(_zoom);

        parameterLayout->addWidget(_zoomSpinBox, 3, 1);

        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXCentre(double)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYCentre(double)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom(double)));

        _parameterLayout    = parameterLayout;

    return _parameterLayout;

 }

void JuliaOrbitalTrap::setZoom(double value) {

        _zoom = value;
}


void JuliaOrbitalTrap::setXCentre(double x) {


        _xOffset = x;

}

void JuliaOrbitalTrap::setYCentre(double y) {

        _yOffset = -y;

}

