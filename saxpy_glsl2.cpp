/*
 * License (based on zlib/libpng):
 *
 * Copyright (c) 2005-2007 Dominik Göddeke, University of Dortmund, Germany.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose, 
 * including commercial applications, and to alter it and redistribute it 
 * freely.
 *
 */



/*
 * GPGPU Basic Math Tutorial
 *
 *
 * www.mathematik.uni-dortmund.de/~goeddeke/gpgpu
 *
 * Please drop me a note if you encounter any bugs, or if you have suggestions 
 * on how to improve this tutorial: dominik.goeddeke@math.uni-dortmund.de
 *
 */


// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>


// error codes
#define  ERROR_GLSL        -1
#define  ERROR_GLEW        -2
#define  ERROR_TEXTURE     -3
#define  ERROR_BINDFBO     -4
#define  ERROR_FBOTEXTURE  -5
#define  ERROR_PARAMS      -6

// prototypes
bool checkFramebufferStatus(void);
void checkGLErrors(const char *label);
void compareResults(void);
void createTextures(void);
void createAllTextureParameters(void);
void initGLSL(void);
void initFBO(void);
void initGLEW(void);
void initGLUT(int argc, char** argv);
void performComputation(void);
void printProgramInfoLog(GLuint obj);
void printShaderInfoLog(GLuint obj);
void printVector(const float *p, const int N);
void setupTexture(const GLuint texID);
void swap(void);
void transferFromTexture(float* data);
void transferToTexture(float* data, GLuint texID);

// mode: 0=test (POT), 1=bench (set from command line)
int mode;

// problem size, texture size, number of iterations (set from command line)
int N;
int texSize;
int numIterations;

// flags to fine-tune application and to ease debugging
bool mode_showResults = true;
bool mode_compareResults = true;

// texture identifiers
GLuint yTexID[2];
GLuint xTexID;
GLuint aTexID;

// ping pong management vars
int writeTex = 0;
int readTex = 1;
GLenum attachmentpoints[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };

// GLSL vars
GLuint glslProgram;
GLuint fragmentShader;
GLint yParam, xParam, alphaParam;

// FBO identifier
GLuint fb;

// timing vars 
double start, end;

// handle to offscreen "window", only used to properly shut down the app
GLuint glutWindowHandle;

// struct for variable parts of GL calls (texture format, float format etc)
struct struct_textureParameters {
	char* name;
	GLenum texTarget;
	GLenum texInternalFormat;
	GLenum texFormat;
	char* shader_source;
}	rect_arb_rgba_32,	// texture rectangles, texture_float_ARB, RGBA, 32 bits
	rect_arb_rgba_16,	// texture rectangles, texture_float_ARB, RGBA, 16 bits
	rect_arb_r_32,		// texture rectangles, texture_float_ARB, R, 32 bits
	rect_ati_rgba_32,	// texture rectangles, ATI_texture_float, RGBA, 32 bits
	rect_ati_rgba_16,	// texture rectangles, ATI_texture_float, RGBA, 16 bits
	rect_ati_r_32,		// texture rectangles, ATI_texture_float, R, 32 bits
	rect_nv_rgba_32,	// texture rectangles, NV_float_buffer, RGBA, 32 bits
	rect_nv_rgba_16,	// texture rectangles, NV_float_buffer, RGBA, 16 bits
	rect_nv_r_32,		// texture rectangles, NV_float_buffer, R, 32 bits
	twod_arb_rgba_32,	// texture 2ds, texture_float_ARB, RGBA, 32 bits
	twod_arb_rgba_16,	// texture 2ds, texture_float_ARB, RGBA, 16 bits
	twod_arb_r_32,		// texture 2ds, texture_float_ARB, R, 32 bits
	twod_ati_rgba_32,	// texture 2ds, ATI_texture_float, RGBA, 32 bits
	twod_ati_rgba_16,	// texture 2ds, ATI_texture_float, RGBA, 16 bits
	twod_ati_r_32,		// texture 2ds, NV_float_buffer, R, 32 bits
	twod_nv_rgba_32,	// texture 2ds, NV_float_buffer, RGBA, 32 bits
	twod_nv_rgba_16,	// texture 2ds, NV_float_buffer, RGBA, 16 bits
	twod_nv_r_32;		// texture 2ds, NV_float_buffer, R, 32 bits


// struct actually being used (set from command line)
struct_textureParameters textureParameters;

// actual data
float* dataX;
float* dataY;
float alpha;


/**
 * main, just calls things in the appropriate order
 */
int main(int argc, char **argv) {
    // create variables for GL
    createAllTextureParameters();
    // parse command line
    if (argc < 6) {
	printf("Command line parameters:\n");
	printf("Param 1: 0 = check if given format is supported\n");
	printf("         1 = do some benchmarking \n");
	printf("Param 2: one of the test formats\n");
	printf("         rect_arb_rgba_32,	// texture rectangles, texture_float_ARB, RGBA, 32 bits\n");
	printf("         rect_arb_rgba_16,	// texture rectangles, texture_float_ARB, RGBA, 16 bits\n");
	printf("         rect_arb_r_32,		// texture rectangles, texture_float_ARB, R, 32 bits\n");
	printf("         rect_ati_rgba_32,	// texture rectangles, ATI_texture_float, RGBA, 32 bits\n");
	printf("         rect_ati_rgba_16,	// texture rectangles, ATI_texture_float, RGBA, 16 bits\n");
	printf("         rect_ati_r_32,		// texture rectangles, ATI_texture_float, R, 32 bits\n");
	printf("         rect_nv_rgba_32,	// texture rectangles, NV_float_buffer, RGBA, 32 bits\n");
	printf("         rect_nv_rgba_16,	// texture rectangles, NV_float_buffer, RGBA, 16 bits\n");
	printf("         rect_nv_r_32,		// texture rectangles, NV_float_buffer, R, 32 bits\n");
	printf("         twod_arb_rgba_32,	// texture 2ds, texture_float_ARB, RGBA, 32 bits\n");
	printf("         twod_arb_rgba_16,	// texture 2ds, texture_float_ARB, RGBA, 16 bits\n");
	printf("         twod_arb_r_32,		// texture 2ds, texture_float_ARB, R, 32 bits\n");
	printf("         twod_ati_rgba_32,	// texture 2ds, ATI_texture_float, RGBA, 32 bits\n");
	printf("         twod_ati_rgba_16,	// texture 2ds, ATI_texture_float, RGBA, 16 bits\n");
	printf("         twod_ati_r_32,		// texture 2ds, ATI_texture_float, R, 32 bits\n");
	printf("         twod_nv_rgba_32,	// texture 2ds, NV_float_buffer, RGBA, 32 bits\n");
	printf("         twod_nv_rgba_16,	// texture 2ds, NV_float_buffer, RGBA, 16 bits\n");
	printf("         twod_nv_r_32,		// texture 2ds, NV_float_buffer, R, 32 bits\n");
	printf("Param 3: 0 = no comparison of results\n");
	printf("         1 = compare and only print out max errors\n");
	printf("         2 = compare and print out full result vectors (use with care for large N)\n");
	printf("Param 4: problem size N       \n");
	printf("Param 5: number of iterations \n");
	exit(0);
    } else {
	mode = atoi(argv[1]);
	if (strcmp(argv[2],"rect_arb_rgba_32")==0)
	    textureParameters = rect_arb_rgba_32;
	else if (strcmp(argv[2],"rect_arb_rgba_16")==0)
	    textureParameters = rect_arb_rgba_16;
	else if (strcmp(argv[2],"rect_arb_r_32")==0)
	    textureParameters = rect_arb_r_32;
	else if (strcmp(argv[2],"rect_ati_rgba_32")==0)
	    textureParameters = rect_ati_rgba_32;
	else if (strcmp(argv[2],"rect_ati_rgba_16")==0)
	    textureParameters = rect_ati_rgba_16;
	else if (strcmp(argv[2],"rect_ati_r_32")==0)
	    textureParameters = rect_ati_r_32;
	else if (strcmp(argv[2],"rect_nv_rgba_32")==0)
	    textureParameters = rect_nv_rgba_32;
	else if (strcmp(argv[2],"rect_nv_rgba_16")==0)
	    textureParameters = rect_nv_rgba_16;
	else if (strcmp(argv[2],"rect_nv_r_32")==0)
	    textureParameters = rect_nv_r_32;
	else if (strcmp(argv[2],"twod_arb_rgba_32")==0)
	    textureParameters = twod_arb_rgba_32;
	else if (strcmp(argv[2],"twod_arb_rgba_16")==0)
	    textureParameters = twod_arb_rgba_16;
	else if (strcmp(argv[2],"twod_arb_r_32")==0)
	    textureParameters = twod_arb_r_32;
	else if (strcmp(argv[2],"twod_ati_rgba_32")==0)
	    textureParameters = twod_ati_rgba_32;
	else if (strcmp(argv[2],"twod_ati_rgba_16")==0)
	    textureParameters = twod_ati_rgba_16;
	else if (strcmp(argv[2],"twod_ati_r_32")==0)
	    textureParameters = twod_ati_r_32;
	else if (strcmp(argv[2],"twod_nv_rgba_32")==0)
	    textureParameters = twod_nv_rgba_32;
	else if (strcmp(argv[2],"twod_nv_rgba_16")==0)
	    textureParameters = twod_nv_rgba_16;
	else if (strcmp(argv[2],"twod_nv_r_32")==0)
	    textureParameters = twod_nv_r_32;
	else {
	    printf("unknown parameter, exit\n");
	    exit(ERROR_PARAMS);
	}
	int i =	atoi(argv[3]);
	switch (i) {
	    case 0: 
		mode_showResults = false;
		mode_compareResults = false;
		break;
	    case 1:
		mode_showResults = false;
		mode_compareResults = true;
		break;
	    case 2:
		mode_showResults = true;
		mode_compareResults = true;
		break;
	    default:
		printf("unknown parameter, exit\n");
		exit(ERROR_PARAMS);
	}
	N = atoi (argv[4]);
	numIterations = atoi (argv[5]);
	printf(textureParameters.name);
	printf(", N=%d, numIter=%d\n",N,numIterations);
    }
    
    // calc texture dimensions
    if (textureParameters.texFormat == GL_RGBA)
	texSize = (int)sqrt(N/4.0);
    else
	texSize = (int)sqrt((double)N);
    // create data vectors
    dataX = (float*)malloc(N*sizeof(float));
    dataY = (float*)malloc(N*sizeof(float));
    // and fill with some arbitrary values
    for (int i=0; i<N; i++) {
	dataX[i] = 2.0;
	dataY[i] = i+1.0;
    }
    alpha = 1.0/9.0;
    // init glut and glew
    initGLUT(argc, argv);
    initGLEW();
    // init offscreen framebuffer
    initFBO();
    // create textures for vectors
    createTextures();
    // init shader runtime
    initGLSL();
    // and start computation
    performComputation();
    // compare results
    compareResults();
    // and clean up
    glDetachShader(glslProgram, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(glslProgram);
    glDeleteFramebuffersEXT(1,&fb);
    free(dataX);
    free(dataY);
    glDeleteTextures(2,yTexID);
    glDeleteTextures (1,&xTexID);
    glutDestroyWindow (glutWindowHandle);
    // exit
    return 0;
}


/**
 * Sets up a floating point texture with NEAREST filtering.
 * (mipmaps etc. are unsupported for floating point textures)
 */
void setupTexture (const GLuint texID) {
    // make active and bind
    glBindTexture(textureParameters.texTarget,texID);
    // turn off filtering and wrap modes
    glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // define texture with floating point format
    glTexImage2D(textureParameters.texTarget,0,textureParameters.texInternalFormat,texSize,texSize,0,textureParameters.texFormat,GL_FLOAT,0);
    // check if that worked
    if (glGetError() != GL_NO_ERROR) {
	printf("glTexImage2D():\t\t\t [FAIL]\n");
	exit (ERROR_TEXTURE);
    } else if (mode == 0) {
	printf("glTexImage2D():\t\t\t [PASS]\n");
    }
}

/**
 * Transfers data from currently texture, and stores it in given array.
 */
void transferFromTexture(float* data) {
    // version (a): texture is attached
    // recommended on both NVIDIA and ATI
    glReadBuffer(attachmentpoints[readTex]);
    glReadPixels(0, 0, texSize, texSize,textureParameters.texFormat,GL_FLOAT,data);
    // version b: texture is not neccessarily attached
//	glBindTexture(textureParameters.texTarget,yTexID[readTex]);
//	glGetTexImage(textureParameters.texTarget,0,textureParameters.texFormat,GL_FLOAT,data);
}

/**
 * Transfers data to texture. 
 * Check web page for detailed explanation on the difference between ATI and NVIDIA.
 */
void transferToTexture (float* data, GLuint texID) {
    // version (a): HW-accelerated on NVIDIA 
    glBindTexture(textureParameters.texTarget, texID);
    glTexSubImage2D(textureParameters.texTarget,0,0,0,texSize,texSize,textureParameters.texFormat,GL_FLOAT,data);
    // version (b): HW-accelerated on ATI 
//	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textureParameters.texTarget, texID, 0);
//	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
//	glRasterPos2i(0,0);
//	glDrawPixels(texSize,texSize,textureParameters.texFormat,GL_FLOAT,data);
//	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textureParameters.texTarget, 0, 0);
}

/**
 * creates textures, sets proper viewport etc.
 */
void createTextures (void) {
    // create textures 
    // y gets two textures, alternatingly read-only and write-only, 
    // x is just read-only
    glGenTextures (2, yTexID);
    glGenTextures (1, &xTexID);
    // set up textures
    setupTexture (yTexID[readTex]);
    transferToTexture(dataY,yTexID[readTex]);
    setupTexture (yTexID[writeTex]);
    transferToTexture(dataY,yTexID[writeTex]);
    setupTexture (xTexID);
    transferToTexture(dataX,xTexID);
    // set texenv mode from modulate (the default) to replace
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // check if something went completely wrong
    checkGLErrors ("createFBOandTextures()");
}

/**
 * Sets up GLUT, creates "window" (better put: valid GL context, since the window is never displayed)
 */
void initGLUT(int argc, char **argv) {
    glutInit ( &argc, argv );
    glutWindowHandle = glutCreateWindow("SAXPY TESTS");  
}

/**
 * Sets up GLEW to initialise OpenGL extensions
 */
void initGLEW (void) {
    int err = glewInit();
    // sanity check
    if (GLEW_OK != err) {
	printf((char*)glewGetErrorString(err));
	exit(ERROR_GLEW);
    }  
}

/**
 * Creates framebuffer object, binds it to reroute rendering operations 
 * from the traditional framebuffer to the offscreen buffer
 */
void initFBO(void) {
    // create FBO (off-screen framebuffer)
    glGenFramebuffersEXT(1, &fb); 
    // bind offscreen framebuffer (that is, skip the window-specific render target)
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
    // viewport for 1:1 pixel=texture mapping
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, texSize, 0.0, texSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, texSize, texSize);
}

/**
 * Sets up the GLSL runtime and creates shader.
 */
void initGLSL(void) {
    // create program object
    glslProgram = glCreateProgram();
    // create shader object (fragment shader)
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
    // set source for shader
    const GLchar* source = textureParameters.shader_source;
    glShaderSource(fragmentShader, 1, &source, NULL);
    // compile shader
    glCompileShader(fragmentShader);
    // check for errors
    printShaderInfoLog(fragmentShader);
    // attach shader to program
    glAttachShader (glslProgram, fragmentShader);
    // link into full program, use fixed function vertex pipeline
    glLinkProgram(glslProgram);
    // check for errors
    printProgramInfoLog(glslProgram);
    checkGLErrors("render(2)");
    // Get location of the texture samplers for future use
    yParam = glGetUniformLocation(glslProgram, "textureY");
    xParam = glGetUniformLocation(glslProgram, "textureX");
    alphaParam = glGetUniformLocation(glslProgram, "alpha");
}


/**
 * Performs and times saxpy on the CPU, compares results
 */
void compareResults () {
    // get GPU results
    float* data = new float[N];
    transferFromTexture (data);
    if (mode_compareResults)  {
	// calc on CPU
	start=clock();
	for (int n=0; n<numIterations; n++) 
	    for (int i=0; i<N; i++) 
		dataY[i] = dataY[i] + alpha*dataX[i];
	end = clock();
	double total = (end-start)/CLOCKS_PER_SEC;
	double mflops = (2.0*N*numIterations) / (total * 1000000.0);
	printf("CPU MFLOP/s:\t\t\t%d\n",(int)mflops);
	// and compare results
	double maxError = -1000.0;
	double avgError = 0.0;
	for (int i=0; i<N; i++) {
	    double diff = fabs(data[i]-dataY[i]);
	    if (diff > maxError)
		maxError = diff;
	    avgError += diff;
	}
	avgError /= (double)N;
	printf("Max Error: \t\t\t%e\n",maxError);
	printf("Avg Error: \t\t\t%e\n",avgError);
	if (mode_showResults) {
	    printf("CPU RESULTS:\n");
	    printVector(dataY,N);
	}
    }
    if (mode_showResults) {
	// print out results
	printf("GPU RESULTS:\n");
	printVector (data,N);
    }
    delete [] data;
}


/**
 * Performs the actual calculation.
 */
void performComputation(void) {
    // attach two textures to FBO
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentpoints[writeTex], textureParameters.texTarget, yTexID[writeTex], 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentpoints[readTex], textureParameters.texTarget, yTexID[readTex], 0);
    // check if that worked
    if (!checkFramebufferStatus()) {
	printf("glFramebufferTexture2DEXT():\t [FAIL]\n");
	exit (ERROR_FBOTEXTURE);
    } else if (mode == 0) {
	printf("glFramebufferTexture2DEXT():\t [PASS]\n");
    }
    // enable GLSL program
    glUseProgram(glslProgram);
    // enable texture x (read-only, not changed in the computation loop)
    glActiveTexture(GL_TEXTURE1);	
    glBindTexture(textureParameters.texTarget,xTexID);
    glUniform1i(xParam, 1); // texunit 1
    // enable scalar alpha (same)
    glUniform1f(alphaParam,alpha);
    // Calling glFinish() is only neccessary to get accurate timings,
    // and we need a high number of iterations to avoid timing noise.
    glFinish();

    start = clock();
    for (int i=0; i<numIterations; i++) {
	// set render destination
	glDrawBuffer (attachmentpoints[writeTex]);
	// enable texture y_old (read-only)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(textureParameters.texTarget,yTexID[readTex]);
	glUniform1i(yParam,0); // texunit 0
	// and render multitextured viewport-sized quad
	// depending on the texture target, switch between 
	// normalised ([0,1]^2) and unnormalised ([0,w]x[0,h])
	// texture coordinates
	
	// make quad filled to hit every pixel/texel 
	// (should be default but we never know)
	glPolygonMode(GL_FRONT,GL_FILL);

	// and render the quad
	if (textureParameters.texTarget == GL_TEXTURE_2D) {
	    // render with normalized texcoords
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0); 
	    glVertex2f(0.0, 0.0);
	    glTexCoord2f(1.0, 0.0); 
	    glVertex2f(texSize, 0.0);
	    glTexCoord2f(1.0, 1.0); 
	    glVertex2f(texSize, texSize);
	    glTexCoord2f(0.0, 1.0); 
	    glVertex2f(0.0, texSize);
	    glEnd();
	} else {
	    // render with unnormalized texcoords
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0); 
	    glVertex2f(0.0, 0.0);
	    glTexCoord2f(texSize, 0.0); 
	    glVertex2f(texSize, 0.0);
	    glTexCoord2f(texSize, texSize); 
	    glVertex2f(texSize, texSize);
	    glTexCoord2f(0.0, texSize); 
	    glVertex2f(0.0, texSize);
	    glEnd();
	}
	// swap role of the two textures (read-only source becomes 
	// write-only target and the other way round):
	swap();
    }
    // done, stop timer, calc MFLOP/s if neccessary
    if (mode == 1) {
	glFinish();
	end = clock();
	double total = (end-start)/CLOCKS_PER_SEC;
	// calc mflops 
	double mflops = (2.0*N*numIterations) / (total * 1000000.0);
	printf("GPU MFLOP/s:\t\t\t%d\n",(int)mflops);
    }
    // done, just do some checks if everything went smoothly.
    checkFramebufferStatus();
    checkGLErrors("render()");
}

/**
 * Sets up the various structs used to handle texture targets, texture formats etc.
 */
void createAllTextureParameters(void) {
    rect_arb_rgba_32.name               = "TEXRECT - float_ARB - RGBA - 32";
    rect_arb_rgba_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_arb_rgba_32.texInternalFormat	= GL_RGBA32F_ARB;
    rect_arb_rgba_32.texFormat		= GL_RGBA;
    rect_arb_rgba_32.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_arb_rgba_16.name		= "TEXRECT - float_ARB - RGBA - 16";
    rect_arb_rgba_16.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_arb_rgba_16.texInternalFormat	= GL_RGBA16F_ARB;
    rect_arb_rgba_16.texFormat		= GL_RGBA;
    rect_arb_rgba_16.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_arb_r_32.name			= "TEXRECT - float_ARB - R - 32";
    rect_arb_r_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_arb_r_32.texInternalFormat	= GL_LUMINANCE32F_ARB;
    rect_arb_r_32.texFormat		= GL_LUMINANCE;
    rect_arb_r_32.shader_source		= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2DRect(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2DRect(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
    rect_ati_rgba_32.name		= "TEXRECT - float_ATI - RGBA - 32";
    rect_ati_rgba_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_ati_rgba_32.texInternalFormat	= GL_RGBA_FLOAT32_ATI;
    rect_ati_rgba_32.texFormat		= GL_RGBA;
    rect_ati_rgba_32.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_ati_rgba_16.name		= "TEXRECT - float_ATI - RGBA - 16";
    rect_ati_rgba_16.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_ati_rgba_16.texInternalFormat	= GL_RGBA_FLOAT16_ATI;
    rect_ati_rgba_16.texFormat		= GL_RGBA;
    rect_ati_rgba_16.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_ati_r_32.name			= "TEXRECT - float_ATI - R - 32";
    rect_ati_r_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_ati_r_32.texInternalFormat	= GL_LUMINANCE_FLOAT32_ATI;
    rect_ati_r_32.texFormat		= GL_LUMINANCE;
    rect_ati_r_32.shader_source		= \
 	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2DRect(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2DRect(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
    rect_nv_rgba_32.name		= "TEXRECT - float_NV - RGBA - 32";
    rect_nv_rgba_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_nv_rgba_32.texInternalFormat	= GL_FLOAT_RGBA32_NV;
    rect_nv_rgba_32.texFormat		= GL_RGBA;
    rect_nv_rgba_32.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_nv_rgba_16.name                = "TEXRECT - float_NV - RGBA - 16";
    rect_nv_rgba_16.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_nv_rgba_16.texInternalFormat	= GL_FLOAT_RGBA16_NV;
    rect_nv_rgba_16.texFormat		= GL_RGBA;
    rect_nv_rgba_16.shader_source	= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2DRect(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2DRect(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    rect_nv_r_32.name			= "TEXRECT - float_NV - R - 32";
    rect_nv_r_32.texTarget		= GL_TEXTURE_RECTANGLE_ARB;
    rect_nv_r_32.texInternalFormat	= GL_FLOAT_R32_NV;
    rect_nv_r_32.texFormat		= GL_LUMINANCE;
    rect_nv_r_32.shader_source		= \
	"#extension GL_ARB_texture_rectangle : enable\n" \
	"uniform sampler2DRect textureY;" \
	"uniform sampler2DRect textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2DRect(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2DRect(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
    
/////////////
    
    twod_arb_rgba_32.name		= "tex2D - float_ARB - RGBA - 32";
    twod_arb_rgba_32.texTarget		= GL_TEXTURE_2D;
    twod_arb_rgba_32.texInternalFormat	= GL_RGBA32F_ARB;
    twod_arb_rgba_32.texFormat		= GL_RGBA;
    twod_arb_rgba_32.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_arb_rgba_16.name		= "tex2D - float_ARB - RGBA - 16";
    twod_arb_rgba_16.texTarget		= GL_TEXTURE_2D;
    twod_arb_rgba_16.texInternalFormat	= GL_RGBA16F_ARB;
    twod_arb_rgba_16.texFormat		= GL_RGBA;
    twod_arb_rgba_16.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_arb_r_32.name			= "tex2D - float_ARB - R - 32";
    twod_arb_r_32.texTarget		= GL_TEXTURE_2D;
    twod_arb_r_32.texInternalFormat	= GL_LUMINANCE32F_ARB;
    twod_arb_r_32.texFormat		= GL_LUMINANCE;
    twod_arb_r_32.shader_source		= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2D(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2D(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
    twod_ati_rgba_32.name		= "tex2D - float_ATI - RGBA - 32";
    twod_ati_rgba_32.texTarget		= GL_TEXTURE_2D;
    twod_ati_rgba_32.texInternalFormat	= GL_RGBA_FLOAT32_ATI;
    twod_ati_rgba_32.texFormat		= GL_RGBA;
    twod_ati_rgba_32.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_ati_rgba_16.name		= "tex2D - float_ATI - RGBA - 16";
    twod_ati_rgba_16.texTarget		= GL_TEXTURE_2D;
    twod_ati_rgba_16.texInternalFormat	= GL_RGBA_FLOAT16_ATI;
    twod_ati_rgba_16.texFormat		= GL_RGBA;
    twod_ati_rgba_16.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_ati_r_32.name			= "tex2D - float_ATI - R - 32";
    twod_ati_r_32.texTarget		= GL_TEXTURE_2D;
    twod_ati_r_32.texInternalFormat	= GL_LUMINANCE_FLOAT32_ATI;
    twod_ati_r_32.texFormat		= GL_LUMINANCE;
    twod_ati_r_32.shader_source		= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2D(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2D(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
    twod_nv_rgba_32.name		= "tex2D - float_NV - RGBA - 32";
    twod_nv_rgba_32.texTarget		= GL_TEXTURE_2D;
    twod_nv_rgba_32.texInternalFormat	= GL_FLOAT_RGBA32_NV;
    twod_nv_rgba_32.texFormat		= GL_RGBA;
    twod_nv_rgba_32.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_nv_rgba_16.name		= "tex2D - float_NV - RGBA - 16";
    twod_nv_rgba_16.texTarget		= GL_TEXTURE_2D;
    twod_nv_rgba_16.texInternalFormat	= GL_FLOAT_RGBA16_NV;
    twod_nv_rgba_16.texFormat		= GL_RGBA;
    twod_nv_rgba_16.shader_source	= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  vec4 y = texture2D(textureY, gl_TexCoord[0].st);" \
	"  vec4 x = texture2D(textureX, gl_TexCoord[0].st);" \
	"  gl_FragColor = y + alpha*x;"\
	"}";
    twod_nv_r_32.name			= "tex2D - float_NV - R - 32";
    twod_nv_r_32.texTarget		= GL_TEXTURE_2D;
    twod_nv_r_32.texInternalFormat	= GL_FLOAT_R32_NV;
    twod_nv_r_32.texFormat		= GL_LUMINANCE;
    twod_nv_r_32.shader_source		= \
	"uniform sampler2D textureY;" \
	"uniform sampler2D textureX;" \
	"uniform float alpha;" \
	"void main(void) { " \
	"  float y = texture2D(textureY, gl_TexCoord[0].st).x;" \
	"  float x = texture2D(textureX, gl_TexCoord[0].st).x;" \
	"  gl_FragColor.x = y + alpha*x;"\
	"}";
}

/**
 * Checks for OpenGL errors.
 * Extremely useful debugging function: When developing, 
 * make sure to call this after almost every GL call.
 */
void checkGLErrors (const char *label) {
    GLenum errCode;
    const GLubyte *errStr;
    
    if ((errCode = glGetError()) != GL_NO_ERROR) {
	errStr = gluErrorString(errCode);
	printf("OpenGL ERROR: ");
	printf((char*)errStr);
	printf("(Label: ");
	printf(label);
	printf(")\n.");
    }
}

/**
 * Checks framebuffer status.
 * Copied directly out of the spec, modified to deliver a return value.
 */
bool checkFramebufferStatus() {
    GLenum status;
    status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
	    printf("Framebuffer incomplete, incomplete attachment\n");
            return false;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
	    printf("Unsupported framebuffer format\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
	    printf("Framebuffer incomplete, missing attachment\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
	    printf("Framebuffer incomplete, attached images must have same dimensions\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
	    printf("Framebuffer incomplete, attached images must have same format\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
	    printf("Framebuffer incomplete, missing draw buffer\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
	    printf("Framebuffer incomplete, missing read buffer\n");
            return false;
    }
    return false;
}

/**
 * error checking for GLSL
 */
void printProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
}
void printShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf(infoLog);
        printf("\n");
        free(infoLog);
    }
}


/**
 * Prints out given vector for debugging purposes.
 */
void printVector (const float *p, const int N) {
    for (int i=0; i<N; i++) 
	printf("%f\n",p[i]);
}



/**
 * swaps the role of the two y-textures (read-only and write-only)
 * Can be done in a smarter way :-)
 */
void swap(void) {
    if (writeTex == 0) {
	writeTex = 1;
	readTex = 0;
    } else {
	writeTex = 0;
	readTex = 1;
    }
}




