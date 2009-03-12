/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MANDELBROTBOOSTSHADERMP_H
#define MANDELBROTBOOSTSHADERMP_H

#include "Shader.h"

class MandelbrotBoostShaderMP : public Shader {

public:
	/* shader interface */
	MandelbrotBoostShaderMP();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
	virtual bool preRenderInitialization(GLuint); // things thats need to be done once just before a render.
	virtual bool postRendertidyUp(GLuint);  // restore everything to pre-init state
	virtual std::string &functionName(void);
	virtual void createShaderCode(); 
	virtual void setRenderSize(GLuint, GLuint);
	virtual void render(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right);

private:
				
	unsigned int *_buddhaBuffer;
	unsigned char *_imageBuffer;
	GLuint texture;


	GLfloat _xOffset, _yOffset, _zoom;

};

#endif
