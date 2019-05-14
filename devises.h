#ifndef _TRADING_H
#define _TRADING_H
#include <QString>
#include <QDate>
#include <cmath>
#include <QWidget>
#include <QtCharts>

using namespace std;

class Devise{
    QString code;
    QString monnaie, zone;
    Devise(const QString& c, const QString& m, const QString& z = "");
    ~Devise() = default;
    friend class DevisesManager;

    public:
    QString getCode() const { return code; }
    const QString& getMonnaie() const { return monnaie; }
    const QString& getZoneGeographique() const { return zone; }
};

class PaireDevises{
    const Devise* base;
    const Devise* contrepartie;
    QString surnom;
    PaireDevises(const Devise& b, const Devise& c, const QString& s = "");
    ~PaireDevises() = default;
    friend class DevisesManager;

    public:
    const Devise& getBase() const { return *base; }
    const Devise& getContrepartie() const { return *contrepartie; }
    const QString& getSurnom() const { return surnom; }
    QString toString() const;
};

class CoursOHLC{
    double open = 0, high = 0, low = 0, close = 0;
    QDate date;

    public:
    CoursOHLC() {}
    CoursOHLC(double o, double h, double l, double c, const QDate& d);
    double getOpen() const { return open; }
    double getHigh() const { return high; }
    double getLow() const { return low; }
    double getClose() const { return close; }
    void setCours(double o, double h, double l, double c);
    QDate getDate() const { return date; }
    void setDate(const QDate& d) { date=d; }
};

class EvolutionCours{
    const PaireDevises* paire;
    CoursOHLC* cours = nullptr;
    unsigned int nbCours = 0;
    unsigned int nbMaxCours = 0;

    public:
    EvolutionCours(const PaireDevises* p) :paire(p) {}
    void addCours(double o, double h, double l, double c, const QDate& d);
    ~EvolutionCours();
    EvolutionCours(const EvolutionCours& e);
    EvolutionCours& operator=(const EvolutionCours& e);
    unsigned int getNbCours() const { return nbCours; }
    const PaireDevises& getPaireDevises() const { return *paire; }
    using iterator = CoursOHLC*;
    iterator begin() { return iterator(cours); }
    iterator end() { return iterator(cours + nbCours); }
    using const_iterator = const CoursOHLC*;
    const_iterator begin() const { return cours; }
    const_iterator end() const { return cours + nbCours; }
    const_iterator cbegin() const { return cours; }
    const_iterator cend() const { return cours + nbCours; }
};

class DevisesManager{
    Devise** devises = nullptr; // tableau de pointeurs sur objets Devise
    unsigned int nbDevises = 0;
    unsigned int nbMaxDevises = 0;
    mutable PaireDevises** paires = nullptr; // tableau de pointeurs sur objets PaireDevises
    mutable unsigned int nbPaires = 0;
    mutable unsigned int nbMaxPaires = 0;

    // empêcher la duplication par recopie ou affectation
    DevisesManager(const DevisesManager& m) = delete;
    DevisesManager& operator=(const DevisesManager& m) = delete;
    // pour le singleton
    DevisesManager() {}
    ~DevisesManager();
    struct Handler {
        DevisesManager* instance = nullptr;
        ~Handler() { delete instance; }
    };
    static Handler handler;

    public:
    static DevisesManager& getManager(){
        if (handler.instance == nullptr)
            handler.instance = new DevisesManager;
        return *handler.instance;
    }
    static void libererManager(){
        delete handler.instance;  handler.instance = nullptr;
    }
    const Devise& creationDevise(const QString& c, const QString& m, const QString& z);
    const Devise& getDevise(const QString& c)const;
    const PaireDevises& getPaireDevises(const QString & c1, const QString & c2) const;
};

#endif
