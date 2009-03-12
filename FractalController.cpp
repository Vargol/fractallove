/*
 *  FractalController.cpp
 *  
 *
 *  Created by David Burnett on 14/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#include <iostream>


#include <QtGui>
#include <QtOpenGL>
#include <math.h>



#include "FractalController.h"
#include "Shaders/MandelbrotShaderMP.h"
#include "Shaders/MandelbrotInter.h"

using namespace std;


FractalController::FractalController() {
	
        _fractalWidth = 1000.0;
        _fractalHeight = 1000.0;
        _viewPortHeight = 0;
        _viewPortWidth = 0;

        _selectedShader = NULL;

        _fractalParameters["xOffset"] = 0.0;
        _fractalParameters["yOffset"] = 0.0;
        _fractalParameters["zoom"] = 1.0;

}


FractalController::~FractalController() {
	
}

void FractalController::initializeFractal() {

	std::cout << " FractalController::initializeFactory" << std::endl;
		
	
}


void FractalController::renderToTexture() {
	
    std::cout << "renderToTexture: " << _fractalWidth << "," << _fractalHeight << std::endl;



}


void FractalController::renderToScreen() {

    if(_selectedShader == NULL) {
      return;
    }
    setRatios();
    _selectedShader->setRenderSize(_previewWidth, _previewHeight);
    _selectedShader->setRenderParameters(_fractalParameters);
    _selectedShader->initializeShader();
    _selectedShader->render();

    emit redrawFractal();
}



void FractalController::setRatios(void) {

	std::cout << "FractalController::setRatios" << std::endl;
	 
        if(_fractalWidth < _fractalHeight) {
		_heightRatio = 1.0;
                _widthRatio = (_fractalWidth / _fractalHeight);
		
	} else {
		_widthRatio = 1.0;
                _heightRatio = (_fractalHeight / _fractalWidth);
	}

        std::cout << _fractalWidth << "," << _fractalHeight << std::endl;
	std::cout << _widthRatio << "," << _heightRatio << std::endl;

        _previewWidth = _viewPortWidth * _widthRatio;
        _previewHeight = _viewPortHeight * _heightRatio;

}

/* slots */
void FractalController::setViewPortSize(int width, int height) {
	
	std::cout << "FractalController::setViewPortSize" << std::endl;
	
	if(width == _viewPortWidth && height == _viewPortHeight) {
		return;
	}
	
	_viewPortWidth = width;
	_viewPortHeight = height;

	std::cout << _viewPortWidth << ","  << _viewPortHeight << std::endl;

    setRatios();
	
	
}

void FractalController::setFractalWidth(int value) {
	
        if(value == _fractalWidth) {
		return;
	}
	
        _fractalWidth = value;
    setRatios();
}

void FractalController::setFractalHeight(int value)  {
	
    if(value == _fractalHeight) {
        return;
    }
	
    _fractalHeight = value;
    setRatios();
}

void FractalController::setZoom(double value) {

	if(value == _zoom) {
		return;
	}
	
	_zoom = value;
        _fractalParameters["zoom"] = value;

    setRatios();
	
}


void FractalController::setOffset(double x, double y) {
	
	if(x == _xOffset && y == _yOffset) {
		return;
	}
	
	_xOffset = x;
	_yOffset = y;
	
}

void FractalController::setXOffset(double x) {
	
	if(x == _xOffset) {
		return;
	}
	
	_xOffset = x;

        _fractalParameters["xOffset"] = x;
	
}

void FractalController::setYOffset(double y) {
	
	if(y == _yOffset) {
		return;
	}
	
        _yOffset = -y;
        _fractalParameters["yOffset"] = -y;

}

void FractalController::setFractalClass(const QString &fractalClass)
{

        std::cout << "FractalController::setFractalClass (" << qPrintable(fractalClass) << ")" << std::endl;

        if(fractalClass.compare(QObject::tr("MandelBrot")) == 0) {
            addShader(new MandelbrotShaderMP());
        } else if(fractalClass.compare(QObject::tr("MandelbrotInter")) == 0) {
            addShader(new MandelbrotInter());
        }

;
}

void FractalController::addShader(Shader *shader) {

    _fractalList.append(shader);
    _selectedShader = shader;

}

void FractalController::composeImages(QPainter  &imagePainter) {

    if(_fractalList.count() > 0) {

        QListIterator<Shader *> imageItor(_fractalList);

        Shader *buffer = imageItor.next();
        QImage *tmpImage = buffer->getRenderedImage();

        imagePainter.drawImage(0,0, *tmpImage);

        while (imageItor.hasNext()) {

            buffer = imageItor.next();
            tmpImage = buffer->getRenderedImage();

            imagePainter.drawImage(0,0, *tmpImage);
        }

    }


}
