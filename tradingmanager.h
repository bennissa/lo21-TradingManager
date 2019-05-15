#ifndef TRADINGMANAGER_H
#define TRADINGMANAGER_H

#include "devises.h"
#include "texteditor.h"

class Simulation;

class TradingException{
    public:
        TradingException(const QString& message):info(message){}
        QString getInfo() const{ return info; }
    private:
        QString info;
};

class Transaction{
    Simulation* sim;
    CoursOHLC* cours;
    double montant;
    // Constructeur de recopie et opérateur d'affections ? mettre en privé et laisser addTransaction faire le travail ou permettre en les définissant ?
    public:
    Transaction(Simulation* s, CoursOHLC* c, double montant);
    double getMontant() const{ return montant; }
    CoursOHLC* getCours() const{ return cours; }
};

class Strategy{

};

class Simulation{
    QString filename;
    QDate debutFichier, finFichier;
    EvolutionCours* evolution=nullptr;
    double broker;
    double soldeBase, soldeContrepartie;
    Transaction** transactions=nullptr;
    unsigned int nbTransactions=0;
    unsigned int nbMaxTransactions=0;
    QDate debut, fin;

    // Constructeur de recopie et opérateur d'affections ? mettre en privé et laisser addSimulation faire le travail ou permettre en les définissant ?

    public:
    ~Simulation(){
        for(unsigned int i=0;i<nbTransactions;i++){
            delete transactions[i];
        }
        delete transactions;
    }
    // enlever dates de constructeur
    Simulation(QString file, double base, double contrepartie, double b):filename(file),soldeBase(base),soldeContrepartie(contrepartie),broker(b){}
    Transaction* addTransaction(double montant, CoursOHLC* c);
    void cancelLastTransaction();

    EvolutionCours* lireFichier();
    double getSoldeBase() const{ return soldeBase; }
    double getSoldeContrepartie() const{ return soldeContrepartie; }
    double getBroker() const{ return broker; }
    void addToSoldeBase(double montant){ soldeBase+=montant; soldeBase*=(1-broker);  return; }
    void addToSoldeContrepartie(double montant){ soldeContrepartie+=montant; soldeContrepartie*=(1-broker); return; }
    void run();
};

class SimulationAuto : public Simulation{
    Strategy* strat;
    public:
    SimulationAuto(QString file, double base, double contrepartie, double b, QString s);
};

class TradingManager{
    DevisesManager& devisesManager=DevisesManager::getManager();
    TextEditor& textEditor=TextEditor::getEditor();

    Simulation** sim;
    unsigned int nbSim=0;
    unsigned int nbMaxSim=0;

    bool PasAPas=false;

    // Interdiction de recopie ou d'affecter
    TradingManager(const TradingManager& m) = delete;
    TradingManager& operator=(const TradingManager& m) = delete;

    // Singleton
    TradingManager(){}
    ~TradingManager(){
        devisesManager.libererManager();
        textEditor.libererEditor();
        for(unsigned int i=0;i<nbSim;i++){
            delete sim[i];
        }
        delete sim;
    }
    struct Handler{
        TradingManager* instance = nullptr;
        ~Handler(){ delete instance; }
    };
    static Handler handler;

    public:

    static TradingManager& getInstance() {
        if(handler.instance == nullptr)
            handler.instance = new TradingManager;
        return *handler.instance;
    }
    static void libererInstance() {
        delete handler.instance;  handler.instance = nullptr;
    }

    DevisesManager& getDManager() const{ return devisesManager; }
    TextEditor& getTEditor() const{ return textEditor; }

    Simulation* addSimulation(double base, double contrepartie, QString filename, double broker, bool automatic=false, QString strat=QString());
    void closeSimulation(Simulation* s);
};

#endif // TRADINGMANAGER_H
