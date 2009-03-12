/*
 *  ShaderFractory.h
 *  
 *
 *  Created by David Burnett on 12/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "Shaders/Shader.h"

class ShaderFactory  {

public:	
	
	ShaderFactory();
        void setShader(const QString &);
	void setAntiAliasing(bool);
	bool buildShader();
	std::string *getErrorMessage();	

	bool initializeShader();  // things thats need to be done once
	bool preRenderInitialization(); // things thats need to be done once just before a render.
	bool postRendertidyUp();  // restore everything to pre-init state
	
        void setZoom(double);
        void setOffset(double, double);
	
        void render(double top, double bottom, double left, double right);
        void drawToScreen(double top, double bottom, double left, double right);
        void setRenderSize(unsigned int width, unsigned int height);

public slots:	
	
	
private:
 	
	Shader *_shader;
	
        unsigned int _fragShader;
        unsigned int _shaderProgram;

	bool _antialias;

        double _xOffset, _yOffset, _zoom;

	
};

#endif
