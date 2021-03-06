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
#include "Shaders/PerlinNoiseShader.h"
#include "Shaders/JuliaOrbitalTrapMP.h"
#include "Shaders/MandelbrotImageTrap.h"
#include "Shaders/MandelbrotOrbitTrap.h"
#include "Shaders/CurlicueFractal.h"
#include "Shaders/ApollonianGasket.h"
#include "Shaders/Mobius.h"

using namespace std;


FractalController::FractalController() {
	
        _fractalWidth = 1000.0;
        _fractalHeight = 1000.0;
        _viewPortHeight = 0;
        _viewPortWidth = 0;

        _selectedShader = NULL;

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
 //   _selectedShader->setRenderParameters(_fractalParameters);
    _selectedShader->initializeShader();

     QTime t;
    t.start();

    _selectedShader->render();

    qDebug("Time elapsed: %d ms", t.elapsed());

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


void FractalController::setFractalClass(const QString &fractalClass)
{

        std::cout << "FractalController::setFractalClass (" << qPrintable(fractalClass) << ")" << std::endl;

        if(fractalClass.compare(QObject::tr("MandelBrot")) == 0) {
            addShader(new MandelbrotShaderMP());
        } else if(fractalClass.compare(QObject::tr("MandelbrotInter")) == 0) {
            addShader(new MandelbrotInter());
        } else if(fractalClass.compare(QObject::tr("Perlin Noise")) == 0) {
            addShader(new PerlinNoiseShader());
        } else if(fractalClass.compare(QObject::tr("Julia Orbital Trap")) == 0) {
            addShader(new JuliaOrbitalTrapMP());
        } else if(fractalClass.compare(QObject::tr("Mandelbrot Orbit Trap")) == 0) {
            addShader(new MandelbrotOrbitTrap());
        } else if(fractalClass.compare(QObject::tr("Mandelbrot Image Trap")) == 0) {
            addShader(new MandelbrotImageTrap());
        } else if(fractalClass.compare(QObject::tr("Curlicue Fractal")) == 0) {
            addShader(new CurlicueFractal());
        } else if(fractalClass.compare(QObject::tr("Mobius Fractal")) == 0) {
            addShader(new Mobius());
        } else if(fractalClass.compare(QObject::tr("Apollonian Gasket")) == 0) {
            addShader(new ApollonianGasket());
        }

        QLayout *old = _fractalGroupBox->layout();
        if(old != NULL) {
            delete(old);
        }

        _fractalGroupBox->update();

        _fractalGroupBox->setLayout(_selectedShader->getParameterLayout());
        _fractalGroupBox->updateGeometry();


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

            if(tmpImage != NULL) {
                imagePainter.drawImage(0,0, *tmpImage);
            }
        while (imageItor.hasNext()) {

            buffer = imageItor.next();
            tmpImage = buffer->getRenderedImage();
            if(tmpImage != NULL) {
                imagePainter.drawImage(0,0, *tmpImage);
            }
        }

    }


}

void FractalController::setFractalGroupBox(QGroupBox *gb) {

    _fractalGroupBox = gb;
}


void FractalController::mouseReleaseEvent (QMouseEvent *event) {
    if(_selectedShader == NULL) {
        return;
    }
    _selectedShader->mouseReleaseEvent (event);
    renderToScreen();
}



