#include "mainwindow.h"
#include "ui_mainwindow.h"



int to_int(const std::vector<bool> &v, const int n) {
    int cur = 0;
    int st = 1;
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        if ( v[i] )
            cur += (1 << (n - i - 1));
    }
    return cur;
}
std::vector<std::pair<int, double>> f(){
    using namespace std;
    const int MOD = 15;
    const int p = 2;

    int n = ceil(log2(MOD));

    vector< vector< bool > > Mf( (1 << (n + n)), vector< bool > (n + n) );

    int cur = 1;

    // for (int i = 0; i < 10; i++) {
    //     cout << i << " " << cur << endl;;
    //     cur *= p;
    //     cur %= MOD;
    // }
    // cur = 1;

    for (int x = 0; x < (1 << n); x++) {

        for (int pos = 0; pos < n; pos++) {
            Mf[x][ n - pos - 1 ] = ((cur & (1 << pos)) >> pos);
        }
        cur = cur * p % MOD;
    }
    const int x_mask = (1 << n) - 1;
    for (int x = (1 << n); x < (1 << n + n); x++) {
        for (int pos = 0; pos < n; pos++) {
            Mf[x][n - pos - 1] = Mf[x & x_mask][n - pos - 1] ^ ( (x & (1 << n + pos)) >> (n + pos) );
        }
    }

    for (int x = 0; x < (1 << n + n); x++) {
        for (int pos = 0; pos < n; pos++) {
            Mf[x][n * 2 - pos - 1] = ((x & (1 << pos)) >> pos);
        }
    }

    // int tmp = 0;
    // for (int i = 0; i < (1 << n + n); i++) {
    //     tmp++;
    //     cout << bitset<4>(i).to_string() << " : ";
    //     for (int j = 0; j < n+n; j++) {
    //         cout << Mf[i][j];
    //     }
    //     cout << " " << to_int(Mf[i], n + n);
    //     cout << endl;
    //     if (tmp == (1 << n)) {
    //         cout << endl;
    //         tmp = 0;
    //     }
    // }

    // return 0;



    // phi1
    vector< bool > f1(1 << (n + n));
    for (int i = 0; i < (1 << (n + n - 1)); i++) {
        f1[i] = 1;
    }



    //phi2
    vector< bool > f2(1 << (n + n));
    for (int i = 0; i < (1 << (n + n)); i++) {
        f2[i] = f1[ to_int(Mf[i], n + n) ];
    }


    const double pi2 = acos(-1) * 2;

    vector< double > cos_v(1 << (n + n));
    for (int block = 0; block < (1 << n); block++) {
        for (int i = 0; i < (1 << n); i++) {
            for (int j = 0; j < (1 << n); j++) {
                cos_v[ block * (1 << n) + i ] += cos(pi2 * i * j / (1 << n)) * f2[ block * (1 << n) + j ];
            }
        }
    }

    vector< double > sin_v(1 << (n + n));
    for (int block = 0; block < (1 << n); block++) {
        for (int i = 0; i < (1 << n); i++) {
            for (int j = 0; j < (1 << n); j++) {
                sin_v[ block * (1 << n) + i ] += sin(pi2 * i * j / (1 << n)) * f2[ block * (1 << n) + j ];
            }
        }
    }


    vector< double > res((1 << n + n));

    for (int i = 0; i < (1 << n + n); i++) {
        res[i] = cos_v[i] * cos_v[i] + sin_v[i] * sin_v[i];
    }

    // for (int i = 0; i < 20; i++) {
    //     cout << i << ":" << Mf[i][0] << Mf[i][1] << Mf[i][2] << Mf[i][3] << "\n";
    // }
    vector<pair<int, double>> ans;
    for (int i = 0; i < 200; i++) {
//        cout << i << " " << res[i] << "\n";
        ans.push_back({i,res[i]});
    }
    return ans;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const int n=30;
    QLineSeries* series = new QLineSeries();
//    QFile file("input.txt");
//    try {
//        file.open(QIODevice::ReadOnly);
//    } catch (...) {
//        qDebug()<<"not open";
//        return;
//    }
//    QTextStream in(&file);

//    char c;
//    for(int i=0,x;i<n;i++){
//        double y;
//        in>>x>>c>>y;
//        qDebug()<<x<<" "<<y<<Qt::endl;
//        series->append(x, y);
//    }
    auto res = f();
    for(int i=0;i<n;i++)
        series->append(res[i].first, res[i].second);
    QChartView * chartView;

    chartView = new QChartView(drawChart(series, n));
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget( chartView );

}
QChart * MainWindow::drawChart(QLineSeries *series, const int n){
    series->setName("");
    series->setPen( QColor("blue") );

    QChart * chart = new QChart();

    //title
    chart->setTitleFont( QFont("Helvetica [Cronyx]", 10, QFont::Bold) );
    chart->setTitle("N = " + QString::number(15) + "    a="+QString::number(2));

    QValueAxis * axisX = new QValueAxis();
    axisX->setTickCount(n);
    QValueAxis * axisY = new QValueAxis();

    axisX->setTitleText("order");
    axisY->setTitleText("probability");

    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();

    axisX->setLabelFormat("%d");

    chart->addSeries( series );

    chart->legend()->setFont( QFont("Helvetica [Cronyx]", 9) );
    chart->legend()->setMarkerShape( QLegend::MarkerShapeFromSeries );

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    return chart;
}
MainWindow::~MainWindow()
{
    delete ui;
}

