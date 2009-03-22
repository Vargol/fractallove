#ifndef MANDELBROTINTER_H
#define MANDELBROTINTER_H

#include "MandelbrotShaderMP.h"

class MandelbrotInter : public MandelbrotShaderMP {
    Q_OBJECT
public:
    MandelbrotInter();

    virtual void render();
};

#endif // MANDELBROTINTER_H
