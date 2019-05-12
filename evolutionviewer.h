#ifndef _EVOLUTIONVIEWER_H
#define _EVOLUTIONVIEWER_H

#include <QWidget>
#include <QtCharts>
#include <QDate>
#include <QChart>
#include <QChartView>
#include <QCandlestickSeries>
#include "devises.h"

class Bougie : public QCandlestickSet {
        Q_OBJECT
        CoursOHLC * cours;
    public:
        Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC * c, qreal timestamp = 0.0, QObject*parent = nullptr);
        CoursOHLC& getCoursOHLC() { return *cours; }
        const CoursOHLC& getCoursOHLC() const { return *cours; }
    signals:
    private slots:
};
class EvolutionViewer : public QWidget{
        Q_OBJECT
        EvolutionCours& evolution;
        QCandlestickSeries * series; // un ensemble de bougies
        QChart* chart; // un graphique sur un ensemble de bougies
        QChartView* chartView; // un viewer de graphique
    public:
        explicit EvolutionViewer(EvolutionCours& e,QWidget* parent = nullptr);
    signals:
    private slots:
    public slots:
};

#endif
