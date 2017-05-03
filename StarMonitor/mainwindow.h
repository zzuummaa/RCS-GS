#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "telemetryplot.h"
#include "shm/redis.h"
#include "realtimetelemetryloader.h"
#include "telemetry.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_plotButton_clicked();
    void repaintGraph();

    void on_comboBox_currentIndexChanged(int index);

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    redisDataService* dserv;
    telemetryPlot* plott;

    QTimer* animationTimer;
    QVector<double> x;
    QVector<double> y;
    int i;

    realTimeTelemetryLoader* rtLoader;

    bool isInitedMainWindow;

    void fillingListWidget();
signals:
    void resizeEvent ( QResizeEvent * e);
};

#endif // MAINWINDOW_H
