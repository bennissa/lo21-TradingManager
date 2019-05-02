#ifndef _EVOLUTIONVIEWER_H
#define _EVOLUTIONVIEWER_H
#include <QWidget>
#include <QtCharts>
#include <QDate>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QChart>
#include <QChartView>
#include <QCandlestickSeries>
#include "trading.h"

class Bougie : public QCandlestickSet {
        Q_OBJECT
        CoursOHLC * cours;
    public:
        Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC * c, qreal timestamp = 0.0, QObject*parent = nullptr);
        CoursOHLC& getCoursOHLC() { return *cours; }
        const CoursOHLC& getCoursOHLC() const { return *cours; }
    signals:
        void clickBougie(Bougie * c);
    private slots:
        void viewCoursOHLC(){ emit clickBougie(this); }
};
class EvolutionViewer : public QWidget{
        Q_OBJECT
        EvolutionCours& evolution;
        QCandlestickSeries * series; // un ensemble de bougies
        QChart* chart; // un graphique sur un ensemble de bougies
        QChartView* chartView; // un viewer de graphique
        QLineEdit * open; // barres d’édition
        QLineEdit * high;
        QLineEdit * low;
        QLineEdit * close;
        QLabel * openl; // labels
        QLabel * highl;
        QLabel * lowl;
        QLabel * closel;
        QPushButton * save; // bouton
        QHBoxLayout * copen; // couches de placement
        QHBoxLayout * chigh;
        QHBoxLayout * clow;
        QHBoxLayout * cclose;
        QVBoxLayout * coucheCours;
        QHBoxLayout * fenetre;

        Bougie* lastBougieClicked=nullptr;

    public:
        explicit EvolutionViewer(EvolutionCours& e,QWidget* parent = nullptr);
    signals:
    private slots:
        void saveCoursOHLC();
        void viewCoursOHLC(Bougie* b);
    public slots:
};

#endif
