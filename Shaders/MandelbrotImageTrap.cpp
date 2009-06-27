#include "MandelbrotImageTrap.h"

#include <iostream>
#include <complex>

#include <QString>
#include <QImage>

using namespace std;

MandelbrotImageTrap::MandelbrotImageTrap() {

    _image = NULL;
    _real = 0.7;
    _imaginary = 0.2;
    _imageFile = "basic00.jpg";
    _trapImage = NULL;
    _oversample = 1;
}

void MandelbrotImageTrap::setRenderSize(unsigned int width, unsigned int height) {

//	width = width + 4 - (width & 3);

        std::cout << "MandelbrotShaderMP::setRenderSize "  << width << ", " << height << std::endl;

        if(_textureWidth != width || _textureHeight != height) {


                if(_imageBuffer != NULL) {
                        free(_imageBuffer);
                }
                _imageBuffer = (unsigned char *)malloc(_oversample * _oversample * 4 * height * width * sizeof(unsigned char));
        }

        _textureWidth = width;
        _textureHeight = height;

}


bool MandelbrotImageTrap::initializeShader() {

    if(_imageFile == NULL) {
  //      return false;
    }

    if(_trapImage != NULL) {
        delete _trapImage;
    }
    _trapImage = new QImage(_imageFile);

    cout << "image depth" << _trapImage->depth();

//    _trapImage->fill(32765);

    return true;
}

void MandelbrotImageTrap::render() {

//	sleep(10);
    int textureHeight = _textureHeight * _oversample;
    int textureWidth = _textureWidth * _oversample;

    int height = _trapImage->height() - 1;
    int width = _trapImage->width() - 1;
    int bps = _trapImage->depth() == 32 ? 4 : 3;
        std::cout << "MandelbrotImageTrap::render "  << _textureWidth << ", " << _textureHeight << std::endl;

 //       bzero (_resultsBuffer, _textureHeight * _textureWidth * sizeof(double));

        double widthDelta, heightDelta;


        if(textureWidth < textureHeight) {
                widthDelta = _zoom * (textureWidth / (double)textureHeight) / (textureWidth + 1.0);
                heightDelta = _zoom / (double)(textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (textureWidth / (double)textureHeight) / (textureHeight + 1.0);
            widthDelta = _zoom / (double)(textureHeight + 1.0);
        }

        double xOffset = _xOffset - (widthDelta * textureWidth / 2.0);
        double yOffset = _yOffset - (heightDelta * textureHeight / 2.0);

        complex<double> z, z0;

        unsigned int maxIter = 50;
        double maxIterDouble = 250.0 / (double)maxIter ;

        complex<double> delta = complex<double>(widthDelta, heightDelta);

        double *resultsBuffer = _resultsBuffer;

        unsigned int i;

        unsigned int x = 0;

        unsigned char *imageBuffer = _imageBuffer;
        unsigned char *trapBuffer = _trapImage->bits();

        complex<double> imageOffset = complex<double>(0.5, 0.5);

#pragma omp parallel for ordered schedule(dynamic) private(i, x, z, z0)
        for(unsigned int y=0; y<textureHeight; y++) {
                z0 = complex<double>(xOffset, yOffset);
                z0 +=  complex<double>(x * widthDelta, y * heightDelta);;
                z = z0;
                for(x=0; x<textureWidth; x++) {
                       z = complex<double>(xOffset, yOffset);
                       z +=  complex<double>(x * widthDelta, y * heightDelta);
                       for (i = 0; i < maxIter; i++) {
                              if(z.real() * z.real() + z.imag() * z.imag() >=  4.0) {
                                     break;
                               }
                               z = z*z + z0;
                       }
                        
                       int offset = (y * textureWidth) + x;

                        double real = fabs(z.real());
                        real -=  (int)real;
                        int trapOffset = floor(real *  height) * width;

                        double imag = fabs(z.imag());
                        imag -=  (int)imag;
                        trapOffset += (imag * width);

                       if(trapOffset * bps > _trapImage->numBytes()) {
                           cout <<  "bad value " <<fmod(fabs(z.real()), 1.0) << ',' <<fmod(fabs(z.imag()), 1.0)<< endl;
                           cout <<  "offset " << trapOffset << " size " << _trapImage->numBytes() << endl;
                       }

                        *(imageBuffer  + (offset * 4)) = *(trapBuffer + (trapOffset * bps));
                        *(imageBuffer  + (offset * 4) + 1) = *(trapBuffer + (trapOffset * bps)+1);
                        *(imageBuffer  + (offset * 4) + 2) = *(trapBuffer + (trapOffset * bps)+2);
                        *(imageBuffer  + (offset * 4) + 3) = *(trapBuffer + (trapOffset * bps)+3);

                }
        }

std::cout << "MandelbrotImageTrap::render finished mandlebrot" << std::endl;

        if(_image != NULL) {
            delete _image;
        }

        QImage *image = new QImage (_imageBuffer, textureWidth, textureHeight,  textureWidth*4 , QImage::Format_RGB32);

        _image = new QImage(image->scaled(_textureWidth, _textureHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        delete image;

        QString str = "~/test.png";

        bool saved = _image->save(str);

        std::cout << "Saved image " << saved <<  std::endl;



}

