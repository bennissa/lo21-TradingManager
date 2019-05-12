#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMainWindow>
#include <QSplitter>
#include "tradingmanager.h"
#include "devises.h"
#include "evolutionviewer.h"
#include "texteditor.h"

int main(int argc, char * argv[]) {

    // Application
    QApplication app(argc, argv);
    QMainWindow window;

    // Traduction des boutons par défaut de QT (https://stackoverflow.com/questions/31533019/how-to-translate-the-buttons-in-qmessagebox)
    QLocale curLocale(QLocale("fr_FR"));
    QLocale::setDefault(curLocale);
    qDebug() << QLocale().name();
    QTranslator qtTranslator;
    if(qtTranslator.load(curLocale, "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
        qDebug() << "qtTranslator ok";
        app.installTranslator(&qtTranslator);
    }
    QTranslator qtBaseTranslator;
    if(qtBaseTranslator.load("qtbase_" + curLocale.name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
        qDebug() << "qtBaseTranslator ok";
        app.installTranslator(&qtBaseTranslator);
    }

    // Initialisation DevisesManager
    TradingManager& manager = TradingManager::getInstance();

    // Création devises
    manager.getDManager().creationDevise("USD", "Dollar Américain","USA");
    manager.getDManager().creationDevise("BTC", "BitCoin","monnaie virtuelle");

    // Création paire et évolution BTC/USD
    const PaireDevises& BTC_USD=manager.getDManager().getPaireDevises("BTC", "USD");
    EvolutionCours e(BTC_USD);

    // Une bougie = Un mois pour bitcoin entre janvier 2017 et août 2018
    e.addCours(4261.48000000,4745.42000000,3400.00000000,4724.89000000,QDate(2017,1,1));
    e.addCours(4689.89000000,4939.19000000,2817.00000000,4378.51000000,QDate(2017,2,1));
    e.addCours(4378.49000000,6498.01000000,4110.00000000,6463.00000000,QDate(2017,3,1));
    e.addCours(6463.00000000,11300.03000000,5325.01000000,9838.96000000,QDate(2017,4,1));
    e.addCours(9837.00000000,19798.68000000,9380.00000000,13716.36000000,QDate(2017,5,1));
    e.addCours(13715.65000000,17176.24000000,9035.00000000,10285.10000000,QDate(2017,6,1));
    e.addCours(10285.10000000,11786.01000000,6000.01000000,10326.76000000,QDate(2017,7,1));
    e.addCours(10325.64000000,11710.00000000,6600.10000000,6923.91000000,QDate(2017,8,1));
    e.addCours(6922.00000000,9759.82000000,6430.00000000,9246.01000000,QDate(2017,9,1));
    e.addCours(9246.01000000,10020.00000000,7032.95000000,7485.01000000,QDate(2017,10,1));
    e.addCours(7485.01000000,7786.69000000,5750.00000000,6390.07000000,QDate(2017,11,1));
    e.addCours(6391.08000000,8491.77000000,6070.00000000,7730.93000000,QDate(2017,12,1));
    e.addCours(7735.67000000,7750.00000000,5880.00000000,7011.21000000,QDate(2018,1,1));
    e.addCours(7011.21000000,7410.00000000,6111.00000000,6626.57000000,QDate(2018,2,1));
    e.addCours(6626.57000000,7680.00000000,6205.00000000,6371.93000000,QDate(2018,3,1));
    e.addCours(6369.52000000,6615.15000000,3652.66000000,4041.32000000,QDate(2018,4,1));
    e.addCours(4041.27000000,4312.99000000,3156.26000000,3702.90000000,QDate(2018,5,1));
    e.addCours(3701.23000000,4069.80000000,3349.92000000,3434.10000000,QDate(2018,6,1));
    e.addCours(3434.10000000,4198.00000000,3373.10000000,3813.69000000,QDate(2018,7,1));
    e.addCours(3814.26000000,4140.00000000,3670.69000000,4106.65000000,QDate(2018,8,1));

    // Initialisation modules
    EvolutionViewer v(e);

    // Splitter principal
    QSplitter splitter;
    splitter.setStretchFactor(0, 0);
    splitter.setStretchFactor(1, 0);
    splitter.addWidget(&v);
    splitter.addWidget(&manager.getTEditor());

    // Affichage
    window.resize(1600, 900);
    window.setCentralWidget(&splitter);
    window.show();

    return app.exec();
}
