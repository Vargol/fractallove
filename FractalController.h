#ifndef FRACTALCONTROLLER_H
#define FRACTALCONTROLLER_H
/*
 *  FractalController.h
 *  
 *
 *  Created by David Burnett on 14/05/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <QObject>
#include <QList>
#include <QGroupBox>

#include "ShaderFactory.h"

class FractalController : public QObject 
{

	Q_OBJECT	
	
public:	

	FractalController();
	~FractalController();
	
	/* fractal settings */
	double _widthRatio;
	double _heightRatio;


	/* texture settings */
        double _fractalWidth;
        double _fractalHeight;

        double _previewWidth;
        double _previewHeight;

	/* view settings */
	double _viewPortWidth;
	double _viewPortHeight;
	
	void loadTextures();
	void unloadTextures();
        void composeImages(QPainter  &imagePainter);
        void setFractalGroupBox(QGroupBox *gb);
        void mouseReleaseEvent (QMouseEvent *event);

public slots:	
	void setViewPortSize(int width, int height);
        void setFractalWidth(int value);
        void setFractalHeight(int value);
//	void setZoom(double value);
//	void setOffset(double x, double y);
//	void setXOffset(double x);
//	void setYOffset(double y);
	void renderToScreen();
	void renderToTexture();
        void setFractalClass(const QString &fractalClass);
	
signals:
        void redrawFractal(void);
	
private:
        void initializeFractal();
	void setRatios(void) ;
        void addShader(Shader *);

        double _realWidth;
        double _imaginaryHeight;

        QList<Shader *> _fractalList;
        Shader *_selectedShader;
  //      QHash<QString, double> _fractalParameters;
        QGroupBox *_fractalGroupBox;
};

#endif
