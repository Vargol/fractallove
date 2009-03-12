/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEWTONSHADER_H
#define NEWTONSHADER_H

#include "Shader.h"

class NewtonShader : public Shader {

public:
	
	NewtonShader();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
	virtual bool preRenderInitialization(GLuint); // things thats need to be done once just before a render.
	virtual bool postRendertidyUp(GLuint);  // restore everything to pre-init state
	virtual std::string &functionName(void);
	virtual std::string *createShaderCode(bool asFunction, std::string &functionName); 
	
private:
		
	
	GLuint gradientTexture;

	GLfloat gradient[1024];

};

#endif