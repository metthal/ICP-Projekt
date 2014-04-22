#include "gui/zoomgraphicsview.h"

#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

/**
* Sets up the subclassed QGraphicsView
*/
ZoomGraphicsView::ZoomGraphicsView(QWidget* parent) : QGraphicsView(parent) {

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
    QGraphicsScene* Scene = new QGraphicsScene(this);
    setScene(Scene);

    //Populate the scene
    for(int x = 0; x < 1000; x = x + 25) {
        for(int y = 0; y < 1000; y = y + 25) {

            if(x % 100 == 0 && y % 100 == 0) {
                Scene->addRect(x, y, 2, 2);

                QString pointString;
                QTextStream stream(&pointString);
                stream << "(" << x << "," << y << ")";
                QGraphicsTextItem* item = Scene->addText(pointString);
                item->setPos(x, y);
            } else {
                Scene->addRect(x, y, 1, 1);
            }
        }
    }

    //Use ScrollHand Drag Mode to enable Panning
    setDragMode(ScrollHandDrag);

    zoom = 0;
}

/**
  * Zoom the view in and out.
  */
void ZoomGraphicsView::wheelEvent(QWheelEvent* event) {

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.25;
    if(event->delta() > 0) {
        // Zoom in
        if (zoom < maxZoom)
        {
            scale(scaleFactor, scaleFactor);
            zoom++;
        }
    } else {
        // Zooming out
        if (zoom > -maxZoom)
        {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            zoom--;
        }
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}
