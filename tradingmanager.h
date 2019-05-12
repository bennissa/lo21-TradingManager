#ifndef TRADINGMANAGER_H
#define TRADINGMANAGER_H

#include "devises.h"
#include "texteditor.h"

class TradingException{
    public:
        TradingException(const QString& message):info(message){}
        QString getInfo() const{ return info; }
    private:
        QString info;
};

class Simulation{
    bool automatic=false;
};

class TradingManager{
    DevisesManager& devisesManager=DevisesManager::getManager();
    TextEditor& textEditor=TextEditor::getEditor();
    QString filename;

    Simulation** sim;
    bool PasAPas=false;

    // Interdiction de recopie ou d'affecter
    TradingManager(const TradingManager& m) = delete;
    TradingManager& operator=(const TradingManager& m) = delete;

    // Singleton
    TradingManager(){}
    ~TradingManager(){
        devisesManager.libererManager();
        textEditor.libererEditor();
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
};

#endif // TRADINGMANAGER_H
