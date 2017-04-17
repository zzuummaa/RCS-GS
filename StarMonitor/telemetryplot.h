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

    void start(int ms);
    void setTelemetryLoader(realTimeTelemetryLoader* telloader);

public slots:
    void repaintGraph();

private:
    QCustomPlot* plot;
    QTimer* animationTimer;
    realTimeTelemetryLoader* telloader;

    int i;

    QVector<double> x;
    QVector<double> y;
};

#endif // TELEMETRYPLOT_H
