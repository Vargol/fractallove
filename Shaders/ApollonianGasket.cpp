#include "ApollonianGasket.h"

#include <QPainter>

ApollonianGasket::ApollonianGasket()
{

    _oversample = 3;
    _circleCount = 3;
    _levels = 12;
    _showInversionCircles = false;
    _zoom = 200 * _oversample;

}


bool ApollonianGasket::initializeShader() {

    return true;
}

void ApollonianGasket::render() {


    vector<double> V;
    vector<double> cosV;
    vector<double> sinV;
    double diameter;
    double radius;
    double radiusInner1;

    for(double radians=0; radians <= 2.0 * M_PI; radians += (2 * M_PI / _circleCount)) {

        double value = radians + (M_PI * 0.5);
        double cosValue = cos(value);
        double sinValue = sin(value);
        V.push_back(value);
        cosV.push_back(cosValue);
        sinV.push_back(sinValue);
    }

    /*
   for(int i=1; i < _circleCount; i++) {

       double cosDiff = cosV[i] - cosV[i-1];
       double sinDiff = sinV[i] - sinV[i-1];

       diameter.push_back(sqrt((cosDiff * cosDiff) + (sinDiff - sinDiff)));
   }
*/

      double cosDiff = cosV[1] - cosV[0];
      double sinDiff = sinV[1] - sinV[0];

       diameter = sqrt((cosDiff * cosDiff) + (sinDiff * sinDiff));

    radius = diameter * 0.5;
    radiusInner1 = sqrt((cosV[0] * cosV[0]) + (sinV[0] * sinV[0])) - radius;

    // create top level circles

    _circles.push_back(new Circles());
    _circles[0]->x.push_back(0.0);
    _circles[0]->x.push_back(0.0);
    _circles[0]->x.insert(_circles[0]->x.end(), cosV.begin(), --cosV.end());

    _circles[0]->y.push_back(0.0);
    _circles[0]->y.push_back(0.0);
    _circles[0]->y.insert(_circles[0]->y.end(), sinV.begin(), --sinV.end());

    _circles[0]->r.push_back((2.0 * radius) + radiusInner1);
    _circles[0]->r.push_back(radiusInner1);
    _circles[0]->r.insert(_circles[0]->r.end(), _circleCount, radius);

    // create inversion circles


    for(int t = 0; t< _circleCount; t++) {

        double xMid = (cosV[t] + cosV[t+1]) * 0.5;
        double yMid = (sinV[t] + sinV[t+1]) * 0.5;

        double a1 = atan2(sinV[t], cosV[t]);
        double a2 = atan2(sinV[t+1], cosV[t+1]);
        double x1 = cosV[t] + cos(a1) * radius;
        double y1 = sinV[t] + sin(a1) * radius;
        double x2 = cosV[t+1] + cos(a2) * radius;
        double y2 = sinV[t+1] + sin(a2) * radius;
        circleFromThreePoints(_inversionCircles, x1, y1, x2, y2, xMid, yMid);
    }

    double xMid = (cosV[0] + cosV[1]) * 0.5;
    double yMid = (sinV[0] + sinV[1]) * 0.5;

    _inversionCircles.x.push_back(0);
    _inversionCircles.y.push_back(0);
    _inversionCircles.r.push_back(sqrt((xMid * xMid)+(yMid * yMid)));

    for(int i=0; i < _levels - 1; i++) {

        Circles *circles = new Circles();
        _circles.push_back(circles);

        for(unsigned int j=0; j < _circles[i]->x.size(); j++) {
            for(int k=0; k <= _circleCount; k++) {

                double invCircleX = _inversionCircles.x[k];
                double invCircleY = _inversionCircles.y[k];
                double invCircleR = _inversionCircles.r[k];

                double circleX = _circles[i]->x[j];
                double circleY = _circles[i]->y[j];
                double circleR = _circles[i]->r[j];

                double xDiff = invCircleX - circleX;
                double yDiff = invCircleY - circleY;

                diameter = sqrt((xDiff*xDiff)+(yDiff*yDiff));

                if(diameter > _inversionCircles.r[k]) {

                    double r, x, y;

                    inversionCircle(circleX, circleY, circleR, invCircleX, invCircleY, invCircleR, &x, &y, &r);

                    xDiff = x - circleX;
                    yDiff = y - circleY;

                    if(sqrt((xDiff*xDiff)+(yDiff*yDiff)) > circleR) {

                        circles->x.push_back(x);
                        circles->y.push_back(y);
                        circles->r.push_back(r);

                    }
                }
            }
        }
    }



    QImage *image = new QImage (_imageBuffer, _textureWidth * _oversample, _textureHeight * _oversample,  _textureWidth * _oversample * 4 , QImage::Format_RGB32);


    image->fill(0x00FFFFFF);

    QPainter painter (image);

    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    if(_showInversionCircles) {
            for(unsigned int i=0; i<_inversionCircles.r.size(); i++) {

                painter.setPen(QColor(255, 0, 0));
                painter.drawEllipse(QPointF (_inversionCircles.x[i] * _zoom + 600.0, _inversionCircles.y[i] * _zoom + 450.0),
                                    _inversionCircles.r[i] * _zoom ,
                                    _inversionCircles.r[i] * _zoom);
            }

    }


      /* we want to draw the big circle in a different colour */

    Circles *circles = _circles[0];


    painter.setPen(QPen(QBrush(QColor(0, 0, 0),Qt::SolidPattern), 5.0));

    painter.drawEllipse(QPointF (circles->x[0] * _zoom + (600.0 * _oversample), circles->y[0] * _zoom + (450.0* _oversample)),
                        circles->r[0] * _zoom,
                        circles->r[0] * _zoom );

    for(unsigned int i=1; i<circles->r.size(); i++) {
        painter.setBrush(QBrush(QColor(255, 0, 255), Qt::SolidPattern));
        painter.drawEllipse(QPointF (circles->x[i] * _zoom + (600.0 * _oversample), circles->y[i] * _zoom + (450.0* _oversample)),
                            circles->r[i] * _zoom,
                            circles->r[i] * _zoom );
    }

    for(int j=1; j < _levels; j++) {

        Circles *circles = _circles[j];

        for(unsigned int i=0; i<circles->r.size(); i++) {

            painter.setBrush(QBrush(QColor((j * 255/_levels), 0, 255), Qt::SolidPattern));
            painter.drawEllipse(QPointF (circles->x[i] * _zoom + (600.0 * _oversample), circles->y[i] * _zoom + (450.0* _oversample)),
                                circles->r[i] * _zoom,
                                circles->r[i] * _zoom );

        }

    }

    painter.end();

    if(_oversample > 1) {
        _image = new QImage(image->scaled(_textureWidth, _textureHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        delete image;
    } else {
        _image = image;
    }

}


void ApollonianGasket::inversionCircle(double x1, double y1, double r1, double x2, double y2, double r2, double *x, double *y, double *r) {


 double theta = atan2((y1-y2),(x1-x2));

 double ix1, ix2, iy1, iy2;

 inversion(x1+r1*cos(theta),y1+r1*sin(theta),x2,y2,r2, &ix1, &iy1);
 inversion(x1-r1*cos(theta),y1-r1*sin(theta),x2,y2,r2, &ix2, &iy2);

 *x = (ix1+ix2) * 0.5;
 *y = (iy1+iy2) * 0.5;
 *r = sqrt(((ix1-ix2) * (ix1-ix2)) + ((iy1-iy2) * (iy1-iy2))) * 0.5;

}

void ApollonianGasket::inversion(double x1, double y1, double x2, double y2, double r2,  double *x, double *y) {

 double xDiff = x1-x2;
 double yDiff = y1-y2;

 double d  = sqrt((xDiff * xDiff) + (yDiff * yDiff));
 double dn = (r2 * r2) / d;

 double theta = atan2(yDiff,xDiff);

 *x = x2 + dn*cos(theta);
 *y = y2 + dn*sin(theta);

}

void ApollonianGasket::circleFromThreePoints(Circles &inversionCircles, double x1, double y1, double x2, double y2, double x3, double y3) {


    double ma = (y2 - y1) / (x2 -x1);
    double mb = (y3 - y2) / (x3 -x2);

    double x = ((ma * mb) * (y1 - y3)) + (mb * (x1 + x2)) - (ma * (x2 + x3));
    x /= (2.0 * (mb - ma));

    double y = (-1.0 / ma) * (x - ((x1 + x2) / 2.0 ));
    y += ((y1 + y2) / 2.0);

    double xDiff = x - x1;
    double yDiff = y - y1;

    double r = sqrt((xDiff * xDiff) + (yDiff * yDiff));

    inversionCircles.x.push_back(x);
    inversionCircles.y.push_back(y);
    inversionCircles.r.push_back(r);

}


bool ApollonianGasket::postRendertidyUp() {

    return true;

}
QLayout *ApollonianGasket::getParameterLayout() {

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


        QSpinBox *_levelSpinBox = new QSpinBox;
        _levelSpinBox->setMinimum(1);
        _levelSpinBox->setMaximum(INT32_MAX);
        _levelSpinBox->setValue(_levels);
        parameterLayout->addWidget(_levelSpinBox, 4, 1);


        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setXCentre(int)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setYCentre(int)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZoom(double)));
        QObject::connect(_levelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setIterations(int)));

        _parameterLayout    = parameterLayout;

    return _parameterLayout;
}

void ApollonianGasket::mouseReleaseEvent (QMouseEvent *event) {

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

void ApollonianGasket::setRenderSize(unsigned int width, unsigned int height) {

        std::cout << "MandelbrotShaderMP::setRenderSize "  << width << ", " << height << std::endl;

        if(_textureWidth != width || _textureHeight != height) {

                if(_imageBuffer != NULL) {
                        free(_imageBuffer);
                }
                _imageBuffer = (unsigned char *)malloc(4 * height * width * _oversample * _oversample * sizeof(unsigned char));
        }

        _textureWidth = width;
        _textureHeight = height;

}
