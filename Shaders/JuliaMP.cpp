#include "JuliaOrbitalTrapMP.h"

#include <iostream>
#include <complex>

#include <QString>
#include <QImage>

using namespace std;

JuliaOrbitalTrapMP::JuliaOrbitalTrapMP() {

    _image = NULL;
    _real = 0.7;
    _imaginary = 0.7;
}

bool JuliaOrbitalTrapMP::initializeShader() {

    if(_imageFile == NULL) {
  //      return false;
    }

    if(_trapImage != NULL) {
        delete _trapImage;
    }
    _image = new QImage(_imageFile);

    return true;
}

void JuliaOrbitalTrapMP::render() {

//	sleep(10);

        std::cout << "JuliaOrbitalTrapMP::render "  << _textureWidth << ", " << _textureHeight << std::endl;

        bzero (_resultsBuffer, _textureHeight * _textureWidth * sizeof(double));

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

        double xOffset = -(widthDelta * _textureWidth / 2.0)  + _xOffset;
        double yOffset = -(heightDelta * _textureHeight / 2.0) + _yOffset;

        complex<double> c = complex<double> (_real, _imaginary);
        complex<double> z;

        unsigned int maxIter = 50;
        double maxIterDouble = 250.0 / (double)maxIter ;

        complex<double> delta = complex<double>(widthDelta, heightDelta);

        double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x;

#pragma omp parallel for ordered schedule(dynamic) private(i, x, z)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {
                        z = complex<double>(xOffset, yOffset);
                        z +=  complex<double>(x * widthDelta, y * heightDelta);
                        for (i = 0; i < maxIter; i++) {
                                if(z.real() * z.real() + z.imag() * z.imag() >=  4.0) {
                                        break;
                                }
                                z = z*z + c;
                        }
                        *(resultsBuffer+(y * _textureWidth) + x) = i;
                }
        }

std::cout << "JuliaOrbitalTrapMP::render finished mandlebrot" << std::endl;

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

