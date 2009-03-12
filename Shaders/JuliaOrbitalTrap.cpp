/*
 *  MandelbrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <QImage>
#include <QGLWidget>

#include "JuliaOrbitalTrap.h"


bool JuliaOrbitalTrap::initializeShader() {
	
	QImage file = QImage("orbital.png");
	QImage trap = QGLWidget::convertToGLFormat(file);
	
	std::cout << "file:" << file.depth();
	std::cout << "trap:" << trap.depth();
	
	glGenTextures(1, &gradientTexture);
	
	glActiveTexture(GL_TEXTURE1);
	
	Shader::createRenderTexture(gradientTexture, trap.width(), trap.height(), trap.bits());

	createJuliaShader();

	return true;

	
}

bool JuliaOrbitalTrap::preRenderInitialization() {

	// things thats need to be done once just before a render.

	
	glUseProgram(shaderProgram);
	
	glActiveTexture(GL_TEXTURE1);	
	glBindTexture(GL_TEXTURE_2D, gradientTexture);
	
	glUniform1i(glGetUniformLocation(shaderProgram,"trap"), 1); // texunit 1
	glUniform2f(glGetUniformLocation(shaderProgram,"zoom"), zoom, zoom); // texunit 1
	glUniform2f(glGetUniformLocation(shaderProgram,"offset"), xOffset, yOffset); // texunit 1

	return true;

} 

bool JuliaOrbitalTrap::postRendertidyUp() {

	
	glDeleteTextures(1, &gradientTexture);
	glDetachShader(shaderProgram, fragShader);
	glDeleteShader(fragShader);
	glDeleteProgram(shaderProgram);
	
	return true;

}

void JuliaOrbitalTrap::createJuliaShader() {
	
	
	
	std::string shader = 
	" uniform sampler2D trap;"
	" uniform vec2 zoom;"
	" uniform vec2 offset;"
	"void main ()"
	"{"
	"	vec2 z = gl_TexCoord[0].xy;"
	"     z -= offset;"
	"     z *= zoom;"
	"	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);" 
	"	for (float i = 0.0; i < 1000.0; i += 1.0)"
	"	{"
	"		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + vec2(0.6, 0.7);"
//	"		if (all(greaterThanEqual(z, vec2(-0.5, -0.5))) && all(lessThanEqual(z, vec2(0.5, 0.5))))"
	"		if (dot(z, z) < 0.5)"
	"		{"
	"			gl_FragColor = texture2D(trap, z + vec2(0.5, 0.5)); "
	"			break;"
	"		}"
	"	}"
	"}";
	
	
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar *shader_string =  (GLchar *)shader.data();
	glShaderSource(fragShader, 1, &shader_string, NULL);
	glCompileShader(fragShader);	
	
	printShaderInfoLog(fragShader);	
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	
	Shader::printShaderInfoLog(fragShader);	
	
	Shader::printProgramInfoLog(shaderProgram);	
}