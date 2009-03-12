/*
 *  MandelbrotShader.cpp
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "NewtonShader.h"

NewtonShader::NewtonShader(void) {
	_functionName = std::string("fnNewtonShader");
} 

bool NewtonShader::initializeShader() {
	
	int index = 0;
	
	
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

bool NewtonShader::preRenderInitialization(GLuint shaderProgram) {

	// things thats need to be done once just before a render.
	
	glActiveTexture(GL_TEXTURE1);	
	glBindTexture(GL_TEXTURE_2D, gradientTexture);
	
	glUniform1i(glGetUniformLocation(shaderProgram,"gradient_sampler"), 1); // texunit 1

	return true;

} 

bool NewtonShader::postRendertidyUp(GLuint shaderProgram) {

	
	glDeleteTextures(1, &gradientTexture);
	
	return true;

}



std::string *NewtonShader::createShaderCode(bool asFunction, std::string &functionName) {
	
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
	"	  float maxIter = 30.0;"
	
	"	for (float i = 0.0; i < maxIter; i += 1.0)"
	"	{"
	
	"	    vec2 z4 = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y);"   // z^2
	"	    z4 = vec2(z4.x*z4.x - z4.y*z4.y, 2.0*z4.x*z4.y);"   // z^4
	"	    vec2 zf = vec2(z4.x*z.x - z4.y*z.y, z4.x*z.y + z4.y*z.x) - vec2(1.0, 0.0);"
	
	"   	if(zf.x * zf.x + zf.y * zf.y < 0.01) {";
	
	if(asFunction) {
		*shader += 
		"			return texture2D(gradient_sampler, vec2(i/maxIter, 0.0));";	
		
	} else {
		*shader += 
		"			gl_FragColor = texture2D(gradient_sampler, vec2(i/maxIter, 0.0));";	
	}	
	
	*shader += 
	"	        break;"
	"       }"	
	
	"		vec2 zfp = z4 * vec2(5.0, 5.0);"
	
	"       float bottomTerm = zfp.x * zfp.x + zfp.y * zfp.y;"
	/* division */
	"       vec2 zn;"
	"       zn.x = (zf.x * zfp.x + zf.y * zfp.y) / bottomTerm;"
	"       zn.y = (zf.y * zfp.x - zf.x * zfp.y) / bottomTerm;"	
	"       z = z - zn;"
	
	"	}";
	
	if(asFunction) {
		*shader += 
		"			return oldColour;";	
		
	}	
	*shader += 
	"}";
	
	return shader;

}

std::string &NewtonShader::functionName(void) {
	return _functionName;
}