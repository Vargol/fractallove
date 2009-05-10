
#include <iostream>

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "PerlinNoiseShader.h"

using namespace std;

PerlinNoiseShader::PerlinNoiseShader() {
}


bool PerlinNoiseShader::initializeShader() {

    for (int i = 0; i < PERLIN_SIZE; i++) {
            perlin[i] = i;
    }

    srand(time(0));
    //stir the table up, monte carlo style
    for (int i = 0; i < PERLIN_SIZE; i++) {
            int tmp = perlin[i];
            int j = rand() % PERLIN_SIZE;
            perlin[i] = perlin[j];
            perlin[j] = tmp;
    }

    // extend the tables so cwe can cheat the indexing
    for (int i = 0; i < PERLIN_SIZE; i++) {
            perlin[PERLIN_SIZE + i] = perlin[i];
    }

    return true;
}

inline double PerlinNoiseShader::lerp(double weight, double pointA, double pointB) {
                return pointA + weight * (pointB - pointA);
        }

inline double PerlinNoiseShader::fade(double pointOffset) {
                return pointOffset
                        * pointOffset
                        * pointOffset
                        * (pointOffset * (pointOffset * 6 - 15) + 10);
        }

inline double PerlinNoiseShader::grad(int hash, double x, double y, double z) {
                // use Ken Perlin hashing algorithm. This fixed set of gradients
                // is designed to reduce the grid like artifacts you can get
                // in Ken's original algorithm.

                int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
//			double u = h < 8 || h == 12 || h == 13 ? x : y,
                double u = h < 8 ? x : y,
                // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4 || h == 12 || h == 13 ? y : z;
                return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }


        // A tidied up version of Ken's reference code, with Ken's comments
inline double PerlinNoiseShader::noise(double x, double y, double z) {

                int X = (int) floor(x) & PERLIN_MASK; // FIND UNIT CUBE THAT
                int Y = (int) floor(y) & PERLIN_MASK; // CONTAINS POINT.
                int Z = (int) floor(z) & PERLIN_MASK;

                x -= floor(x); // FIND RELATIVE X,Y,Z
                y -= floor(y); // OF POINT IN CUBE.
                z -= floor(z);

                double u = fade(x); // COMPUTE FADE CURVES
                double v = fade(y); // FOR EACH OF X,Y,Z.
                double w = fade(z);

                int A = perlin[X] + Y;
                int AA = perlin[A] + Z;
                int	AB = perlin[A + 1] + Z;
                // HASH COORDINATES OF
                int B = perlin[X + 1] + Y;
                int BA = perlin[B] + Z;
                int BB = perlin[B + 1] + Z;
                // THE 8 CUBE CORNERS,

                return 	lerp(w,
                                        lerp(v,
                                                lerp(u, grad(perlin[AA], x, y, z), // AND ADD
                                                        grad(perlin[BA], x - 1, y, z)), // BLENDED
                                                lerp(u, grad(perlin[AB], x, y - 1, z), // RESULTS
                                                        grad(perlin[BB], x - 1, y - 1, z))), // FROM  8
                                        lerp(v, lerp(u, grad(perlin[AA + 1], x, y, z - 1), // CORNERS
                                                grad(perlin[BA + 1], x - 1, y, z - 1)), // OF CUBE
                                        lerp(
                                                u,
                                                grad(perlin[AB + 1], x, y - 1, z - 1),
                                                grad(perlin[BB + 1], x - 1, y - 1, z - 1))));
}

bool PerlinNoiseShader::postRendertidyUp() {

        return true;

}

QLayout *PerlinNoiseShader::getParameterLayout(void) {

        QGridLayout *parameterLayout = new QGridLayout;

        parameterLayout->addWidget(new QLabel(tr("Centre x: ")), 1, 0);
        parameterLayout->addWidget(new QLabel(tr("Centre y: ")), 2, 0);
        parameterLayout->addWidget(new QLabel(tr("Zoom: ")), 3, 0);

       QDoubleSpinBox *_xOffsetSpinBox = new QDoubleSpinBox;
        _xOffsetSpinBox->setMinimum(-99.9);
        _xOffsetSpinBox->setDecimals(9);
        _xOffsetSpinBox->setValue(_xOffset);
        parameterLayout->addWidget(_xOffsetSpinBox, 1, 1);

        QDoubleSpinBox *_yOffsetSpinBox = new QDoubleSpinBox;
        _yOffsetSpinBox->setMinimum(-99.9);
        _yOffsetSpinBox->setDecimals(9);
        _yOffsetSpinBox->setValue(_yOffset);
        parameterLayout->addWidget(_yOffsetSpinBox, 2, 1);

        QDoubleSpinBox *_zoomSpinBox = new QDoubleSpinBox;
        _zoomSpinBox->setMinimum(-99.9);
        _zoomSpinBox->setDecimals(9);
        _zoomSpinBox->setValue(_zoom);

        parameterLayout->addWidget(_zoomSpinBox, 3, 1);

        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXCentre(double)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYCentre(double)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom(double)));

        _parameterLayout    = parameterLayout;

    return _parameterLayout;

 }


void PerlinNoiseShader::render() {

//	sleep(10);

        std::cout << "PerlinNoiseShader::render "  << _textureWidth << ", " << _textureHeight << std::endl;

        bzero (_resultsBuffer, _textureHeight * _textureWidth * sizeof(double));

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

        double xOffset = -(widthDelta * _textureWidth / 2.0);
        double yOffset = -(heightDelta * _textureHeight / 2.0);

        double pnoise;
        unsigned int x;

        double *resultsBuffer = _resultsBuffer;

#pragma omp parallel for ordered schedule(static) private(pnoise, x)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {

                        pnoise = noise(xOffset+(x * widthDelta),
                                       yOffset+(y * heightDelta),
                                       0);
                        *(resultsBuffer+(y * _textureWidth) + x) = pnoise;

                }
        }

std::cout << "PerlinNoiseShaderMP::render finished." << std::endl;

        resultsBuffer = _resultsBuffer;
        unsigned char *imageBuffer = _imageBuffer;
        unsigned char byteValue;

#pragma omp for schedule(static) private (x, byteValue)
        for(unsigned int y=0; y<_textureHeight; y++) {
                for(x=0; x<_textureWidth; x++) {
                        int offset = (y * _textureWidth) + x;
//                      std::cout << offset << " ";
                        byteValue = 127 + (unsigned char)(*(resultsBuffer+offset) * 127);
                        *(imageBuffer  + (offset * 4)) = byteValue;
                        *(imageBuffer  + (offset * 4) + 1) = byteValue;
                        *(imageBuffer  + (offset * 4) + 2) = byteValue;
                        *(imageBuffer  + (offset * 4) + 3) = 0.0;
                }
//		std::cout << "next ";
        }

/*
        for (int i = 0; i < QImageWriter::supportedImageFormats().size(); ++i) {
                std::cout << QImageWriter::supportedImageFormats().at(i).constData()  << std::endl;
        }
*/

        if(_image != NULL) {
            delete _image;
        }
        _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);

        QString str = "/home/david/test.png";

        bool saved = _image->save(str);

        std::cout << "Saved image " << saved <<  std::endl;



}

std::string &PerlinNoiseShader::functionName(void) {
        return _functionName;
}


void PerlinNoiseShader::setRenderSize(unsigned int width, unsigned int height) {

//	width = width + 4 - (width & 3);

        std::cout << "MandelbrotShaderMP::setRenderSize "  << width << ", " << height << std::endl;

        if(_textureWidth != width || _textureHeight != height) {

                if(_resultsBuffer != NULL) {
                        free(_resultsBuffer);
                }
                _resultsBuffer = (double *)malloc(height * width * sizeof(double));

                if(_imageBuffer != NULL) {
                        free(_imageBuffer);
                }
                _imageBuffer = (unsigned char *)malloc(4 * height * width * sizeof(unsigned char));
        }

        _textureWidth = width;
        _textureHeight = height;

}

void PerlinNoiseShader::setZoom(double value) {
        _zoom = value;
}


void PerlinNoiseShader::setXCentre(double x) {
        _xOffset = x;
}

void PerlinNoiseShader::setYCentre(double y) {
        _yOffset = -y;
}

void PerlinNoiseShader::mouseReleaseEvent (QMouseEvent *event) {

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / _textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

        double xOffset = _xOffset -(widthDelta * _textureWidth / 2.0);
        double yOffset = _yOffset -(heightDelta * _textureHeight / 2.0);

        _xOffset = xOffset + (widthDelta * event->x());
        _yOffset = yOffset + (heightDelta * event->y());
}
