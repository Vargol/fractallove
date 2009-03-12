/*
 *  MandelbrotBoostShaderMP.cpp
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

#include <QImageWriter>

typedef std::complex<double> complex;

#include "MandelbrotBoostShaderMP.h"

MandelbrotBoostShaderMP::MandelbrotBoostShaderMP(void) {
	_functionName = std::string("fnBuddabrotShader");
	_buddhaBuffer = NULL;
} 


bool MandelbrotBoostShaderMP::initializeShader() {
	
	std::cout << "MandelbrotBoostShaderMP::initializeShader" << std::endl;

	/* create PBO's **/
	_zoom = 4.0;
	_xOffset = 0.0;
	_yOffset = 0.0;	
	
	return true;

	
}

void MandelbrotBoostShaderMP::setRenderSize(unsigned int width, unsigned int height) {

	width = width + 4 - (width & 3);
	
	std::cout << "MandelbrotBoostShaderMP::setRenderSize "  << width << ", " << height << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

		if(_buddhaBuffer != NULL) {
			free(_buddhaBuffer);
		}
		_buddhaBuffer = (unsigned int *)malloc(height * width * sizeof(unsigned int));
		if(_imageBuffer != NULL) {
			free(_imageBuffer);
		}
		_imageBuffer = (unsigned char *)malloc(height * width * sizeof(unsigned char) * 4);
	}

	_textureWidth = width;
	_textureHeight = height;

}

bool MandelbrotBoostShaderMP::preRenderInitialization(unsigned int shaderProgram) {

	
	std::cout << "MandelbrotBoostShaderMP::preRenderInitialization "  << _textureWidth << ", " << _textureHeight << std::endl;

	// things thats need to be done once just before a render.
	bzero (_buddhaBuffer, _textureHeight * _textureWidth * sizeof(unsigned int)); 
//	memset (_buddhaBuffer, '\0', 1); 
	
	return true;

} 

bool MandelbrotBoostShaderMP::postRendertidyUp(unsigned int shaderProgram) {

	return true;

}

void MandelbrotBoostShaderMP::createShaderCode() {
	
	return;
	
}



void MandelbrotBoostShaderMP::render(double top, double bottom, double left, double right) {

//	sleep(10);
	
	std::cout << "MandelbrotBoostShaderMP::render "  << _textureWidth << ", " << _textureHeight << std::endl;


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
	complex z;
        unsigned int maxIter = 500;
	double maxIterDouble = 255.0 / (double)maxIter ;	
	complex delta = complex(widthDelta, heightDelta);

	unsigned int *buddhaBuffer = _buddhaBuffer;

        unsigned int i;

        unsigned int x;
	
	complex power = complex(3.0, 0.0);
	complex relax = complex(1.1, 2.3);
	complex one = complex(1.0, 0.0);
	
#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			z0 = complex(xOffset, yOffset);
			z0 +=  complex(x * widthDelta, y * heightDelta);;
			z = z0;
			for (i = 0; i < maxIter; i++) {
				if(z.real() * z.real() + z.imag() * z.imag() >=  10.0) {
					break;
				}
				z = z - relax * (z * z *z - one) / (power * z * z) + sin(z0);
			}
			*(buddhaBuffer+(y * _textureWidth) + x) = i;
			z0 += widthDelta;
		} 
	} 

std::cout << "MandelbrotBoostShaderMP::render finished mandlebrot" << std::endl;

	buddhaBuffer = _buddhaBuffer;
	unsigned char *imageBuffer = _imageBuffer;
	unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			int offset = (y * _textureWidth) + x;
//			std::cout << offset << " ";
			byteValue = (unsigned char)(*(buddhaBuffer+offset) * (double)maxIterDouble);
			*(imageBuffer  + (offset * 4)) = byteValue;
			*(imageBuffer  + (offset * 4) + 1) = byteValue;
			*(imageBuffer  + (offset * 4) + 2) = byteValue;
			*(imageBuffer  + (offset * 4) + 3) = 0.5;
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

	std::cout << "Saved image " << saved <<  std::endl;
	
	std::cout << "MandelbrotBoostShaderMP::finished" << std::endl;
	
}


std::string &MandelbrotBoostShaderMP::functionName(void) {
	return _functionName;
}


