#ifndef ZOOMGRAPHICSVIEW_H
#define ZOOMGRAPHICSVIEW_H

#include <QGraphicsView>

class ZoomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    ZoomGraphicsView(QWidget* parent = NULL);

protected:

    //Take over the interaction
    virtual void wheelEvent(QWheelEvent* event);

    const int maxZoom = 5;
    int zoom;
};

#endif // ZOOMGRAPHICSVIEW_H
