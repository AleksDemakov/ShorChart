#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <random>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <iostream>

#include <QtCharts/QChart>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace QtCharts;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QChart* drawChart(QLineSeries *, const int);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
