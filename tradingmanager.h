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

class Simulation{
    EvolutionCours* evolution;
    bool automatic=false;
    double soldeBase, soldeContrepartie;
    Transaction** transactions=nullptr;
    unsigned int nbTransactions=0;
    unsigned int nbMaxTransactions=0;
    QDate debut, fin;
    double broker;

    // Constructeur de recopie et opérateur d'affections ? mettre en privé et laisser addSimulation faire le travail ou permettre en les définissant ?

    public:
    ~Simulation(){
        for(unsigned int i=0;i<nbTransactions;i++){
            delete transactions[i];
        }
        delete transactions;
    }
    Simulation(EvolutionCours* evo, double base, double contrepartie, QDate d, QDate f, double b):evolution(evo),soldeBase(base),soldeContrepartie(contrepartie),debut(d),fin(f),broker(b){}
    Transaction* addTransaction(double montant, CoursOHLC* c);
    void cancelLastTransaction();

    EvolutionCours* getEvo() const{ return evolution; }
    double getSoldeBase() const{ return soldeBase; }
    double getSoldeContrepartie() const{ return soldeContrepartie; }
    double getBroker() const{ return broker; }
    void addToSoldeBase(double montant){ soldeBase+=montant; soldeBase*=(1-broker);  return; }
    void addToSoldeContrepartie(double montant){ soldeContrepartie+=montant; soldeContrepartie*=(1-broker); return; }
};

class TradingManager{
    DevisesManager& devisesManager=DevisesManager::getManager();
    TextEditor& textEditor=TextEditor::getEditor();
    QString filename;
    QDate debutFichier, finFichier;

    EvolutionCours* evolution=nullptr;

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
    const EvolutionCours* getEvo() const{ return evolution; }
    void setEvo(EvolutionCours* evo){
        if(evolution==nullptr){
            evolution=evo; return;
        }else{
            throw new TradingException("Paire déjà définie");
        }
    }

    Simulation* addSimulation(double base, double contrepartie, QDate debut, QDate fin, double broker);
    void closeSimulation(Simulation* s);
};

#endif // TRADINGMANAGER_H
