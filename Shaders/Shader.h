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

#include <QImage>
#include <QHash>
#include <QLayout>
#include <QMouseEvent>

class Shader : public QObject {

        Q_OBJECT

    public:

        Shader(void);
        virtual ~Shader(void);

            /* shader interface */
        virtual bool initializeShader()=0;  // things thats need to be done once
        virtual void render()=0;
        virtual bool postRendertidyUp()=0;  // restore everything to pre-init state
        virtual QLayout *getParameterLayout()=0;  // get layout containing parameter controls
        virtual void mouseReleaseEvent (QMouseEvent *event)=0;


        /* non pure virtual interface functions */
        virtual void setRenderSize(unsigned int width, unsigned int height);
        virtual QImage *getRenderedImage();


        std::string &functionName(void);


    protected:

        std::string _functionName;
        unsigned int _textureWidth;
        unsigned int _textureHeight;
        unsigned int _texture;
        QImage *_image;
        double *_resultsBuffer;
        unsigned char *_imageBuffer;
        QLayout *_parameterLayout;

	
};

#endif
