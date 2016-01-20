#include "paint.h"
#include "paintscene.h"
#include "ui_paint.h"
#include <fstream>
#include <unistd.h>
#include "move.h"

point_t *points = new point_t[100];
int points_count = 0;
int useful_points_count = 0;

paint::paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::paint)
{
    ui->setupUi(this);

    scene = new paintScene();
    ui->graphicsView->setScene(scene);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &paint::slotTimer);
    timer->start(100);

    connect(scene, SIGNAL(mouseMoved()), this, SLOT(readCoordinates()));
    connect(ui->StartButton, SIGNAL(clicked()), this, SLOT(printCoordinates()));
    connect(ui->ClearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(pointReached(int, point_t *)), scene, SLOT(selectPoint(int, point_t *)));

}



paint::~paint()
{
    delete [] points;
    delete scene;
    delete timer;
    delete ui;
}

void paint::pointReachEvent(int pos) {
    emit pointReached(pos, points);
}

void paint::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void paint::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void paint::readCoordinates() {
    point_t point;
    point.x = scene->previousPoint.x();
    point.y = scene->previousPoint.y();
    points_count++;
    if (points_count % 9 == 0 || points_count == 1) {
        points[useful_points_count] = point;
        useful_points_count++;
    }
}

void paint::printCoordinates() {
    std::ofstream fout ("points.txt");
    fout << useful_points_count << std::endl;
    for (int pos = 0; pos < useful_points_count; pos++) {
        fout << points[pos].x << ' ' << points[pos].y << std::endl;
    }
    fout.close();
    emit pointReached(0, points);

    action(this);

    for (int pos = 0; pos < useful_points_count; pos++) {
         ui->plainTextEdit->appendPlainText(QString::number(pos) + ' ' + QString::number(points[pos].x) + ',' + QString::number(points[pos].y));
    }
}

void paint::clear() {
    scene->clear();
    ui->plainTextEdit->clear();
    points_count = 0;
    useful_points_count = 0;
    paintScene::already_drown = 0;
}

