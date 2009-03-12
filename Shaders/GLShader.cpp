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
	_functionName = std::string("don't use this name");
} 

Shader::~Shader(void) {
	
        delete _image;
} 

std::string &Shader::functionName(void) {
	return _functionName;
}

void Shader::setRenderSize(GLuint width, GLuint height) {
	
	_textureWidth = width;
	_textureHeight = height;
	
}

void Shader::createRenderTexture (GLuint texture, int width, int height)  {
	
	glBindTexture(GL_TEXTURE_2D, texture);

	std::cout << "createRenderTexture " << texture << ": " << gluErrorString(glGetError()) << std::endl;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	std::cout << "createRenderTexture " << texture << ": " << gluErrorString(glGetError()) << std::endl;
	
	
}

void Shader::createRenderTexture (GLuint texture, int width, int height, void *buffer)  {
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, buffer);
	
	
}


void Shader::createRenderTexture (GLuint texture, int width, int height, unsigned char *buffer)  {
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	
}

void Shader::render(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right) {
	
	std::cout << "Shader::Render" << std::endl;
	std::cout << top << "," << bottom << ","  << left << "," << right << std::endl;

        render(top, bottom, left, right);


	
}

void Shader::drawToScreen(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right) {
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        glPolygonMode(GL_FRONT,GL_FILL);
	
	glBegin(GL_QUADS); 
	//	glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
	glTexCoord2f(0.0, 0.0); 
	glVertex2f(left, bottom); 
	
	//	glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Red
	glTexCoord2f(1.0, 0.0); 
	glVertex2f(right, bottom); 
	
	//	glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Red
	glTexCoord2f(1.0, 1.0); 
	glVertex2f(right, top); 
	
	//	glColor3f(0.0f,0.0f,0.0f);			// Set The Color To Red
	glTexCoord2f(0.0, 1.0); 
	glVertex2f(left, top); 
	glEnd(); 

}



void Shader::printProgramInfoLog(GLuint obj) {
	
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cout << "printProgramInfoLog: " << infoLog << std::endl;
		free(infoLog);
	}else{
	 	std::cout << "Program Info Log: OK" << std::endl;
	}
}

void Shader::printShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
	
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cout << "printShaderInfoLog: " << infoLog << std::endl;
        free(infoLog);
    }else{
	 	std::cout << "Shader Info Log: OK" << std::endl;
	}
}

void Shader::checkFBOStatus () {
	
	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
		std::cout << "FBO error " << glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) <<  std::endl;
		switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) 
		{	
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				printf("Error!  missing a required image/buffer attachment!\n");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				printf("Error!  has no images/buffers attached!\n");
				break;
				//			case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
				//				printf("Error!  has an image/buffer attached in multiple locations!\n");
				//				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				printf("Error!   has mismatched image/buffer dimensions!\n");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				printf("Error!   colorbuffer attachments have different types!\n");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				printf("Error!  trying to draw to non-attached color buffer!\n");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				printf("Error!   trying to read from a non-attached color buffer!\n");
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				printf("Error!   format is not supported by current graphics card/driver!\n");
				break;
				//			case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
				//				printf("Error!  Non-framebuffer passed to glCheckFramebufferStatusEXT()!\n");
				//				break;
			default:
				printf("*UNKNOWN ERROR* reported from glCheckFramebufferStatusEXT() !\n");
				break;
		}	
	}
	
}

