/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MANDELBROTSHADER_H
#define MANDELBROTSHADER_H

#include "Shader.h"

class BuddhabrotShader : public Shader {

public:
	/* shader interface */
	BuddhabrotShader();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
	virtual bool preRenderInitialization(GLuint); // things thats need to be done once just before a render.
	virtual bool postRendertidyUp(GLuint);  // restore everything to pre-init state
	virtual std::string &functionName(void);
	virtual void createShaderCode(); 
	virtual void setRenderSize(GLuint, GLuint);
	virtual void render(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right);

private:
	
	void loadTextures();
	void unloadTextures();
	
	std::string *BuddhabrotShader::createMandlebrotShaderCode();		
	std::string *BuddhabrotShader::createBuddhabrotVertexShaderCode();
	std::string *BuddhabrotShader::createBuddhabrotFragShaderCode();
			
	GLuint _attachPoint[3];
	GLuint _textures[3];
	GLuint _glPointsArray;
	GLuint _framebuffer;

	GLuint _buddhaPointTexture;
	GLuint _buddhaNextIter;
	GLuint _buddhaLastIter;

	GLuint _mandelbrot;
	GLuint _buddhabrot;

	GLfloat _xOffset, _yOffset, _zoom;

};

#endif