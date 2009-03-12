/*
 *  MainWindow.cpp
 *  
 *
 *  Created by David Burnett on 22/04/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "MainWindow.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(void) : QWidget(NULL) {


	QHBoxLayout *hbox = new QHBoxLayout;
	QVBoxLayout *vbox = new QVBoxLayout;
	
        _compWidget = new CompositionWidget(&_fc);


        _compWidget->setMinimumSize(512,512);

        hbox->addWidget(_compWidget);
	
	vbox->setSpacing(16);

	
	QGroupBox *gb = new QGroupBox (tr("Image"));

	gb->setGeometry(QRect( 400,  0,  160, 80 ));
	
	QGridLayout *grid = new QGridLayout;
	grid->addWidget(new QLabel(tr("Image width: ")), 0, 0);
	grid->addWidget(new QLabel(tr("Image height: ")), 1, 0);

	QDoubleSpinBox *dsb = new QDoubleSpinBox;
	dsb->setMinimum(-99.9);
	dsb->setDecimals(9);
	dsb->setDecimals(9);
	grid->addWidget(dsb, 0, 1);
	
	dsb = new QDoubleSpinBox;
	dsb->setMinimum(-99.9);
	dsb->setDecimals(9);	

        grid->addWidget(dsb, 1, 1);
	
//	grid->addItem(new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding),2,1);
	
	gb->setLayout(grid);
	vbox->addWidget(gb);
	
//	vbox->addSpacing(16);
	
	gb = new QGroupBox (tr("Fractal"));
	grid = new QGridLayout;

       _fractals = new QComboBox;

        _fractals->addItem(tr("MandelBrot"));
        _fractals->addItem(tr("MandelbrotInter"));
        _fractals->addItem(tr("Buddhabrot"));
        _fractals->addItem(tr("Buddhabrot Julia"));

        grid->addWidget(new QLabel(tr("Fractal: ")), 1,0);
        grid->addWidget(_fractals, 1,1);


	grid->addWidget(new QLabel(tr("Centre x: ")), 2, 0);
	grid->addWidget(new QLabel(tr("Centre y: ")), 3, 0);
	grid->addWidget(new QLabel(tr("Zoom: ")), 4, 0);

        _xOffsetSpinBox = new QDoubleSpinBox;
        _xOffsetSpinBox->setMinimum(-99.9);
        _xOffsetSpinBox->setDecimals(9);
        grid->addWidget(_xOffsetSpinBox, 2, 1);

        _yOffsetSpinBox = new QDoubleSpinBox;
        _yOffsetSpinBox->setMinimum(-99.9);
        _yOffsetSpinBox->setDecimals(9);
        grid->addWidget(_yOffsetSpinBox, 3, 1);

	

        _zoomSpinBox = new QDoubleSpinBox;
        _zoomSpinBox->setMinimum(-99.9);
        _zoomSpinBox->setDecimals(9);
        grid->addWidget(_zoomSpinBox, 4, 1);

        _preview = new QPushButton(tr("Preview"));

        grid->addWidget(_preview, 5, 1);

        gb->setLayout(grid);

	vbox->addWidget(gb);
	vbox->addItem(new QSpacerItem(1,400, QSizePolicy::Fixed, QSizePolicy::Expanding));
	
	hbox->addItem(vbox);

	setLayout(hbox);

        connectSignalAndSlots ();

//	resize(1000, 1000);

}

void MainWindow::resize(int width, int height)
{
	
	cout <<  "mw " << width << "," << height << endl;
	QWidget::resize(width, height);
}

void MainWindow::connectSignalAndSlots () {
	
//        QObject::connect(&_fc, SIGNAL(renderToScreen()),
//                                         &_fc, SLOT(renderToScreen()));

 //       QObject::connect(&_fc, SIGNAL(viewportResized(int, int)),
   //                                      &_fc, SLOT(setViewPortSize(int, int)));
	

        QObject::connect(_fractals, SIGNAL(currentIndexChanged(const QString &)),
                                         &_fc, SLOT(setFractalClass(const QString &)));

        QObject::connect(_preview, SIGNAL(clicked()),
                                         &_fc, SLOT(renderToScreen()));

        QObject::connect(&_fc, SIGNAL(redrawFractal()),
                                         _compWidget, SLOT(update()));
        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setXOffset(double)));
        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setYOffset(double)));
        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setZoom(double)));
    }
