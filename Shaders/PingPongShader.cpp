/*
 *  PingPongShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "PingPongShader.h"

PingPongShader::PingPongShader(void) {
	_functionName = std::string("fnPingPongShader");
} 

bool PingPongShader::initializeShader() {
	
	int index = 0;

	_att_point[0] = GL_COLOR_ATTACHMENT0_EXT;
	_att_point[1] = GL_COLOR_ATTACHMENT1_EXT;

	_read_texture  = 0;
	_write_texture = 1;
	
	std::cout << "PingPongShader::initializeShader" << std::endl;

	glGenTextures(2, _textures);
	
	// create and init the FBO
    glGenFramebuffersEXT(1,&_framebuffer); 
	
	return true;

	
}

bool PingPongShader::preRenderInitialization(GLuint shaderProgram) {

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	
	glDrawBuffer(_att_point[_write_texture]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textures[_write_texture]);	// things thats need to be done once just before a render.
	
	return true;

} 

bool PingPongShader::postRendertidyUp(GLuint shaderProgram) {

	return true;

}

void Shader::drawToScreen(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right) {
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glBindTexture(GL_TEXTURE_2D, _textures[_read_texture]);

	render(top, bottom, left, right);
	
}

void PingPongShader::loadTextures() {
	
	//	QImage t = QFractalController::convertToGLFormat( buffer );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	glActiveTexture(GL_TEXTURE0);
	Shader::checkFBOStatus();
	
	Shader::createRenderTexture(_textures[_read_texture], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _att_point[_read_texture], GL_TEXTURE_2D, tex[_read_texture], 0);
	checkFBOStatus();
	
	Shader::createRenderTexture(_textures[_write_texture], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _att_point[_write_texture], GL_TEXTURE_2D, tex[_write_texture], 0);
	Shader::checkFBOStatus();
	
	std::cout << "glFramebufferTexture2DEXT 1" << gluErrorString(glGetError()) << std::endl;
	
	
}

void PingPongShader::unloadTextures() {
	
	//	QImage t = QFractalController::convertToGLFormat( buffer );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	glActiveTexture(GL_TEXTURE0);
	
	/* detach the texture from the frame buffer */ 
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _att_point[read_tex], GL_TEXTURE_2D, 0, 0);
	std::cout << "PingPongShader::unloadTextures 1" << gluErrorString(glGetError()) << std::endl;
	checkFBOStatus();
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _att_point[write_tex], GL_TEXTURE_2D, 0, 0);
	std::cout << "PingPongShader::unloadTextures 2" << gluErrorString(glGetError()) << std::endl;
	checkFBOStatus();
	
	std::cout << "PingPongShader::unloadTextures 3" << gluErrorString(glGetError()) << std::endl;
	
	std::cout << gluErrorString(glGetError()) << std::endl;
	
	
	
}

