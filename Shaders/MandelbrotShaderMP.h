/*
 *  MandelbrotShader.h
 *  
 *
 *  Created by David Burnett on 02/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MandelbrotShaderMP_H
#define MandelbrotShaderMP_H

#include <QDoubleSpinBox>

#include "Shader.h"

class MandelbrotShaderMP : public Shader {

    Q_OBJECT

public:
	/* shader interface */
	MandelbrotShaderMP();
	/* shader interface */
	virtual bool initializeShader();  // things thats need to be done once
        virtual bool postRendertidyUp();  // restore everything to pre-init state
	virtual std::string &functionName(void);
        virtual void setRenderSize(unsigned int, unsigned int);
        virtual void render();
        virtual QLayout *getParameterLayout();  // get layout containing parameter controls
        virtual void mouseReleaseEvent (QMouseEvent *event);


protected slots:
        void setZoom(double value);
        void setXCentre(double);
        void setYCentre(double y);
        void setIterations(int i);

protected:
        double _xOffset, _yOffset, _zoom;
        int _iterations;
        QDoubleSpinBox *_xOffsetSpinBox;
        QDoubleSpinBox *_yOffsetSpinBox;


};

#endif
