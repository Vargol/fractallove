/*
 *  MandelbrotAvgShader.cpp
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

#include "MandelbrotAvgShader.h"

MandelbrotAvgShader::MandelbrotAvgShader(void) {
	_functionName = std::string("fnBuddabrotShader");
	_buddhaBuffer = NULL;
} 


bool MandelbrotAvgShader::initializeShader() {
	
        std::cout << "MandelbrotAvgShader::initializeShader" << std::endl;
	
	/* create PBO's **/
        _zoom = 4.0;
	_xOffset = 0.0;
	_yOffset = 0.0;	
	
	return true;

	
}

void MandelbrotAvgShader::setRenderSize(unsigned int width, unsigned int height) {

	width = width + 4 - (width & 3);
	
        std::cout << "MandelbrotAvgShader::setRenderSize "  << width << ", " << height << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

		if(_buddhaBuffer != NULL) {
			free(_buddhaBuffer);
		}
                _buddhaBuffer = (double *)malloc(height * width * sizeof(double));
		if(_imageBuffer != NULL) {
			free(_imageBuffer);
		}
		_imageBuffer = (unsigned char *)malloc(height * width * sizeof(unsigned char) * 4);
	}

	_textureWidth = width;
	_textureHeight = height;

}

bool MandelbrotAvgShader::preRenderInitialization(unsigned int shaderProgram) {

	
        std::cout << "MandelbrotAvgShader::preRenderInitialization "  << _textureWidth << ", " << _textureHeight << std::endl;

	// things thats need to be done once just before a render.
        bzero (_buddhaBuffer, _textureHeight * _textureWidth * sizeof(double));
//	memset (_buddhaBuffer, '\0', 1); 
	
	return true;

} 

bool MandelbrotAvgShader::postRendertidyUp(unsigned int shaderProgram) {

	return true;

}

void MandelbrotAvgShader::createShaderCode() {
	
	return;
	
}



void MandelbrotAvgShader::render(double top, double bottom, double left, double right) {

//	sleep(10);
	
        std::cout << "MandelbrotAvgShader::render "  << _textureWidth << ", " << _textureHeight << std::endl;


	double widthDelta, heightDelta; 
	
	
	if(_textureWidth < _textureHeight) {
		widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
		heightDelta = _zoom / (_textureHeight + 1.0);
	} else {
        heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
	    widthDelta = _zoom / (double)(_textureHeight + 1.0);
	}

	double xOffset = -(widthDelta * _textureWidth / 2.0);
	double yOffset = -(heightDelta * _textureHeight / 2.0);

	complex z0 = complex (xOffset, yOffset);
	complex c, z;
        unsigned int maxIter = 50;
	double maxIterDouble = 250.0 / (double)maxIter ;	
	complex delta = complex(widthDelta, heightDelta);

        double *buddhaBuffer = _buddhaBuffer;

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
                        zDiff = z - zOld;
                        abs = zDiff.real() * zDiff.real() + zDiff.imag() * zDiff.imag();
                        *(buddhaBuffer+(y * _textureWidth) + x) = avg / (double)i;
			z0 += widthDelta;
		} 
	} 

std::cout << "MandelbrotAvgShader::render finished mandlebrot" << std::endl;

	buddhaBuffer = _buddhaBuffer;
	unsigned char *imageBuffer = _imageBuffer;
	unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			int offset = (y * _textureWidth) + x;
//			std::cout << offset << " ";
                        byteValue = (unsigned char)(*(buddhaBuffer+offset) * 63.75);
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
	

        if(_image != NULL) {
            delete _image;
        }
        _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);
        QString str = "/Users/vargol/Source/FractGo/test.png";

        bool saved = _image->save(str);

        std::cout << "MandelbrotAvgShader::finished" << std::endl;
	
}


std::string &MandelbrotAvgShader::functionName(void) {
	return _functionName;
}


