/*
 *  BuddhabrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


/*
 
rendering buddabrot ....

Render mandlebrot, use alpha for inside outside

texture -> VBO GL_POINTS;
vertex shader does one iteration
frag shader write to two textures, one accumulates the other stores new co-ordinates
texture 2 -> VBO GL_POINTS 
 
 for bitmap
 
 
 
 
*/ 
#include <iostream>

#include "BuddhabrotShader.h"

BuddhabrotShader::BuddhabrotShader(void) {
	_functionName = std::string("fnBuddabrotShader");
} 

bool BuddhabrotShader::initializeShader() {
	
	std::cout << "BuddhabrotShader::initializeShader" << std::endl;
	
	/* create PBO's **/
	
	glGenBuffers(1, &_glPointsArray);

	/* create FBO's **/

	glGenTextures(3, _textures);
	
	// create and init the FBO
    glGenFramebuffersEXT(1,&_framebuffer); 
	
	_attachPoint[0] = GL_COLOR_ATTACHMENT0_EXT;
	_attachPoint[1] = GL_COLOR_ATTACHMENT2_EXT;
	_attachPoint[2] = GL_COLOR_ATTACHMENT3_EXT;

	_buddhaPointTexture = 0;
	_buddhaNextIter    = 1;
	_buddhaLastIter    = 2;

	_zoom = 2.0;
	_xOffset = 0.5;
	_yOffset = 0.5;	
	
	return true;

	
}

void BuddhabrotShader::setRenderSize(GLuint width, GLuint height) {

	std::cout << "BuddhabrotShader::setRenderSize" << std::endl;

	if(_textureWidth != width || _textureHeight != height) {

		unloadTextures();
		
	}

	_textureWidth = width;
	_textureHeight = height;

	loadTextures();
	
}

bool BuddhabrotShader::preRenderInitialization(GLuint shaderProgram) {

	// things thats need to be done once just before a render.

	std::cout << "BuddhabrotShader::preRenderInitialization" << std::endl;

	glUseProgram(_mandelbrot);
	// things thats need to be done once just before a render.
	glUniform2f(glGetUniformLocation(_mandelbrot,"zoom"), _zoom, _zoom); // texunit 1
	glUniform2f(glGetUniformLocation(_mandelbrot,"offset"), _xOffset, _yOffset); // texunit 1
	
	glUseProgram(_buddhabrot);
	// things thats need to be done once just before a render.
	glUniform4f(glGetUniformLocation(_mandelbrot,"zoom"), _zoom, _zoom, 1.0, 1.0); // texunit 1
	glUniform4f(glGetUniformLocation(_buddhabrot,"offset"), _xOffset, _yOffset, 0.0, 0.0); // texunit 1
	
	return true;

} 

bool BuddhabrotShader::postRendertidyUp(GLuint shaderProgram) {

	return true;

}

void BuddhabrotShader::createShaderCode() {

	std::string *shader = createMandlebrotShaderCode();
	
	GLuint mandelFragShader = glCreateShader(GL_FRAGMENT_SHADER);
		
	const GLchar *mfCode =  (GLchar *)shader->data();
	glShaderSource(mandelFragShader, 1, &mfCode , NULL);
	glCompileShader(mandelFragShader);	
	
	printShaderInfoLog(mandelFragShader);	
	
	_mandelbrot = glCreateProgram();
	glAttachShader(_mandelbrot, mandelFragShader);
	glLinkProgram(_mandelbrot);
	glValidateProgram(_mandelbrot);
		
	Shader::printProgramInfoLog(_mandelbrot);		
	
	_buddhabrot = glCreateProgram();

	GLuint buddhaFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint buddhalVertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	delete(shader);
	
	shader = createBuddhabrotFragShaderCode();

	const GLchar *bfCode = (GLchar*)shader->data();

	glShaderSource(buddhaFragShader, 1, &bfCode , NULL);
	glCompileShader(buddhaFragShader);	

	Shader::printShaderInfoLog(buddhaFragShader);	
	
	glAttachShader(_mandelbrot, buddhaFragShader);

	delete(shader);
	
	shader = createBuddhabrotVertexShaderCode();
	
	const GLchar *bvCode = (GLchar*)shader->data();
	
	glShaderSource(buddhalVertexShader, 1, &bvCode , NULL);
	glCompileShader(buddhalVertexShader);		
	Shader::printShaderInfoLog(buddhalVertexShader);	
	
	glAttachShader(_buddhabrot, buddhalVertexShader);
	glLinkProgram(_buddhabrot);
	glValidateProgram(_buddhabrot);
	Shader::printProgramInfoLog(_buddhabrot);		

	delete(shader);
	
	return;
	
}


std::string *BuddhabrotShader::createMandlebrotShaderCode() {
	
	// start with x -> zx^5 = 1	
	std::cout << "createMandlebrotShaderCode" << std::endl;
	
	std::string *shader = new std::string("");
	
	*shader +=
	" uniform vec2 zoom;"
	" uniform vec2 offset;";
	
	
	*shader += "void main ()"
	"{"
	"    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);" 
	"	vec2 z = gl_TexCoord[0].xy;"
	"     z -= offset;"
	"     z *= zoom;"
	"	  vec2 c = z;"
	"	  float maxIter = 1000.0;"
	
	"	for (float i = 0.0; i < maxIter; i += 1.0)"
	"	{"
	
	"		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;"
	"		if (dot(z, z) > 4.0)"
	"		{"
	"			gl_FragColor = gl_TexCoord[0];"
	"			gl_FragColor.a = 1.0;"
	"	        break;"
	"       }"	
	"	}"
	"	gl_FragColor = vec4(-1.0, -1.0, -1.0, 0.0);"
	"}";

	
	return shader;
	
}


std::string *BuddhabrotShader::createBuddhabrotVertexShaderCode() {
	
	// start with x -> zx^5 = 1	
	std::cout << "createBuddhabrotVertexShaderCode" << std::endl;
	
	std::string *vectorShader = new std::string("uniform vec4 zoom;");
	
	*vectorShader +=
	"uniform vec4 offset;";
	
	*vectorShader += "void main ()"
	"{"
	"	vec4 z = gl_MultiTexCoord0.xyzw;"
	"   if(z.w == 0.0) {"
	"       gl_Position = z; "
	"       return;"
	"   }"
	
	"   z -= offset;"
	"   z *= zoom;"
	"	z = vec4(z.x*z.x - z.y*z.y, 2.0*z.x*z.y, z.zw);"
	"	gl_Position = z;"
	"}";

	return vectorShader;
	
}

std::string *BuddhabrotShader::createBuddhabrotFragShaderCode() {
	
	// start with x -> zx^5 = 1	
	std::cout << "createBuddhabrotFragShaderCode" << std::endl;
	
	std::string *fragShader = new std::string("uniform sampler2D lastIteration;");
	
	*fragShader += "void main ()"
	"{"
	"	vec4 z = gl_TexCoord[0];"
	"   if(z.w == 0.0) {"
/* value is not plotted */	
	
	"       gl_FragData[0] = vec4(0.0, 0.0, 0.0, 0.0); "
	"       return;"
	"   }"

/* tex[0] gets new co-ordinates, tex[1] accumulates */	
	"gl_FragData[0] = z;"
	"gl_FragData[1] = texture2D(lastIteration, z.xy);"
	"gl_FragData[1].a += 1.0;"
	
	"}";
	
	return fragShader;
	
}

void BuddhabrotShader::render(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right) {

	std::cout << "BuddhabrotShader::render" << std::endl;
	
	
	GLfloat textureWidth = right - left;
	GLfloat textureHeight = top - bottom;

	std::cout << "BuddhabrotShader::render: " << textureWidth << "," << textureHeight << std::endl;
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	glActiveTexture(GL_TEXTURE0);	
	glDrawBuffer(_attachPoint[_buddhaPointTexture]);
	
	// viewport for 1:1 pixel=texture mapping
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, textureWidth, 0.0, textureHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, textureWidth, textureHeight);
	
	glUseProgram(_mandelbrot);
	
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

	glUseProgram(_buddhabrot);

	GLuint buffers[2];
	
	for(int i=0; i<1000; i++) {
			
		/* copy texture on GL_POINTS ARRAY */
		glReadBuffer(_attachPoint[_buddhaPointTexture]);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _glPointsArray);
		glReadPixels(0, 0, textureWidth, textureHeight, GL_RGBA, GL_FLOAT, NULL);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _glPointsArray); 

		std::cout << gluErrorString(glGetError()) << std::endl;
		
		glEnableClientState(GL_VERTEX_ARRAY); 
		glVertexPointer  ( 4, GL_FLOAT,4*sizeof(GLfloat), 0); 
		
		/* bind the texture FBO's*/
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);	
		
		buffers[0] = _attachPoint[_buddhaPointTexture];
		buffers[1] = _attachPoint[_buddhaNextIter];

		glDrawBuffers(2, buffers);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textures[_buddhaLastIter]);
		glUniform1i(glGetUniformLocation(_buddhabrot ,"lastIteration"), 0); // texunit 1	

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		/* draw as vertex points */
		glDrawArrays(GL_POINTS, 0, textureWidth*textureHeight);

		glDisableClientState(GL_VERTEX_ARRAY);
		
		std::swap(_buddhaLastIter, _buddhaNextIter);

	}
	
	std::cout << gluErrorString(glGetError()) << std::endl;

	/* draw texture to screen */
	glUseProgram(0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, _textures[_buddhaLastIter]);
	
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

void BuddhabrotShader::loadTextures() {

	std::cout << "BuddhabrotShader::loadTextures" << std::endl;
	std::cout << "BuddhabrotShader::loadTextures: " << _textureWidth << "," << _textureHeight << std::endl;


	//	QImage t = QFractalController::convertToGLFormat( buffer );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	glActiveTexture(GL_TEXTURE0);
	checkFBOStatus();
	
	Shader::createRenderTexture(_textures[0], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[0], GL_TEXTURE_2D, _textures[0], 0);
	checkFBOStatus();
	
	Shader::createRenderTexture(_textures[1], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[1], GL_TEXTURE_2D, _textures[1], 0);
	checkFBOStatus();
	
	Shader::createRenderTexture(_textures[2], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[2], GL_TEXTURE_2D, _textures[2], 0);
	checkFBOStatus();
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _glPointsArray);
	glBufferData(GL_PIXEL_PACK_BUFFER, _textureWidth*_textureHeight*4*sizeof(GLfloat), NULL, GL_DYNAMIC_COPY );
	
	
	
}

void BuddhabrotShader::unloadTextures() {

	std::cout << "unloadTextures" << std::endl;

	//	QImage t = QFractalController::convertToGLFormat( buffer );
	//	QImage t = QFractalController::convertToGLFormat( buffer );
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _framebuffer);
	glActiveTexture(GL_TEXTURE0);
	
	Shader::createRenderTexture(_textures[0], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[0], GL_TEXTURE_2D, 0, 0);
	
	Shader::createRenderTexture(_textures[1], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[1], GL_TEXTURE_2D, 0, 0);
	
	Shader::createRenderTexture(_textures[2], _textureWidth, _textureHeight);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, _attachPoint[2], GL_TEXTURE_2D, 0, 0);
	
	
	
}


std::string &BuddhabrotShader::functionName(void) {
	return _functionName;
}

