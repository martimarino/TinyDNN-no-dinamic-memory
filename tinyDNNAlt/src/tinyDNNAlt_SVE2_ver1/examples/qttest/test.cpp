#define QT_BOARD
#include <QApplication>
#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QTimer>
#include "tiny_dnn/tiny_dnn.h"
using namespace QtCharts;

QLineSeries *accuracy;
QChart* chart;
std::vector<std::thread*> threadPool;
bool running = true;
void updateChart() {
	QValueAxis *xa = new QValueAxis();
    xa->setRange(0,accuracy->count());
    xa->setTickInterval(1.0f);
    xa->setLabelFormat("d");
    chart->setAxisX(xa,accuracy);
    std::string title = "Train performances - Epoch: "+std::to_string(accuracy->count());
    chart->setTitle(QString::fromStdString(title));
}

void fetcher(tinyboard::Broker* b) {
	int i = 0;
	while(true) {
		float f=b->fetch(tinyboard::topics[0]);
		*accuracy << QPointF(++i,f);
	}
}


int gui(int argc,char* argv[]) {
    QApplication a(argc, argv);

    accuracy = new QLineSeries();
    accuracy->append(0,0);
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(accuracy);
    chart->createDefaultAxes();
    chart->setTitle("Train performances - Epoch: ");

    QValueAxis *ya = new QValueAxis();
    ya->setRange(0,1);
    chart->setAxisY(ya,accuracy);

    updateChart();


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();


    QTimer *timer = new QTimer();
    window.connect(timer, &QTimer::timeout,[] {updateChart();});
    timer->start(1000);
	tinyboard::Broker b;
	threadPool.push_back(new std::thread(&fetcher,&b));
    return a.exec();	
}

int main(int argc, char *argv[])
{	
	gui(argc,argv);
}