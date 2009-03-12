/*
 *  MandelbrotVelocity.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MandelbrotVelocity_H
#define MandelbrotVelocity_H

#include "Shader.h"

class MandelbrotVelocity : public Shader {

public:
	/* shader interface */
        MandelbrotVelocity();
        ~MandelbrotVelocity();
        /* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
        virtual bool preRenderInitialization(unsigned int); // things thats need to be done once just before a render.
        virtual bool postRendertidyUp(unsigned int);  // restore everything to pre-init state
	virtual std::string &functionName(void);
	virtual void createShaderCode(); 
        virtual void setRenderSize(unsigned int, unsigned int);
        virtual void render(double top, double bottom, double left, double right);

private:
				
        double *_fractalBuffer;
	unsigned char *_imageBuffer;
        unsigned int texture;


        double _xOffset, _yOffset, _zoom;

};

#endif
