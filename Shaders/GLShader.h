/*
 *  Shader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <QtOpenGL>


class Shader {
	
    public:

        Shader(void);
        virtual ~Shader(void);

            /* shader interface */
        virtual bool initializeShader()=0;  // things thats need to be done once
        virtual bool postRendertidyUp(GLuint)=0;  // restore everything to pre-init state
        //		virtual std::string *createShaderCode(bool asFunction, std::string &functionName)=0;

        /* non pure virtual interface functions */
        virtual void render(GLfloat top, GLfloat bottom, GLfloat left, GLfloat right);
        virtual void setRenderSize(GLuint width, GLuint height);
        virtual QImage &getRenderedImage();


        std::string &functionName(void);


    protected:

        std::string _functionName;
        GLuint _textureWidth;
        GLuint _textureHeight;
        GLuint _texture;
        QImage *_image;
        unsigned char *_imageBuffer;


	
};

#endif
