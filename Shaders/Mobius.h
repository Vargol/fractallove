#ifndef MOBIUS_H
#define MOBIUS_H


#include <QDoubleSpinBox>

#include "Shader.h"


#include <complex>


using namespace std;

    static const complex<double> r_2 = complex<double>(2, 0);
    static const complex<double> i_2 = complex<double>(0, 2);

    static const complex<double> r_4 = complex<double>(4, 0);
    static const complex<double> i_4 = complex<double>(0, 4);


class Mobius : public Shader {

    Q_OBJECT

public:

    Mobius();

    virtual bool initializeShader();
    virtual void render();
    virtual bool postRendertidyUp();
    virtual QLayout *getParameterLayout();
    virtual void mouseReleaseEvent (QMouseEvent *event);

protected slots:
    void setZoom(double value);
    void setXCentre(int);
    void setYCentre(int);

    void setTaReal(double r);
    void setTaImaginary(double i);
    void setTbReal(double r);
    void setTbImaginary(double i);

protected:

    double _xOffset, _yOffset, _zoom;
    int _iterations;

    double _taReal, _taImaginary;
    double _tbReal, _tbImaginary;


    QSpinBox *_xOffsetSpinBox;
    QSpinBox *_yOffsetSpinBox;

private:

    class Matrix {

        public:

        complex<double> _a,_b,_c,_d;

        Matrix(complex<double> a, complex<double> b, complex<double> c, complex<double> d)   {
            _a=a;
            _b=b;
            _c=c;
            _d=d;
        }

       Matrix()   {
            _a=complex<double>(0.0, 0.0);
            _b=complex<double>(0.0, 0.0);
            _c=complex<double>(0.0, 0.0);
            _d=complex<double>(0.0, 0.0);
        }
        complex<double>  det() {
            return (_a * _d) - (_b * _c);
        }

        Matrix inv() {

            complex<double> det = (_a * _d) - (_b * _c);
            return Matrix(_d / det , (complex<double>(-1,0) * _b) / det, (complex<double>(-1,0) * _c)  / det , _a / det);
        }


        Matrix operator*(Matrix n) {
            complex<double> p1 = (_a * n._a) + (_b * n._c);
            complex<double> p2 = (_a * n._b) + (_b * n._d);
            complex<double> p3 = (_c * n._a) + (_d * n._c);
            complex<double> p4 = (_c * n._b) + (_d * n._d);
            return Matrix(p1, p2, p3, p4);
        }

         void setMatrix(complex<double> a, complex<double> b, complex<double> c, complex<double> d)   {
            _a=a;
            _b=b;
            _c=c;
            _d=d;
        }
    };


    static const int DEPTH=20;
    static const double EPSILON=0.00001;
    static const double SCALE=2.5;



    int level;
    int* tag;

    vector<Matrix> word;
    vector<Matrix> gen;

    complex<double> parabolics[4][3];

    complex<double> oldPoint, newPoint, tempPoint;

    void grandmasRecipie(complex<double>, complex<double>, Matrix &ma, Matrix &mb);
    complex<double> solveQuadratic(complex<double> a, complex<double> b, complex<double> c);
    complex<double> fixedPoint(Matrix m);
    bool branchTermination(QPainter &painter);
    complex<double> mobius(complex<double> z, Matrix m);
    double distance(complex<double> z1, complex<double> z2);
    bool availableTurn();

};


#endif // MOBIUS_H
