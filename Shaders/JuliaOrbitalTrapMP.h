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

private:
    QImage *_trapImage;
    QString _imageFile;

    double _real, _imaginary;
    int _oversample;



};



#endif // JULIAORBITALTRAP_H
