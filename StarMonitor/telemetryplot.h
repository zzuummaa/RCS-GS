#ifndef TELEMETRYPLOT_H
#define TELEMETRYPLOT_H

#include "qcustomplot.h"
#include <QObject>
#include "realtimetelemetryloader.h"

class telemetryPlot : private QObject
{
    Q_OBJECT
public:
    telemetryPlot(QWidget* parent, QCustomPlot* plot);
    ~telemetryPlot();

    void start();
    void start(int ms);
    void pause();

    void setTelemetryLoader(telemetryLoader* telloader);
    telemetryLoader* getTelemetryLoader();

public slots:
    void repaintGraph();

private:
    QCustomPlot* plot;
    QTimer* animationTimer;
    telemetryLoader* telloader;

    int i;
    int start_ms;

    QVector<double> x;
    QVector<double> y;
};

#endif // TELEMETRYPLOT_H
