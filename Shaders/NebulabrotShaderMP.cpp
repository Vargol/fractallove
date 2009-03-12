/*
 *  NebulabrotShaderMP.cpp
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

#include "NebulabrotShaderMP.h"

NebulabrotShaderMP::NebulabrotShaderMP(void) {
	_functionName = std::string("fnBuddabrotShader");
	_buddhaRedBuffer = NULL;
	_buddhaGreenBuffer = NULL;
	_buddhaBlueBuffer = NULL;
	_imageBuffer = NULL;
	_redraw = true;
} 


bool NebulabrotShaderMP::initializeShader() {
	
	
	/* create PBO's **/
	_zoom = 4.0;
	_xOffset = 0.0;
	_yOffset = 0.0;	
	
	return true;

	
}

void NebulabrotShaderMP::setRenderSize(unsigned int width, unsigned int height) {

	width = width + 4 - (width & 3);
	
	std::cout << "NebulabrotShaderMP::setRenderSize "  << width << ", " << height << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

		if(_buddhaRedBuffer != NULL) {
			free(_buddhaRedBuffer);
		}
		_buddhaRedBuffer = (unsigned int *)malloc(height * width * sizeof(unsigned int));

		if(_buddhaGreenBuffer != NULL) {
			free(_buddhaGreenBuffer);
		}
		_buddhaGreenBuffer = (unsigned int *)malloc(height * width * sizeof(unsigned int));

		if(_buddhaBlueBuffer != NULL) {
			free(_buddhaBlueBuffer);
		}
		_buddhaBlueBuffer = (unsigned int *)malloc(height * width * sizeof(unsigned int));


		if(_imageBuffer != NULL) {
			free(_imageBuffer);
		}
		_imageBuffer = (unsigned char *)malloc(height * width * sizeof(unsigned char) * 4);
		
		_redraw = true;
	}

	_textureWidth = width;
	_textureHeight = height;

}

bool NebulabrotShaderMP::preRenderInitialization(unsigned int shaderProgram) {

	
	std::cout << "NebulabrotShaderMP::preRenderInitialization "  << _textureWidth << ", " << _textureHeight << std::endl;

	// things thats need to be done once just before a render.
	bzero (_buddhaRedBuffer, _textureHeight * _textureWidth * sizeof(unsigned int)); 
	bzero (_buddhaGreenBuffer, _textureHeight * _textureWidth * sizeof(unsigned int)); 
	bzero (_buddhaBlueBuffer, _textureHeight * _textureWidth * sizeof(unsigned int)); 
//	memset (_buddhaBuffer, '\0', 1); 
	
	return true;

} 

bool NebulabrotShaderMP::postRendertidyUp(unsigned int shaderProgram) {

	return true;

}

void NebulabrotShaderMP::createShaderCode() {
	
	return;
	
}

void NebulabrotShaderMP::renderBuddha() {

	std::cout << "NebulabrotShaderMP::renderBuddha "  << _textureWidth << ", " << _textureHeight << std::endl;
	
	
	double widthDelta, heightDelta; 
	
	
	if(_textureWidth < _textureHeight) {
		widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
		heightDelta =  _zoom / (_textureHeight + 1.0);
	} else {
        heightDelta =  _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
	    widthDelta =  _zoom / (double)(_textureHeight + 1.0);
	}
	
	double xMin = -(widthDelta * _textureWidth / 2.0) + _xOffset;
	double yMin = -(heightDelta * _textureHeight / 2.0) + _yOffset;
	
	double xMax = xMin + (widthDelta * _textureWidth);
	double yMax = yMin + (heightDelta * _textureHeight);
	
	int xPlot, yPlot;
	
	complex c, z, z0;
        unsigned int maxIter = 2000;
	complex delta = complex(widthDelta, heightDelta);
	
	unsigned int *buddhaRedBuffer = _buddhaRedBuffer;
	unsigned int *buddhaGreenBuffer = _buddhaGreenBuffer;
	unsigned int *buddhaBlueBuffer = _buddhaBlueBuffer;
	
        unsigned int redIter = maxIter;
        unsigned int greenIter = maxIter / 10;
        unsigned int blueIter = maxIter / 100;
	
	double redIterDouble = 255.0 / (double)redIter * 0.01;	
	double greenIterDouble = 255.0 / (double)greenIter * 0.01 ;	
	double blueIterDouble = 255.0 / (double)blueIter * 0.005 ;	
	
        unsigned int i, j;
	
        unsigned int x;
	
	complex temp;
	
#pragma omp parallel for ordered schedule(static) private(i, j, x, z, z0, xPlot, yPlot)
        for(unsigned int y=0; y<_textureHeight * 20; y++) {
		for(x=0; x<_textureWidth * 20; x++) {
			z0 = complex (xMin, yMin);
			z0 +=  complex(x * widthDelta * 0.05, y * heightDelta * 0.05);
			z = z0;
			for (i = 0; i < maxIter; i++) {
				if(z.real() * z.real() + z.imag() * z.imag() >=  4.0) {
					z0 = complex (xMin, yMin);
					z0 +=  complex(x * widthDelta * 0.05, y * heightDelta * 0.05);
					z = z0;
					for (j = 0; j < i; j++) {
						z = z*z + z0;
						if (z.real() >= xMin && z.real() <= xMax &&
							z.imag() >= yMin && z.imag() <= yMax) {
							xPlot = (z.real() - xMin) / widthDelta;
							yPlot = (z.imag() - yMin) / heightDelta;
							if(j < redIter) {
								(*(buddhaRedBuffer+(yPlot * _textureWidth) + xPlot))++;							
							}
							if(j < greenIter) {
								(*(buddhaGreenBuffer+(yPlot * _textureWidth) + xPlot))++;							
							}
							if(j < blueIter) {
								(*(buddhaBlueBuffer+(yPlot * _textureWidth) + xPlot))++;							
							}
						} 
					}
					break;
				}
				z = z*z + z0;
			}
			
			
			//			z0 += widthDelta;
		} 
	} 
	
	std::cout << "NebulabrotShaderMP::renderBuddha finished mandlebrot" << std::endl;

	int offset;

#pragma omp for schedule(static) private (x, offset)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			offset = (y * _textureWidth) + x;
			*(_imageBuffer  + (offset * 4)) = (unsigned char)(*(_buddhaRedBuffer+offset) * redIterDouble);
			*(_imageBuffer  + (offset * 4) + 1) =  (unsigned char)(*(_buddhaGreenBuffer+offset) * greenIterDouble);
			*(_imageBuffer  + (offset * 4) + 2) = (unsigned char)(*(_buddhaBlueBuffer+offset) * blueIterDouble);
			*(_imageBuffer  + (offset * 4) + 3) = 1.0;
		}
		//		std::cout << "next ";
	}	

	std::cout << "NebulabrotShaderMP::renderBuddha finished image" << std::endl;

}

void NebulabrotShaderMP::render(double top, double bottom, double left, double right) {


        renderBuddha();
	
	QImage image = QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);
	QString str = "/Users/vargol/Source/FractGo/test.png";

	bool saved = image.save(str);

	std::cout << "Saved image " << saved <<  std::endl;

	std::cout << "NebulabrotShaderMP::render finished" << std::endl;
	
}


std::string &NebulabrotShaderMP::functionName(void) {
	return _functionName;
}


