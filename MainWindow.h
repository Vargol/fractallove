/*
 *  MainWindow.h
 *  
 *
 *  Created by David Burnett on 22/04/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "CompositionWidget.h"
#include "FractalController.h"

class MainWindow : public QWidget
	{
//		Q_OBJECT
		
	public:
                MainWindow();
                void resize(int, int);
		
	private:
                CompositionWidget *_compWidget;
                FractalController _fc;
                QComboBox *_fractals;
                QDoubleSpinBox *_xOffsetSpinBox;
                QDoubleSpinBox *_yOffsetSpinBox;
                QDoubleSpinBox *_zoomSpinBox;
                QPushButton *_preview;

		void connectSignalAndSlots();
};

#endif
