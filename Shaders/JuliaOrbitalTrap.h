/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef JULIAORBITALTRAP_H
#define JULIAORBITALTRAP_H

#include "Shader.h"

class JuliaOrbitalTrap : public Shader {

public:
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
	virtual bool preRenderInitialization(); // things thats need to be done once just before a render.
	virtual bool postRendertidyUp();  // restore everything to pre-init state
	
private:
		
	void createJuliaShader();

	GLuint gradientTexture;
	GLuint fragShader;
	GLuint shaderProgram;

	GLfloat gradient[1024];

};

#endif