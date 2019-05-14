#include "tradingmanager.h"
#include "devises.h"

TradingManager::Handler TradingManager::handler=TradingManager::Handler();

Transaction::Transaction(Simulation* s, CoursOHLC* c, double montant):sim(s),cours(c){
    if(montant>0){ // Achat de base
        s->addToSoldeBase(montant*c->getOpen());
        s->addToSoldeContrepartie((-montant)*cours->getClose());
    }else{ // Vente de base
        s->addToSoldeBase(-montant*c->getOpen());
        s->addToSoldeContrepartie(montant*c->getClose());
    }
}

Transaction* Simulation::addTransaction(double montant, CoursOHLC* c){
    if(nbMaxTransactions==nbTransactions){
        Transaction** newtab = new Transaction*[nbMaxTransactions+10];
        for(unsigned int i=0; i<nbTransactions;i++) newtab[i]=transactions[i];
        Transaction** old=transactions;
        transactions=newtab;
        nbMaxTransactions+=10;
        delete[] old;
    }
    return transactions[nbTransactions++] = new Transaction(this, c, montant);
}

void Simulation::cancelLastTransaction(){
    if(nbTransactions!=0){
        double montant = transactions[nbTransactions]->getMontant();
        CoursOHLC* c = transactions[nbTransactions]->getCours();
        if(montant>0){ // Achat annulé
            addToSoldeBase(-montant*c->getOpen());
            addToSoldeContrepartie(montant*c->getClose());
        }else{ // Vente annulée
            addToSoldeBase(montant*c->getOpen());
            addToSoldeContrepartie(-montant*c->getClose());
        }
        delete transactions[nbTransactions];
        nbTransactions--;
    }else{
        throw new TradingException("Pas de transactions à annuler");
    }
}

Simulation* TradingManager::addSimulation(double base, double contrepartie, QDate debut, QDate fin, double broker){
    if(nbMaxSim==nbSim){
        Simulation** newtab = new Simulation*[nbMaxSim+5];
        for(unsigned int i=0; i<nbSim;i++) newtab[i]=sim[i];
        Simulation** old=sim;
        sim=newtab;
        nbMaxSim+=5;
        delete[] old;
    }
    return sim[nbSim++] = new Simulation(evolution, base, contrepartie, debut, fin, broker);
}
void TradingManager::closeSimulation(Simulation* s){
    for(unsigned int i=0;i<nbSim;i++){
        if(sim[i]==s){
            delete s;
            for(unsigned int j=i;j<nbSim-1;j++){
                sim[j]=sim[j+1];
            }
            nbSim--;
            return;
        }
    }
    throw new TradingException("La simulation n'existe pas");
}
