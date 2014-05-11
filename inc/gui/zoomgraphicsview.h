/*
* Project name:
* Bludiste 2014
*
* Description:
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
*
* Project's GitHub repository:
* https://github.com/metthal/ICP-Projekt
*
* Team:
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

#ifndef ZOOMGRAPHICSVIEW_H
#define ZOOMGRAPHICSVIEW_H

#include <QGraphicsView>

class ZoomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    /**
     * Creates instance of ZoomGraphicsView that is extending
     * graphics view with zoom support (mouse wheel)
     */
    ZoomGraphicsView(QWidget* parent = NULL);

protected:

    /**
     * Takes over interaction with mouse wheel and uses it to
     * zoom in / out.
     */
    virtual void wheelEvent(QWheelEvent* event);

    const int maxZoom = 5;
    int zoom;
};

#endif // ZOOMGRAPHICSVIEW_H
