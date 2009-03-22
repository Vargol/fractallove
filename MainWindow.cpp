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

        _fractals = new QComboBox;

        _fractals->addItem(tr("MandelBrot"));
        _fractals->addItem(tr("MandelbrotInter"));
        _fractals->addItem(tr("Buddhabrot"));
        _fractals->addItem(tr("Buddhabrot Julia"));

        vbox->addWidget(_fractals);

	gb = new QGroupBox (tr("Fractal"));

        _fc.setFractalGroupBox(gb);

	vbox->addWidget(gb);

        _preview = new QPushButton(tr("Preview"));

        vbox->addWidget(_preview);

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
	

        QObject::connect(_fractals, SIGNAL(currentIndexChanged(const QString &)),
                                         &_fc, SLOT(setFractalClass(const QString &)));

        QObject::connect(_preview, SIGNAL(clicked()),
                                         &_fc, SLOT(renderToScreen()));

        QObject::connect(&_fc, SIGNAL(redrawFractal()),
                                         _compWidget, SLOT(update()));
//        QObject::connect(_xOffsetSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setXOffset(double)));
//        QObject::connect(_yOffsetSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setYOffset(double)));
//        QObject::connect(_zoomSpinBox, SIGNAL(valueChanged(double)), &_fc, SLOT(setZoom(double)));
    }
