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
        connect(bougie, SIGNAL(clickBougie(Bougie*)), this, SLOT(viewCoursOHLC(Bougie*)));
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
    chartView->setFixedHeight(900);
    chartView->setFixedWidth(1440);

    // Couche Cours
    // Layouts
    copen = new QHBoxLayout;
    chigh = new QHBoxLayout;
    clow = new QHBoxLayout;
    cclose = new QHBoxLayout;
    coucheCours = new QVBoxLayout;

    // Open
    openl = new QLabel("Open");
    open = new QLineEdit();
    open->setFixedWidth(100);
    copen->addWidget(openl);
    copen->addWidget(open);

    // High
    highl = new QLabel("High");
    high = new QLineEdit();
    high->setFixedWidth(100);
    chigh->addWidget(highl);
    chigh->addWidget(high);

    // Low
    lowl = new QLabel("Low");
    low = new QLineEdit();
    low->setFixedWidth(100);
    clow->addWidget(lowl);
    clow->addWidget(low);

    // Close
    closel = new QLabel("Close");
    close = new QLineEdit();
    close->setFixedWidth(100);
    cclose->addWidget(closel);
    cclose->addWidget(close);

    // Save
    save = new QPushButton("Save");
    save->setFixedWidth(140);
    //Connexion du signal clicked de save à celui de saveValues
    connect(save,SIGNAL(clicked()),this, SLOT(saveCoursOHLC()));

    // Construction layout vertical
    coucheCours->addLayout(copen);
    coucheCours->addLayout(chigh);
    coucheCours->addLayout(clow);
    coucheCours->addLayout(cclose);
    coucheCours->addWidget(save);

    // Construction fenetre
    fenetre = new QHBoxLayout;
    fenetre->addWidget(chartView);
    fenetre->addLayout(coucheCours);

    this->setLayout(fenetre);
}
void EvolutionViewer::viewCoursOHLC(Bougie* b){
    QString str;
    str.setNum(b->getCoursOHLC().getOpen());
    open->setText(str);
    str.setNum(b->getCoursOHLC().getHigh());
    high->setText(str);
    str.setNum(b->getCoursOHLC().getLow());
    low->setText(str);
    str.setNum(b->getCoursOHLC().getClose());
    close->setText(str);

    lastBougieClicked=b;
}
void EvolutionViewer::saveCoursOHLC(){
    double o=open->text().toDouble();
    double h=high->text().toDouble();
    double l=low->text().toDouble();
    double c=close->text().toDouble();

    lastBougieClicked->getCoursOHLC().setCours(o,h,l,c);

    lastBougieClicked->setOpen(o);
    lastBougieClicked->setHigh(h);
    lastBougieClicked->setLow(l);
    lastBougieClicked->setClose(c);
}

Bougie::Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC * c, qreal timestamp, QObject*parent):QCandlestickSet(open, high, low, close, timestamp, parent), cours(c){
    //Connexion du signal clicked de chaque bougie au slot viewCours de bougie qui émet un signal clickBougie
    connect(this,SIGNAL(clicked()),this, SLOT( viewCoursOHLC()));
}
