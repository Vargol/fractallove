#ifndef COMPOSITIONWIDGET_H
#define COMPOSITIONWIDGET_H

#include <QResizeEvent>
#include <QWidget>
#include "FractalController.h"

class CompositionWidget : public QWidget
{
public:
    CompositionWidget(FractalController *fc);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

private:
    FractalController *_fc;
    virtual void paintEvent(QPaintEvent *event);

};

#endif // COMPOSITIONWIDGET_H
