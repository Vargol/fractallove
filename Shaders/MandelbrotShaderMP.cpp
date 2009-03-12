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

#include <QImageWriter>

typedef std::complex<double> complex;

#include "MandelbrotShaderMP.h"

MandelbrotShaderMP::MandelbrotShaderMP(void) {
	_functionName = std::string("fnMandelbrotMPShader");
} 


bool MandelbrotShaderMP::initializeShader() {
	
	std::cout << "MandelbrotShaderMP::initializeShader" << std::endl;

	
	/* create PBO's **/
//	_zoom = 2.0;
//	_xOffset = 0.0;
//	_yOffset = 0.0;
	
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

	double xOffset = -(widthDelta * _textureWidth / 2.0);
	double yOffset = -(heightDelta * _textureHeight / 2.0);

	complex z0 = complex (xOffset, yOffset);
	complex c, z;
        unsigned int maxIter = 50;
	double maxIterDouble = 250.0 / (double)maxIter ;	
	complex delta = complex(widthDelta, heightDelta);

	double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x;
	
	complex temp;
	
#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0)
        for(unsigned int y=0; y<_textureHeight; y++) {
		for(x=0; x<_textureWidth; x++) {
			z0 = complex(xOffset, yOffset);
			z0 +=  complex(x * widthDelta, y * heightDelta);;
			z = z0;
			for (i = 0; i < maxIter; i++) {
				if(z.real() * z.real() + z.imag() * z.imag() >=  4.0) {
					break;
				}
				z = z*z + z0;
			}
                        *(resultsBuffer+(y * _textureWidth) + x) = i;
			z0 += widthDelta;
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
                        byteValue = (unsigned char)(*(resultsBuffer+offset) * (double)maxIterDouble);
			*(imageBuffer  + (offset * 4)) = byteValue;
			*(imageBuffer  + (offset * 4) + 1) = byteValue;
			*(imageBuffer  + (offset * 4) + 2) = byteValue;
                        *(imageBuffer  + (offset * 4) + 3) = 0.0;
                }
//		std::cout << "next ";
	}

/*
	for (int i = 0; i < QImageWriter::supportedImageFormats().size(); ++i) {
		std::cout << QImageWriter::supportedImageFormats().at(i).constData()  << std::endl;
	}
*/

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



