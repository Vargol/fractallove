#ifndef MANDELBROTORBITTRAP_H
#define MANDELBROTORBITTRAP_H

#include "MandelbrotShaderMP.h"

class MandelbrotOrbitTrap : public MandelbrotShaderMP
{
    Q_OBJECT
public:
    MandelbrotOrbitTrap();
    virtual void render();
};

#endif // MANDELBROTORBITTRAP_H
