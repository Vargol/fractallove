#ifndef APOLLONIANGASKET_H
#define APOLLONIANGASKET_H


#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>


#include <math.h>
#include <iostream>

#include <vector>

#include "Shader.h"

using namespace std;
class ApollonianGasket : public Shader {

    Q_OBJECT

public:

    ApollonianGasket();

private:

    class Circles {

        public:

            vector<double> x;
            vector<double> y;
            vector<double> r;

    };

    vector<Circles *> _circles;
    Circles _inversionCircles;

    double _xOffset, _yOffset, _zoom;

    int _circleCount;
    int _levels;
    int _oversample;

    bool _showInversionCircles;

    QSpinBox *_xOffsetSpinBox;
    QSpinBox *_yOffsetSpinBox;

    virtual bool initializeShader();
    virtual void render();
    virtual bool postRendertidyUp();
    virtual QLayout *getParameterLayout();
    virtual void mouseReleaseEvent (QMouseEvent *event);
    void setRenderSize(unsigned int width, unsigned int height);

    void circleFromThreePoints(Circles &inversionCircles, double x1, double y1, double x2, double y2, double x3, double y3);
    void inversion(double x1, double y1, double x2, double y2, double r2,  double *x, double *y);
    void inversionCircle(double x1, double y1, double r1, double x2, double y2, double r2, double *x, double *y, double *r);

protected slots:
    void setZoom(double value);
    void setXCentre(int);
    void setYCentre(int);

    void setLevels(int);
    void setCircleCount(int);

};

#endif // APOLLONIANGASKET_H
