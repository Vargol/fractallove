/*
 *  MandelbrotVelocity.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#include <iostream>
#include <complex>
//#include <omp.h>

#include <QImageWriter>

typedef std::complex<double> complex;

#include "MandelbrotVelocity.h"

MandelbrotVelocity::MandelbrotVelocity(void) {
	_functionName = std::string("fnBuddabrotShader");
        _fractalBuffer = NULL;
        _imageBuffer = NULL;
} 


bool MandelbrotVelocity::initializeShader() {
	
        std::cout << "MandelbrotVelocity::initializeShader" << std::endl;

	
	/* create PBO's **/
        _zoom = 1.3;
        _xOffset = -1.78;
        _yOffset = -0.0;
	
	return true;

	
}

void MandelbrotVelocity::setRenderSize(unsigned int width, unsigned int height) {

	width = width + 4 - (width & 3);
	
        std::cout << "MandelbrotVelocity::setRenderSize "  << width << ", " << height << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

                if(_fractalBuffer != NULL) {
                        free(_fractalBuffer);
		}
                _fractalBuffer = (double *)malloc(height * width * sizeof(double));
		if(_imageBuffer != NULL) {
			free(_imageBuffer);
		}
		_imageBuffer = (unsigned char *)malloc(height * width * sizeof(unsigned char) * 4);
	}

	_textureWidth = width;
	_textureHeight = height;

}

bool MandelbrotVelocity::preRenderInitialization(unsigned int shaderProgram) {

	
        std::cout << "MandelbrotVelocity::preRenderInitialization "  << _textureWidth << ", " << _textureHeight << std::endl;

	// things thats need to be done once just before a render.
        bzero (_fractalBuffer, _textureHeight * _textureWidth * sizeof(double));
//	memset (_fractalBuffer, '\0', 1);
	
	return true;

} 

bool MandelbrotVelocity::postRendertidyUp(unsigned int shaderProgram) {

	return true;

}

void MandelbrotVelocity::createShaderCode() {
	
	return;
	
}



void MandelbrotVelocity::render(double top, double bottom, double left, double right) {

//	sleep(10);
	
        std::cout << "MandelbrotVelocity::render "  << _textureWidth << ", " << _textureHeight << std::endl;


	double widthDelta, heightDelta; 
	
	
	if(_textureWidth < _textureHeight) {
		widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
		heightDelta = _zoom / (_textureHeight + 1.0);
	} else {
        heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
	    widthDelta = _zoom / (double)(_textureHeight + 1.0);
	}

        double xOffset = -(widthDelta * _textureWidth / 2.0) + _xOffset;
        double yOffset = -(heightDelta * _textureHeight / 2.0) + _yOffset;

	complex z0 = complex (xOffset, yOffset);
	complex c, z;
        unsigned int maxIter = 50;
	double maxIterDouble = 250.0 / (double)maxIter ;	
	complex delta = complex(widthDelta, heightDelta);

        double *buddhaBuffer = _fractalBuffer;

        unsigned int i;
        unsigned int x;
	
	complex temp;
	
#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
                        double avg = 0.0;
                        double abs;
                        complex zOld, zDiff;
                        z0 = complex(xOffset, yOffset);
			z0 +=  complex(x * widthDelta, y * heightDelta);;
			z = z0;
			for (i = 0; i < maxIter; i++) {
                                abs = z.real() * z.real() + z.imag() * z.imag();
                                if(abs >=  4.0) {
					break;
				}
                                zOld = z;
				z = z*z + z0;
			}
                        if(i == maxIter) {
                            avg = 0;
                        } else {
                            zDiff = z - zOld;
                            avg = zDiff.real() * zDiff.real() + zDiff.imag() * zDiff.imag();
                        }
                        *(buddhaBuffer+(y * _textureWidth) + x) = avg / (double)i;
			z0 += widthDelta;
		} 
	} 

std::cout << "MandelbrotVelocity::render finished mandlebrot" << std::endl;

        buddhaBuffer = _fractalBuffer;
	unsigned char *imageBuffer = _imageBuffer;
	unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			int offset = (y * _textureWidth) + x;
//			std::cout << offset << " ";
                        byteValue = (unsigned char)(*(buddhaBuffer+offset) * 80.0);
			*(imageBuffer  + (offset * 4)) = byteValue;
			*(imageBuffer  + (offset * 4) + 1) = byteValue;
			*(imageBuffer  + (offset * 4) + 2) = byteValue;
                        *(imageBuffer  + (offset * 4) + 3) = 0.0;
		}
//		std::cout << "next ";
	}

	for (int i = 0; i < QImageWriter::supportedImageFormats().size(); ++i) {
		std::cout << QImageWriter::supportedImageFormats().at(i).constData()  << std::endl;
	}
	
	QImage image = QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);
	QString str = "/Users/vargol/Source/FractGo/test.png";

	bool saved = image.save(str);

	std::cout << "Saved image " << saved <<  std::endl;

        std::cout << "MandelbrotVelocity::render finished image" << std::endl;

	
}


std::string &MandelbrotVelocity::functionName(void) {
	return _functionName;
}

MandelbrotVelocity::~MandelbrotVelocity() {

                if(_fractalBuffer != NULL) {
                        free(_fractalBuffer);
                }
                if(_imageBuffer != NULL) {
                        free(_imageBuffer);
                }
}
