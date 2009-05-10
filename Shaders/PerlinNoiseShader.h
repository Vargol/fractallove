#ifndef PERLINNOISESHADER_H
#define PERLINNOISESHADER_H


#include <QDoubleSpinBox>
#include <QImageWriter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

#include "Shader.h"

class PerlinNoiseShader : public Shader {

    Q_OBJECT

public:

    PerlinNoiseShader();

    virtual bool initializeShader();  // things thats need to be done once
    virtual bool postRendertidyUp();  // restore everything to pre-init state
    virtual std::string &functionName(void);
    virtual void setRenderSize(unsigned int, unsigned int);
    virtual void render();
    virtual QLayout *getParameterLayout();  // get layout containing parameter controls
    virtual void mouseReleaseEvent (QMouseEvent *event);


private:

    const static int PERLIN_SIZE = 256;
    const static int PERLIN_MASK = 255;

    double _xOffset, _yOffset, _zoom;

    int perlin[512];

    inline double lerp(double weight, double pointA, double pointB);
    inline double fade(double pointOffset);
    inline double grad(int hash, double x, double y, double z);
    inline double noise(double x, double y, double z);

protected slots:
        void setZoom(double value);
        void setXCentre(double);
        void setYCentre(double y);


};

#endif // PERLINNOISESHADER_H
