/*
 *  Shader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "Shader.h"

Shader::Shader(void) {
         _image = NULL;
        _textureWidth = 0;
        _textureHeight = 0;
        _texture = 0;
        _resultsBuffer = NULL;
        _imageBuffer = NULL;
        _functionName = std::string("don't use this name");
} 

Shader::~Shader(void) {
}

std::string &Shader::functionName(void) {
	return _functionName;
}

void Shader::setRenderSize(unsigned int width, unsigned int height) {
	
	_textureWidth = width;
	_textureHeight = height;
	
}

//void Shader::render(double top, double bottom, double left, double right) {
void Shader::render() {

	std::cout << "Shader::Render" << std::endl;
//	std::cout << top << "," << bottom << ","  << left << "," << right << std::endl;

}



 QImage *Shader::getRenderedImage() {
            return _image;
 }

void Shader::setRenderParameters(QHash<QString, double> &parameters) {

   _xOffset = parameters.value("xOffset");
   _yOffset = parameters["yOffset"];
   _zoom = parameters["zoom"];

}
