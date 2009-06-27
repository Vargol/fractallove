#include "CurlicueFractal.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QPushButton>

#include <math.h>

CurlicueFractal::CurlicueFractal() {

        _functionName = std::string("Curlicue Fractal");
        _zoom = 1.0;
        _xOffset = 0.0;
        _yOffset = 0.0;
        _iterations = 1000;
        _image = NULL;
        _irrational =  (2 - sqrt(29)) / 4;

}


bool CurlicueFractal::initializeShader() {
    return true;
}
void CurlicueFractal::render() {

    _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);

    _image->fill(0x00FFFFFF);

    QPainter painter (_image);
    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.scale(_zoom, _zoom);
//    painter.translate(_xOffset, _yOffset);

    int xOffset = (_textureWidth / 2) + _xOffset;
    int yOffset = (_textureHeight / 2) + _yOffset;

    double theta = 0;
    double phi = 0;
    double x0, y0, x1, y1;

    x0 = 0;
    y0 = 0;


    for(int i=0; i<_iterations; i++) {

        x1 = x0 + cos(phi);
        y1 = y0 + sin(phi);

        painter.drawLine((_zoom * x0) + xOffset, (_zoom * -y0)  + yOffset, (_zoom * x1)  + xOffset, (_zoom * -y1)  + yOffset);

        x0 = x1;
        y0 = y1;

        phi = fmod(theta + phi, 2 * M_PI);
        theta = fmod(theta + (2 * M_PI * _irrational) , 2 * M_PI);
    }
    painter.end();

}
bool CurlicueFractal::postRendertidyUp() {

    return true;

}
QLayout *CurlicueFractal::getParameterLayout() {

        QGridLayout *parameterLayout = new QGridLayout;

        parameterLayout->addWidget(new QLabel(tr("Centre x: ")), 1, 0);
        parameterLayout->addWidget(new QLabel(tr("Centre y: ")), 2, 0);
        parameterLayout->addWidget(new QLabel(tr("Zoom: ")), 3, 0);
        parameterLayout->addWidget(new QLabel(tr("Iterations: ")), 4, 0);
        parameterLayout->addWidget(new QLabel(tr("Centre x: ")), 5, 0);

        _xOffsetSpinBox = new QSpinBox;
        _xOffsetSpinBox->setRange(-999999, 999999);
        _xOffsetSpinBox->setValue(_xOffset);
        parameterLayout->addWidget(_xOffsetSpinBox, 1, 1);

        _yOffsetSpinBox = new QSpinBox;
        _yOffsetSpinBox->setRange(-999999, 999999);
        _yOffsetSpinBox->setValue(_yOffset);
        parameterLayout->addWidget(_yOffsetSpinBox, 2, 1);


        QDoubleSpinBox *_zoomSpinBox = new QDoubleSpinBox;
        _yOffsetSpinBox->setRange(-9999999, 9999999);
        _zoomSpinBox->setDecimals(7);
        _zoomSpinBox->setValue(_zoom);

        parameterLayout->addWidget(_zoomSpinBox, 3, 1);


        QSpinBox *_iterationsSpinBox = new QSpinBox;
        _iterationsSpinBox->setMinimum(1);
        _iterationsSpinBox->setMaximum(INT32_MAX);
        _iterationsSpinBox->setValue(_iterations);
        parameterLayout->addWidget(_iterationsSpinBox, 4, 1);

        QComboBox *irrationals = new QComboBox;

        irrationals->addItem(tr("sqrt(2)"));

        parameterLayout->addWidget(irrationals);

        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setXCentre(int)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setYCentre(int)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom(double)));
        QObject::connect(_iterationsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setIterations(int)));

        _parameterLayout    = parameterLayout;

    return _parameterLayout;
}

void CurlicueFractal::mouseReleaseEvent (QMouseEvent *event) {

        double widthDelta, heightDelta;


        if(_textureWidth < _textureHeight) {
                widthDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureWidth + 1.0);
                heightDelta = _zoom / (double)(_textureHeight + 1.0);
        } else {
            heightDelta = _zoom * (_textureWidth / (double)_textureHeight) / (_textureHeight + 1.0);
            widthDelta = _zoom / (double)(_textureHeight + 1.0);
        }

        double xOffset = _xOffset -(widthDelta * _textureWidth / 2.0);
        double yOffset = _yOffset -(heightDelta * _textureHeight / 2.0);

        _xOffset = xOffset + (widthDelta * event->x());
        _yOffset = yOffset + (heightDelta * event->y());

        if(_xOffsetSpinBox != NULL) {
            _xOffsetSpinBox->setValue(_xOffset);
            _yOffsetSpinBox->setValue(_yOffset);
        }
}

void CurlicueFractal::setZoom(double value) {

        _zoom = value;
}


void CurlicueFractal::setXCentre(int x) {

        _xOffset = x;
}

void CurlicueFractal::setYCentre(int y) {

        _yOffset = y;
}

void CurlicueFractal::setIterations(int i) {

        _iterations = i;
}



