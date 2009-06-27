#ifndef CURLICUEFRACTAL_H
#define CURLICUEFRACTAL_H

#include <QDoubleSpinBox>

#include "Shader.h"


class CurlicueFractal : public Shader
{

    Q_OBJECT

public:

    CurlicueFractal();

    virtual bool initializeShader();
    virtual void render();
    virtual bool postRendertidyUp();
    virtual QLayout *getParameterLayout();
    virtual void mouseReleaseEvent (QMouseEvent *event);

protected slots:
    void setZoom(double value);
    void setXCentre(int);
    void setYCentre(int);
    void setIterations(int i);

protected:
    double _xOffset, _yOffset, _zoom;
    int _iterations;
    double _irrational;

    QSpinBox *_xOffsetSpinBox;
    QSpinBox *_yOffsetSpinBox;


};

#endif // CURLICUEFRACTAL_H
