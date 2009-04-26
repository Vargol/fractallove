#include "MandelbrotOrbitTrap.h"

#include <iostream>
#include <complex>

#include <QImageWriter>

using namespace std;


MandelbrotOrbitTrap::MandelbrotOrbitTrap()
{
    _functionName = std::string("fnMandelbrotInter");
}

void MandelbrotOrbitTrap::render() {

//	sleep(10);
        complex<double> orbitOrigin = complex<double>(0.25, 0.35);
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

        complex<double> z0 = complex<double>(xOffset, yOffset);
        complex<double> c, z;
        unsigned int maxIter = 50;
        double maxIterDouble = 250.0 / (double)maxIter ;
        complex<double> delta = complex<double>(widthDelta, heightDelta);

        double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x, result = 0.0, oldResult = 0.0;

        complex<double> temp;

#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0, result, oldResult)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {
                        result = 0.0;
                        z0 = complex<double>(xOffset, yOffset);
                        z0 +=  complex<double>(x * widthDelta, y * heightDelta);;
                        z = z0;
                        for (i = 0; i < maxIter; i++) {
                                result = z.real() * orbitOrigin.real() + z.imag() * orbitOrigin.imag();
                                if(result  >=  4.0) {
                                        break;
                                }
                                z = z*z + z0;
                        }
                        *(resultsBuffer+(y * _textureWidth) + x) = result;
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
