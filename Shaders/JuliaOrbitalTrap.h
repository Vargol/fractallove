/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef JULIAORBITALTRAP_H
#define JULIAORBITALTRAP_H

#include "OpenGLShader.h"

class JuliaOrbitalTrap : public OpenGLShader {
   Q_OBJECT
public:
        JuliaOrbitalTrap();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
	virtual bool preRenderInitialization(); // things thats need to be done once just before a render.
	virtual bool postRendertidyUp();  // restore everything to pre-init state
        QLayout *getParameterLayout(void);

        virtual void setRenderSize(unsigned int width, unsigned int height);
        virtual void render(void);

private:
		
	void createJuliaShader();

        GLuint _trapTexture;
        GLuint _renderToTexture;
        GLuint _fragShader;
        GLuint _shaderProgram;
        GLuint _fbo;

        double _zoom, _xOffset, _yOffset;

protected slots:

        void setZoom(double value);
        void setXCentre(double);
        void setYCentre(double y);

};

#endif
