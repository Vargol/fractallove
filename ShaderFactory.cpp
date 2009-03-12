/*
 *  ShaderFractory.cpp
 *  
 *
 *  Created by David Burnett on 12/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include <QString>


#include "ShaderFactory.h"
#include "Shaders/MandelbrotShaderMP.h"
//#include "Shaders/JuliaShader.h"
//#include "Shaders/JuliaOrbitalTrap.h"
//#include "Shaders/NewtonShader.h"
#include "Shaders/BuddhabrotShaderMP.h"
//#include "Shaders/MandelbrotBoostShaderMP.h"
#include "Shaders/BuddhaJuliaShaderMP.h"
#include "Shaders/MandelbrotAvgShader.h"
#include "Shaders/MandelbrotVelocity.h"

ShaderFactory::ShaderFactory() {
	
        this->_shader = NULL;
	this->_antialias = false;
}

void ShaderFactory::setShader(const QString &shader) {
		
	if(this->_shader != NULL) {
		free(this->_shader);
	}

        if(shader.compare(QObject::tr("MandleBrot")) == 0) {
            this->_shader = new MandelbrotShaderMP();
        } /*else if(shader.compare(QObject::tr("MandleBrot Velocity")) == 0) {
            this->_shader = new MandelbrotVelocity();
        } else if(shader.compare(QObject::tr("Buddhabrot Julia")) == 0) {
            this->_shader = new BuddhaJuliaShaderMP();
        } else if(shader.compare(QObject::tr("Buddhabrot")) == 0) {
            this->_shader = new BuddhabrotShaderMP();
        }*/

}

void ShaderFactory::setAntiAliasing(bool antialias) {
	
	this->_antialias = antialias;
	
}


void ShaderFactory::setZoom(double zoom) {
	
	this->_zoom = zoom;
	
}

void ShaderFactory::setOffset(double x, double y) {
	
	this->_xOffset = x;
	this->_yOffset = y;
	
}

bool ShaderFactory::buildShader() {
	
//	_shader->createShaderCode();
	
    	
	return true;
	
}

std::string *ShaderFactory::ShaderFactory::getErrorMessage() {

	return new std::string("Fix me");
	
}

bool ShaderFactory::initializeShader() {
	
	return _shader->initializeShader();
}
bool ShaderFactory::preRenderInitialization() {
	
//	return _shader->preRenderInitialization(_shaderProgram);
        return true;
} 

void ShaderFactory::drawToScreen(double top, double bottom, double left, double right) {

//	_shader->drawToScreen(top, bottom, left, right);

}


void ShaderFactory::render(double top, double bottom, double left, double right) {
	
	
	return _shader->render(top, bottom, left, right);
	
} 

bool ShaderFactory::postRendertidyUp() {
	
        _shader->postRendertidyUp();
	
	return true;
	
}


void ShaderFactory::setRenderSize(unsigned int width, unsigned int height) {
	_shader->setRenderSize(width, height);
}
