/*
 *  MandelbrotShaderMP.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


/*
 
rendering buddabrot ....

Render mandlebrot, use alpha for inside outside

texture -> VBO GL_POINTS;
vertex shader does one iteration
frag shader write to two textures, one accumulates the other stores new co-ordinates
texture 2 -> VBO GL_POINTS 
 
 for bitmap
 
 
 
 
*/ 
#include <iostream>
#include <complex>
#include <limits>

#include <QImageWriter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>


//typedef std::complex<double> complex;

#include "MandelbrotShaderMP.h"


using namespace std;

MandelbrotShaderMP::MandelbrotShaderMP(void) {
	_functionName = std::string("fnMandelbrotMPShader");
        _zoom = 2.0;
        _xOffset = 0.0;
        _yOffset = 0.0;
        _iterations = 100;
        _image = NULL;
        _xOffsetSpinBox = NULL;

} 


bool MandelbrotShaderMP::initializeShader() {
	
	std::cout << "MandelbrotShaderMP::initializeShader" << std::endl;


	return true;

	
}

void MandelbrotShaderMP::setRenderSize(unsigned int width, unsigned int height) {

//	width = width + 4 - (width & 3);
	
	std::cout << "MandelbrotShaderMP::setRenderSize "  << width << ", " << height << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

                if(_resultsBuffer != NULL) {
                        free(_resultsBuffer);
		}
                _resultsBuffer = (double *)malloc(height * width * sizeof(double));

                if(_imageBuffer != NULL) {
                        free(_imageBuffer);
                }
                _imageBuffer = (unsigned char *)malloc(4 * height * width * sizeof(unsigned char));
        }

	_textureWidth = width;
	_textureHeight = height;

}

bool MandelbrotShaderMP::postRendertidyUp() {

	return true;

}




//void MandelbrotShaderMP::render(double top, double bottom, double left, double right) {
void MandelbrotShaderMP::render() {

//	sleep(10);
	
	std::cout << "MandelbrotShaderMP::render "  << _textureWidth << ", " << _textureHeight << std::endl;

        bzero (_resultsBuffer, _textureHeight * _textureWidth * sizeof(double));

	double widthDelta, heightDelta; 
	
	
	if(_textureWidth < _textureHeight) {
		widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
		heightDelta = _zoom / (_textureHeight + 1.0);
	} else {
            heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
	    widthDelta = _zoom / (double)(_textureHeight + 1.0);
	}

        double xOffset = _xOffset-(widthDelta * _textureWidth / 2.0);
        double yOffset = _yOffset-(heightDelta * _textureHeight / 2.0);

        complex<double> z0 = complex<double> (xOffset, yOffset);
        complex<double> z;

        double maxIterDouble = 250.0 / (double)_iterations ;
        complex<double> delta = complex<double>(widthDelta, heightDelta);

	double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x;
	
        complex<double> temp;
	
#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
                        z0 = complex<double>(xOffset, yOffset);
                        z0 +=  complex<double>(x * widthDelta, y * heightDelta);;
			z = z0;
                        for (i = 0; i < _iterations; i++) {
				if(z.real() * z.real() + z.imag() * z.imag() >=  4.0) {
					break;
				}
				z = z*z + z0;
			}
                        *(resultsBuffer+(y * _textureWidth) + x) = i;
//			z0 += widthDelta;
		} 
	} 

std::cout << "MandelbrotShaderMP::render finished mandlebrot" << std::endl;

        resultsBuffer = _resultsBuffer;
	unsigned char *imageBuffer = _imageBuffer;
	unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			int offset = (y * _textureWidth) + x;
//                      std::cout << offset << " ";
                        byteValue = (unsigned char)(*(resultsBuffer+offset) * 6.0);
			*(imageBuffer  + (offset * 4)) = byteValue;
			*(imageBuffer  + (offset * 4) + 1) = byteValue;
			*(imageBuffer  + (offset * 4) + 2) = byteValue;
                        *(imageBuffer  + (offset * 4) + 3) = 0.0;
                }
//		std::cout << "next ";
	}

        if(_image != NULL) {
            delete _image;
        }
        _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);

        QString str = "/home/david/test.png";

        bool saved = _image->save(str);

	std::cout << "Saved image " << saved <<  std::endl;


	
}



std::string &MandelbrotShaderMP::functionName(void) {
	return _functionName;
}

QLayout *MandelbrotShaderMP::getParameterLayout(void) {



        QGridLayout *parameterLayout = new QGridLayout;

  //      parameterLayout->addWidget(new QLabel(tr("Fractal: ")), 1,0);
  ///      parameterLayout->addWidget(_fractals, 1,1);


        parameterLayout->addWidget(new QLabel(tr("Centre x: ")), 1, 0);
        parameterLayout->addWidget(new QLabel(tr("Centre y: ")), 2, 0);
        parameterLayout->addWidget(new QLabel(tr("Zoom: ")), 3, 0);
        parameterLayout->addWidget(new QLabel(tr("Iterations: ")), 4, 0);

        _xOffsetSpinBox = new QDoubleSpinBox;
        _xOffsetSpinBox->setMinimum(-99.9);
        _xOffsetSpinBox->setDecimals(9);
        _xOffsetSpinBox->setValue(_xOffset);
        parameterLayout->addWidget(_xOffsetSpinBox, 1, 1);

        _yOffsetSpinBox = new QDoubleSpinBox;
        _yOffsetSpinBox->setMinimum(-99.9);
        _yOffsetSpinBox->setDecimals(9);
        _yOffsetSpinBox->setValue(_yOffset);
        parameterLayout->addWidget(_yOffsetSpinBox, 2, 1);


        QDoubleSpinBox *_zoomSpinBox = new QDoubleSpinBox;
        _zoomSpinBox->setMinimum(-99.9);
        _zoomSpinBox->setDecimals(9);
        _zoomSpinBox->setValue(_zoom);

        parameterLayout->addWidget(_zoomSpinBox, 3, 1);


        QSpinBox *_iterationsSpinBox = new QSpinBox;
        _iterationsSpinBox->setMinimum(1);
        _iterationsSpinBox->setMaximum(std::numeric_limits<int>::max());
        _iterationsSpinBox->setValue(_iterations);
        parameterLayout->addWidget(_iterationsSpinBox, 4, 1);

        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXCentre(double)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYCentre(double)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom(double)));
        QObject::connect(_iterationsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setIterations(int)));

        _parameterLayout    = parameterLayout;

    return _parameterLayout;

 }

void MandelbrotShaderMP::setZoom(double value) {

        _zoom = value;
}


void MandelbrotShaderMP::setXCentre(double x) {

        _xOffset = x;
}

void MandelbrotShaderMP::setYCentre(double y) {

        _yOffset = y;
}

void MandelbrotShaderMP::setIterations(int i) {

        _iterations = i;
}

void MandelbrotShaderMP::mouseReleaseEvent (QMouseEvent *event) {

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (double)(_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

        double xOffset = _xOffset -(widthDelta * _textureWidth / 2.0);
        double yOffset = _yOffset -(heightDelta * _textureHeight / 2.0);

        _xOffset = xOffset + (widthDelta * event->x());
        _yOffset = yOffset + (heightDelta * event->y());

        if(_xOffsetSpinBox != NULL) {
            _xOffsetSpinBox->setValue(_xOffset);
            _yOffsetSpinBox->setValue(_yOffset);
        }
}

