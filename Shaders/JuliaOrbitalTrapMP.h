#ifndef JULIAORBITALTRAP_H
#define JULIAORBITALTRAP_H

#include "MandelbrotShaderMP.h"

class JuliaOrbitalTrapMP : public MandelbrotShaderMP
{
    Q_OBJECT

public:

    JuliaOrbitalTrapMP();

    /* shader interface */
    virtual bool initializeShader();  // things thats need to be done once
    virtual void render();
    virtual void setRenderSize(unsigned int width, unsigned int height);
    virtual QLayout *getParameterLayout();  // get layout containing parameter controls

private:
    QImage *_trapImage;
    QString _imageFile;

    double _real, _imaginary;
    int _oversample;

protected slots:

    void setReal(double);
    void setImaginary(double);

};




#endif // JULIAORBITALTRAP_H
