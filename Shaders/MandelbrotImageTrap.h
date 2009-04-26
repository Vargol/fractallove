#ifndef MANDELBROTIMAGELTRAP_H
#define MANDELBROTIMAGELTRAP_H

#include "MandelbrotShaderMP.h"

class MandelbrotImageTrap : public MandelbrotShaderMP
{
    Q_OBJECT

public:

    MandelbrotImageTrap();

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
