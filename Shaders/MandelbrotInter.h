#ifndef MANDELBROTINTER_H
#define MANDELBROTINTER_H

#include "MandelbrotShaderMP.h"

class MandelbrotInter : public MandelbrotShaderMP {
public:
    MandelbrotInter();

    virtual void render();
};

#endif // MANDELBROTINTER_H
