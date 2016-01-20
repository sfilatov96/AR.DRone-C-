
#include "paintscene.h"
#include "paint.h"

bool paintScene::already_drown = 0;

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{

}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{   if (!already_drown) {
        addEllipse(event->scenePos().x(),
                   event->scenePos().y(),
                   3,
                   3,
                   QPen(Qt::NoPen),
                   QBrush(Qt::black));

        previousPoint = event->scenePos();
        emit mousePressed();
        already_drown = 1;
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::black,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

    previousPoint = event->scenePos();
    emit mouseMoved();
}

void paintScene::selectPoint(int pos, point_t *points) {
    addEllipse(points[pos].x - 5,
               points[pos].y - 3,
               10,
               10,
               QPen(Qt::NoPen),
               QBrush(Qt::red));
}
