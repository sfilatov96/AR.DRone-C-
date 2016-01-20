#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>

struct point_t {
    int x;
    int y;
};

class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    static bool already_drown;
    explicit paintScene(QObject *parent = 0);
    QPointF     previousPoint;
    ~paintScene();

signals:
    void mouseMoved();
    void mousePressed();


private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void selectPoint(int, point_t *);
};

#endif
