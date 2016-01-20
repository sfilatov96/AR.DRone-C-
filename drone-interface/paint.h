#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QTimer>
#include <QResizeEvent>

#include <paintscene.h>

namespace Ui {
class paint;
}

class paint : public QWidget
{
    Q_OBJECT

public:
    explicit paint(QWidget *parent = 0);
    Ui::paint *ui;
    ~paint();
    void pointReachEvent(int);

private:
    QTimer *timer;
    paintScene *scene;

private:
    void resizeEvent(QResizeEvent * event);

signals:
    void pointReached(int, point_t*);

private slots:
    void slotTimer();
    void readCoordinates();
    void printCoordinates();
    void clear();
};

#endif

