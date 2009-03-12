/*
 *  MandelbrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "JuliaShader.h"


bool JuliaShader::initializeShader() {
	
	int index = 0;
	
	
	for(int i=0; i< 256; i++) {
		
		gradient[index++] = 1.0 - (i/255.0);
		gradient[index++] = 0.0;
		gradient[index++] = i/255.0;
		//		my_gradient[index++] = 1.0 - (i/255.0);
		gradient[index++] = 1.0;
		
	}
	
	
	std::cout << gradient[4] << ",";
	std::cout << gradient[5] << ",";
	std::cout << gradient[6] << ",";
	std::cout << gradient[7] << std::endl;
	
	glGenTextures(1, &gradientTexture);
	
	glActiveTexture(GL_TEXTURE1);
	
	Shader::createRenderTexture(gradientTexture, 256, 1, gradient);

	createJuliaShader();

	return true;

	
}

bool JuliaShader::preRenderInitialization() {

	// things thats need to be done once just before a render.

	
	glUseProgram(shaderProgram);
	
	glActiveTexture(GL_TEXTURE1);	
	glBindTexture(GL_TEXTURE_2D, gradientTexture);
	
	glUniform1i(glGetUniformLocation(shaderProgram,"gradient_sampler"), 1); // texunit 1
	glUniform2f(glGetUniformLocation(shaderProgram,"zoom"), zoom, zoom); // texunit 1
	glUniform2f(glGetUniformLocation(shaderProgram,"offset"), xOffset, yOffset); // texunit 1

	return true;

} 

bool JuliaShader::postRendertidyUp() {

	
	glDeleteTextures(1, &gradientTexture);
	glDetachShader(shaderProgram, fragShader);
	glDeleteShader(fragShader);
	glDeleteProgram(shaderProgram);
	
	return true;

}

void JuliaShader::createJuliaShader() {
	
	
	
	std::string shader = 
	" uniform sampler2D gradient_sampler;"
	" uniform vec2 zoom;"
	" uniform vec2 offset;"
	//	" uniform vec4 gradient_sampler;"
	"void main ()"
	"{"
//	"   vec2 c = gl_TexCoord[0].xy;"
	"	vec2 z = gl_TexCoord[0].xy;"
	"     z -= offset;"
	"     z *= zoom;"
	"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);" 
	"	for (float i = 0.0; i < 1000.0; i += 1.0)"
	"	{"
	"		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + vec2(0.5, 0.5);"
	"		if (dot(z, z) > 4.0)"
	"		{"
	"			gl_FragColor = texture2D(gradient_sampler, vec2(i/20.0, 0.0)); "
	//"			gl_FragColor = gradient_sampler; "
	//"			gl_FragColor = vec4(1.0, 0.5, 0.5, 1.0); "
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
	
	std::cout << "gradient[0]" <<  gradient[0] << std::endl;
	std::cout << "gradient[1]" <<  gradient[1] << std::endl;
	std::cout << "gradient[2]" <<  gradient[2] << std::endl;
	
	
	Shader::printShaderInfoLog(fragShader);	
	
	Shader::printProgramInfoLog(shaderProgram);	
}