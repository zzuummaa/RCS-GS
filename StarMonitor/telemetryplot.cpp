#include "telemetryplot.h"
#include <QObject>
#include <QLoggingCategory>

QLoggingCategory telplot_category("Telemetry plot");

void telemetryPlot::repaintGraph() {

    if (telloader != NULL) {
        tel_pair src = telloader->retreive();

        if ( !src.first.isEmpty() ) {
            qDebug(telplot_category) << "new point: x=" << (int)src.first.front() << "y=" << (int)src.second.front();
        }

        copyWithRemove(&x, &(src.first) );
        copyWithRemove(&y, &(src.second) );
    } else {
        qDebug(telplot_category) << "telemetryLoader is NULL";
        exit(1);
    }

    if (x.isEmpty()) return;

    plot->graph(0)->setData(x, y);
    //end of playback check
    if(x.size() > 100) animationTimer->stop();
    //

    //Установим область, которая будет показываться на графике
    plot->xAxis->setRange(x[0], x.back());//Для оси Ox

    //Для показа границ по оси Oy сложнее, так как надо по правильному
    //вычислить минимальное и максимальное значение в векторах
    double minY = y[0], maxY = y[0];
    for (int i=1; i<x.size(); i++)
    {
        if (y[i]<minY) minY = y[i];
        if (y[i]>maxY) maxY = y[i];
    }
    plot->yAxis->setRange(minY, maxY);//Для оси Oy

    plot->replot();
}

telemetryPlot::telemetryPlot(QWidget* parent, QCustomPlot* plot)
{
    this->telloader = NULL;
    this->plot = plot;
    this->i    = 0;

    animationTimer = new QTimer(parent); //создание экземпляра таймера
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(repaintGraph()) ); //привязка события к функции Play

    this->plot->addGraph();
}

telemetryPlot::~telemetryPlot() {
    delete animationTimer;
}

void telemetryPlot::start(int ms)
{
    animationTimer->start(ms);
}

void telemetryPlot::setTelemetryLoader(realTimeTelemetryLoader *telloader)
{
    this->telloader = telloader;
}
