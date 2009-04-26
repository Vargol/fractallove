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

private:
    QImage *_trapImage;
    QString _imageFile;

    double _real, _imaginary;



};



#endif // JULIAORBITALTRAP_H
