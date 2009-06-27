#include "Mobius.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QPushButton>

#include <math.h>
#include <iostream>

Mobius::Mobius() {
    
    _functionName = std::string("Mobius Fractal");
    _zoom = 1.0;
    _xOffset = 0.0;
    _yOffset = 0.0;
    _iterations = 1000;
    _image = NULL;

    tag = new int[1000];
    
    word = vector<Matrix>(1000);
    gen = vector<Matrix>(5);
    

    _taReal =  1.95;
    _taImaginary = 0.02;
    _tbReal =  2.01;
    _tbImaginary = 0.0;

    _taReal =  1.87;
    _taImaginary = 0.10;
    _tbReal =  1.87;
    _tbImaginary = -0.10;

/*    _taReal =  2.0;
    _taImaginary = 0.0;
    _tbReal =  2.0;
    _tbImaginary = 0.0;
*/
}


bool Mobius::initializeShader() {

    return true;
}

void Mobius::render() {

    Matrix ma = Matrix();
    Matrix mb = Matrix();
    // create the generator matrices using Grandma's recipie

    grandmasRecipie(complex<double>(_taReal, _taImaginary), complex<double>(_tbReal, _tbImaginary), ma, mb);

    Matrix mA = ma.inv();
    Matrix mB = mb.inv();

    // skip [0] to keep indices in line with Indra Pearls alogrithm
    gen[0] = ma;
    gen[1] = mb;
    gen[2] = mA;
    gen[3] = mB;

    char print[4] = {'a', 'b', 'A', 'B'};
    // start at one of the fixed points

    oldPoint =  fixedPoint(ma * mb * mA * mB);

    // parabolic array holds fixed points for infinite worlds

    parabolics[0][0] = fixedPoint(mb * mA * mB *ma);
    parabolics[0][1] = fixedPoint(ma);
    parabolics[0][2] = fixedPoint(mB * mA * mb * ma);
    parabolics[1][0] = fixedPoint(mA * mB * ma * mb);
    parabolics[1][1] = fixedPoint(mb);
    parabolics[1][2] = fixedPoint(ma * mB * mA * mb);
    parabolics[2][0] = fixedPoint(mB * ma * mb * mA);
    parabolics[2][1] = fixedPoint(mA);
    parabolics[2][2] = fixedPoint(mb * ma * mB * mA);
    parabolics[3][0] = fixedPoint(ma * mb * mA * mB);
    parabolics[3][1] = fixedPoint(mB);
    parabolics[3][2] = fixedPoint(mA * mb * ma * mB);

    tag[0]=0;
    word[0]=gen[0];

    level=0;
//    cout << print[tag[level]] << endl;
    cout << "DEPTH: " << DEPTH << endl;

    _image = new QImage (_imageBuffer, _textureWidth, _textureHeight,  _textureWidth*4 , QImage::Format_RGB32);
    _image->fill(0x00FFFFFF);

    QPainter painter (_image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int minLevel = DEPTH + 1;

    while(1==1) {
//    for(int loop = 1; loop < 1000000; loop++) {

        while(!(branchTermination(painter))) {
            level++;
//            tag[level] = ((tag[level-1] + 1) % 4 );
            tag[level] = ((tag[level-1] + 1) & 3 );
            word[level] = word[level-1] * gen[tag[level]];
        }

        do  {
            level--;
        } while( level >= 0 && (!availableTurn()) );


//        tag[level+1] = ((tag[level+1] + 3) % 4) ;
        tag[level+1] = ((tag[level+1] + 3) & 3) ;

        if(tag[0]==0 && level < 0) {
            cout << "Finished" << endl;
            break;
        }

        if(level < 0) {
            word[0] = gen[tag[0]];

        } else {
            word[level+1] = word[level] * gen[tag[level+1]];
;
        }
        level++;
    }



    painter.end();

}
bool Mobius::postRendertidyUp() {

    return true;

}
QLayout *Mobius::getParameterLayout() {

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

void Mobius::mouseReleaseEvent (QMouseEvent *event) {

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


void Mobius::grandmasRecipie(complex<double> ta, complex<double> tb, Matrix &ma, Matrix &mb) {


    complex<double> tab;
    complex<double> z0;

    // first solve the quadratic equation x^2 - ta.tb.x + ta^2 + tb^2 = 0
 // using -b +- sqrt(b^2 - 4ac) / 2a   a = 1, b = -ta.tb and c = ta^2 + tb^2
 // set tab to x

    tab = solveQuadratic(complex<double>(1,0), complex<double>(-1, 0) * ta * tb, (ta * ta) + (tb * tb));


  // z0 is another big calc

    z0 = ((tab - r_2) * tb) / ((tb * tab) - (r_2 * ta) + (i_2 * tab));


  //  finally generate the two generator matrices
    ma.setMatrix(ta / r_2,   ((ta*tab) - (r_2 * tb) + i_4) / (((r_2*tab) + r_4) * z0),
                 (((ta*tab) - (r_2 * tb) - i_4) * z0) / ((r_2*tab) - r_4), ta / r_2);


    mb.setMatrix((tb - i_2)/ r_2, tb / r_2, tb / r_2, (tb + i_2)/ r_2);

    return;
}

complex<double> Mobius::solveQuadratic(complex<double> a, complex<double> b, complex<double> c) {

    return ((complex<double>(-1, 0) * b) + sqrt((b * b) - (r_4 * a * c)))/ (r_2 * a);

}

complex<double> Mobius::fixedPoint(Matrix m) {

    // the fixed points are found by solving the quadratic formula
    // cz^2 + (d - a)z - b
    return  solveQuadratic(m._c, (m._d-m._a), -m._b);

}

bool Mobius::branchTermination(QPainter &painter) {

    complex<double> z[3];

    newPoint = mobius(oldPoint, word[level]);
    double dist = distance(oldPoint, newPoint);

    for(int j=0; j<3; j++)  {
        z[j]=mobius(parabolics[tag[level]][j],word[level]);
    }

    if(level >= DEPTH || ((dist<EPSILON) && (distance(z[0],z[1])<EPSILON) && (distance(z[1],z[2])<EPSILON))) {
//    if(level >= DEPTH || dist<EPSILON) {
        //          stroke(0,4*level,250-5*level);
        painter.setPen(QColor(0.0,2*level, 250-(2*level)));
        painter.drawLine(oldPoint.real() * 10.0 + 600, oldPoint.imag() * 10.00 + 450, newPoint.real() * 10.0 + 600, newPoint.imag() * 10.0 + 450 );
        oldPoint=newPoint;
        return true;
    } else {
        return false;
    }
}

complex<double> Mobius::mobius(complex<double> z, Matrix m) {

    // finally the actual Mobius translation
    return ((m._a * z) + m._b) / ((m._c * z) + m._d);

}

double Mobius::distance(complex<double> z1, complex<double> z2)
{
  return (((z1.real()-z2.real())*(z1.real()-z2.real()))+((z1.imag()-z2.imag())*(z1.imag()-z2.imag())));
}


 bool Mobius::availableTurn() {
//  if ((tag[level+1] + 3) % 4 == (tag[level] + 2) % 4) {
  if (((tag[level+1] + 3) & 3) == ((tag[level] + 2) & 3)) {
    return false;
  } else {
    return true;
  }
}

/// slots

void Mobius::setZoom(double value) {

        _zoom = value;
}


void Mobius::setXCentre(int x) {

        _xOffset = x;
}

void Mobius::setYCentre(int y) {

        _yOffset = y;
}

void Mobius::setTaReal(double r) {

    _taReal = r;
}
void Mobius::setTaImaginary(double i) {
    _taImaginary = i;
}
void Mobius::setTbReal(double r) {
    _tbReal = r;
}
void Mobius::setTbImaginary(double i) {
    _tbImaginary = i;
}




