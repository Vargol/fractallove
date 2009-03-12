/*
 *  BuddhaJuliaShaderMP.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BuddhaJuliaShaderMP_H
#define BuddhaJuliaShaderMP_H

#include "Shader.h"

class BuddhaJuliaShaderMP : public Shader {

public:
	/* shader interface */
	BuddhaJuliaShaderMP();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
        virtual bool preRenderInitialization(unsigned int); // things thats need to be done once just before a render.
        virtual bool postRendertidyUp(unsigned int);  // restore everything to pre-init state
	virtual std::string &functionName(void);
	virtual void createShaderCode(); 
        virtual void setRenderSize(unsigned int, unsigned int);
        virtual void render(double top, double bottom, double left, double right);

private:
				
	unsigned int *_buddhaBuffer;
	unsigned char *_imageBuffer;
	bool _redraw;
	
        unsigned int texture;
        double _xOffset, _yOffset, _zoom;
	
	void renderBuddha();
	


	
};

#endif
