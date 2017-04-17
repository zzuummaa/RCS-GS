#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <chrono>
#include "telemetryloader.h"

#define count_ 100

QLoggingCategory redis_category("Redis client");

bool parseFunc(string* data, double* outVal, int ms) {
    *outVal = atoi(data->c_str());
    return true;
}

void MainWindow::repaintGraph() {
    x.push_back(i);
    y.push_back(i * i);
    i++;

    ui->plot->graph(0)->setData(x, y);
    //end of playback check
    if(x.size() > 100) animationTimer->stop();
    //
    ui->plot->replot();
}

bool filling_db(redisDataService* dserv) {
    auto begin = chrono::high_resolution_clock::now();

    for (int i = 0; i < count_; i++) {
        string data = to_string((double)i * i);

        req_s req;
        req.type = 0;
        req.sec = i * 2;
        req.ms = 0;

        if ( !dserv->add(&req, &data) ) {
            qDebug(redis_category) << "redisDataService add failed";
            return false;
        }
    }

    auto end = chrono::high_resolution_clock::now();

    int diff = chrono::duration_cast<chrono::milliseconds>(end-begin).count();
    qDebug(redis_category) << "redisDataService add" << count_ << "elements time:" << diff << "ms";

    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dserv = new redisDataService();

    try {
        dserv->connect();
        qDebug(redis_category) << "redisDataService connected";
    } catch(const unable_to_connect&) {
        qDebug(redis_category) << "redisDataService unable to connect";
    }

    /*if ( !filling_db(dserv) ) {
        exit(1);
    }*/

    rtLoader = new realTimeTelemetryLoader(dserv);
    rtLoader->setParseValFunc(&parseFunc);
    rtLoader->start();

    plott = new telemetryPlot(this, ui->plot);
    plott->setTelemetryLoader(rtLoader);
    plott->start(200);

}

MainWindow::~MainWindow()
{
    dserv->disconnect();

    delete dserv;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    exit(0);
}

void MainWindow::on_plotButton_clicked()
{
    int diff;
    auto begin = chrono::high_resolution_clock::now();

    telemetryLoader* tl = new telemetryLoader(dserv);
    tl->setType(0);
    tl->setParseValFunc(&parseFunc);

    int elem_count = 50;
    int step = /*(count_ * 1000 - 0) / elem_count*/ 3 * 1000;
    tel_pair telp = tl->loadVals(0, count_ * 1000, step);

    if (telp.first.isEmpty()) {
        qDebug(redis_category) << "loadVals error reading data";
        return;
    }

    QVector<double> x = telp.first;
    QVector<double> y = telp.second;

    auto end = chrono::high_resolution_clock::now();

    diff = chrono::duration_cast<chrono::milliseconds>(end-begin).count();
    qDebug(redis_category) << "Loaded from redis" << x.size() << "elements. Waisted time:" << diff << "ms";

    begin = chrono::high_resolution_clock::now();

    //Рисуем график y=x*x

    //Сгенерируем данные
    //Для этого создадим два массива точек:
    //один для созранения x координат точек,
    //а второй для y соответственно

    /*double a = -10; //Начало интервала, где рисуем график по оси Ox
    double b =  10; //Конец интервала, где рисуем график по оси Ox
    double h = 0.01; //Шаг, с которым будем пробегать по оси Ox

    int N=(b-a)/h + 2; //Вычисляем количество точек, которые будем отрисовывать
    QVector<double> x(N), y(N); //Массивы координат точек

    //Вычисляем наши данные
    int i=0;
    for (double X=a; X<=b; X+=h)//Пробегаем по всем точкам
    {
        x[i] = X;
        y[i] = X*X;//Формула нашей функции
        i++;
    }*/

    ui->plot->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
    ui->plot->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->plot->graph(0)->setData(x, y);

    //Подписываем оси Ox и Oy
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");

    //Установим область, которая будет показываться на графике
    ui->plot->xAxis->setRange(x[0], x.back());//Для оси Ox

    //Для показа границ по оси Oy сложнее, так как надо по правильному
    //вычислить минимальное и максимальное значение в векторах
    double minY = y[0], maxY = y[0];
    for (int i=1; i<x.size(); i++)
    {
        if (y[i]<minY) minY = y[i];
        if (y[i]>maxY) maxY = y[i];
    }
    ui->plot->yAxis->setRange(minY, maxY);//Для оси Oy

    //И перерисуем график на нашем widget
    ui->plot->replot();

    end = chrono::high_resolution_clock::now();

    diff = chrono::duration_cast<chrono::milliseconds>(end-begin).count();
    qDebug(redis_category) << "Ploted " << x.size() << " elements. Waisted time:" << diff << "ms";
}
