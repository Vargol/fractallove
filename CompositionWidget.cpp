#include <QPainter>
#include "CompositionWidget.h"


CompositionWidget::CompositionWidget(FractalController *fc)
{
    _fc = fc;
}


void CompositionWidget::paintEvent(QPaintEvent *event) {

    QPainter imagePainter(this);
    _fc->composeImages(imagePainter);
    imagePainter.end();

}


void CompositionWidget::resizeEvent(QResizeEvent *event) {

    QSize size = event->size();
    _fc->setViewPortSize(size.width(), size.height());
}

