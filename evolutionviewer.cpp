#include "evolutionviewer.h"

EvolutionViewer::EvolutionViewer(EvolutionCours& e,QWidget* parent): QWidget(parent), evolution(e){
    // Construction graphe
    // Création serie de bougies
    series = new QCandlestickSeries;
    series->setName(evolution.getPaireDevises().toString());
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));

    // Remplissage
    QStringList categories;
    for(EvolutionCours::iterator it=evolution.begin();it!=evolution.end();++it){
        CoursOHLC& cours = *it;
        Bougie* bougie = new Bougie(cours.getOpen(), cours.getHigh(), cours.getLow(), cours.getClose(), &cours, 0.0, this);
        categories << cours.getDate().toString("MM.yyyy");
        series->append(bougie);
    }

    // Création Chart
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Cours OHLC");

    // Créations axes
    chart->createDefaultAxes();
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->setAxisX(axisX, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart, this);
    chartView->setFixedSize(1200, 900);
}

Bougie::Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC * c, qreal timestamp, QObject*parent):QCandlestickSet(open, high, low, close, timestamp, parent), cours(c){}
