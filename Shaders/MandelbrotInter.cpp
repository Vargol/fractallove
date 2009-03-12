#include "MandelbrotInter.h"

#include <iostream>
#include <complex>

#include <QImageWriter>

typedef std::complex<double> complex;

MandelbrotInter::MandelbrotInter()
{
    _functionName = std::string("fnMandelbrotInter");
}

void MandelbrotInter::render() {

//	sleep(10);

        std::cout << "MandelbrotIter::render "  << _textureWidth << ", " << _textureHeight << std::endl;

        bzero (_resultsBuffer, _textureHeight * _textureWidth * sizeof(double));

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

  //      double xOffset = -(widthDelta * _textureWidth / 2.0);
  //      double yOffset = -(heightDelta * _textureHeight / 2.0);

        double xOffset = _xOffset - (widthDelta * _textureWidth / 2.0);
        double yOffset =  _yOffset - (heightDelta * _textureHeight / 2.0);

        complex z0 = complex (xOffset, yOffset);
        complex c, z;
        unsigned int maxIter = 50;
        double maxIterDouble = 250.0 / (double)maxIter ;
        complex delta = complex(widthDelta, heightDelta);

        double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x, result, oldResult;

        complex temp;

#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0, result, oldResult)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {
                        z0 = complex(xOffset, yOffset);
                        z0 +=  complex(x * widthDelta, y * heightDelta);;
                        z = z0;
                        for (i = 0; i < maxIter; i++) {
                                result = z.real() * z.real() + z.imag() * z.imag();
                                if(result  >=  4.0) {
                                        break;
                                }
                                z = z*z + z0;
                                oldResult = result;
                        }
                        *(resultsBuffer+(y * _textureWidth) + x) = i + ((4.0 - oldResult) / (result - oldResult)) - 1.0;
                        z0 += widthDelta;
                }
        }

std::cout << "MandelbrotIter::render finished mandlebrot" << std::endl;

        resultsBuffer = _resultsBuffer;
        unsigned char *imageBuffer = _imageBuffer;
        unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {
                        int offset = (y * _textureWidth) + x;
//                      std::cout << offset << " ";
                        byteValue = (unsigned char)(*(resultsBuffer+offset) * (double)maxIterDouble);
                        *(imageBuffer  + (offset * 4)) = byteValue;
                        *(imageBuffer  + (offset * 4) + 1) = byteValue;
                        *(imageBuffer  + (offset * 4) + 2) = byteValue;
                        *(imageBuffer  + (offset * 4) + 3) = 0.0;
                }
//		std::cout << "next ";
        }

        if(_image != NULL) {
            delete _image;
        }
        _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);

        QString str = "/home/david/test.png";

        bool saved = _image->save(str);

        std::cout << "Saved image " << saved <<  std::endl;



}
