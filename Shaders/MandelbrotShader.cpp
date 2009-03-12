/*
 *  MandelbrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "MandelbrotShader.h"

MandelbrotShader::MandelbrotShader(void) {
	_functionName = std::string("fnMandelbrotShader");
} 

bool MandelbrotShader::initializeShader() {
	
	int index = 0;
	
	std::cout << "MandelbrotShader::initializeShader" << std::endl;
	
	for(int i=0; i< 256; i++) {
		
		gradient[index++] = 1.0 - (i/255.0);
		gradient[index++] = 1.0;
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
	
	return true;

	
}

bool MandelbrotShader::preRenderInitialization(GLuint shaderProgram) {

	// things thats need to be done once just before a render.

	std::cout << "mandelbrot preRenderInitialization" << std::endl;
	glActiveTexture(GL_TEXTURE1);	
	glBindTexture(GL_TEXTURE_2D, gradientTexture);
	
	glUniform1i(glGetUniformLocation(shaderProgram,"gradient_sampler"), 1); // texunit 1

	return true;

} 

bool MandelbrotShader::postRendertidyUp(GLuint shaderProgram) {

	glDeleteTextures(1, &gradientTexture);

	return true;

}

std::string *MandelbrotShader::createShaderCode(bool asFunction, std::string &functionName) {
	
	// start with x -> zx^5 = 1	
	
	std::string *shader = new std::string(
										  " uniform sampler2D gradient_sampler;");
	
	if(asFunction) {
		*shader += "vec4 " + functionName + "(vec2 z, vec4 oldColour)"
		"{"
		"    gl_FragColor = oldColour;" ;
		
	} else {
		*shader += "void main ()"
		"{"
		"    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);" 
		"	vec2 z = gl_TexCoord[0].xy;";
		
	}
	
	*shader +=
	"     z -= offset;"
	"     z *= zoom;"
	"	  vec2 c = z;"
	"	  float maxIter = 1000.0;"
	
	"	for (float i = 0.0; i < maxIter; i += 1.0)"
	"	{"

	"		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;"
	"		if (dot(z, z) > 4.0)"
	"		{";


	
	if(asFunction) {
		*shader += 
		"			return texture2D(gradient_sampler, vec2(i/50.0, 0.0));";	
		
	} else {
		*shader += 
		"			gl_FragColor = texture2D(gradient_sampler, vec2(i/50.0, 0.0));";	
	}	
	
	*shader += 
	"	        break;"
	"       }"	
	"	}";
	
	if(asFunction) {
		*shader += 
		"			return oldColour;";	
		
	}	
	*shader += 
	"}";
	
	return shader;
	
}

bool MandelbrotShader::createShaderCode() {
		
	std::string shader("");
	
	shader +=
	" uniform vec2 zoom;"
	" uniform vec2 offset;";
	
	
	if(_antialias) {
		
		shader += 
		"vec4 " + _shader->functionName() + " (vec2 z, vec4 oldColour);"
		
		"void main() {"
		
		
		"vec2 offset = vec2(0.00025, 0.00025);"
		"vec4 fragColour = " + _shader->functionName() + "(gl_TexCoord[0].xy, vec4(0.0, 0.0, 0.0, 1.0));" 
		"vec2 xy = gl_TexCoord[0].xy;"
		"fragColour +=  " + _shader->functionName() + "(gl_TexCoord[0].xy + offset, vec4(0.0, 0.0, 0.0, 1.0));"
		"fragColour +=  " + _shader->functionName() + "(gl_TexCoord[0].xy + offset, vec4(0.0, 0.0, 0.0, 1.0));"
		"vec2 tmpxy = xy;"
		/* sides */
		"tmpxy.x = xy.x + offset.s;"
		"tmpxy.y = xy.y;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		"tmpxy.x = xy.x - offset.s;"
		"tmpxy.y = xy.y;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		/* top and bottom */
		"tmpxy.x = xy.x;"
		"tmpxy.y = xy.y + offset.t;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		"tmpxy.x = xy.x;"
		"tmpxy.y = xy.y - offset.t;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		/* other corners */
		
		"tmpxy.x = xy.x - offset.s;"
		"tmpxy.y = xy.y + offset.t;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		
		"tmpxy.x = xy.x + offset.s;"
		"tmpxy.y = xy.y - offset.t;"
		"fragColour +=  " + _shader->functionName() + "(tmpxy, vec4(0.0, 0.0, 0.0, 1.0));"
		
		"gl_FragColor = fragColour / vec4(9.0,9.0,9.0,9.0);"
		
		"}";
		
		std::string *function = _shader->createShaderCode(true, _shader->functionName());
		shader += *function;
		delete function;		
		
	} else {
		
		std::string *function = _shader->createShaderCode(false, _shader->functionName());
		shader += *function;
		delete function;		
		
	}
	
	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar *shader_string =  (GLchar *)shader.data();
	glShaderSource(_fragShader, 1, &shader_string, NULL);
	glCompileShader(_fragShader);	
	
	printShaderInfoLog(_fragShader);	
	
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _fragShader);
	glLinkProgram(_shaderProgram);
	glValidateProgram(_shaderProgram);
	
	
	ShaderFactory::printShaderInfoLog(_fragShader);	
	
	ShaderFactory::printProgramInfoLog(_shaderProgram);		
	
	return true;
	
}


std::string &MandelbrotShader::functionName(void) {
	return _functionName;
}

