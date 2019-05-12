#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QString>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QTextEdit>

class TextEditor : public QWidget{
    Q_OBJECT

    QVBoxLayout layout;
    QMenuBar menu;
    QMenu file;
    QMenu edit;
    QToolBar format;

    QFile f;
    QString current="";
    QTextEdit text;

    QAction* nouv;
    QAction* open;
    QAction* save;
    QAction* saveas;
    QAction* print;

    QAction* undo;
    QAction* redo;
    QAction* copy;
    QAction* paste;

    QAction* bold;
    QAction* italic;
    QAction* underline;

    // Interdiction de recopie ou d'affecter
    TextEditor(const TextEditor& m) = delete;
    TextEditor& operator=(const TextEditor& m) = delete;

    // Singleton
    TextEditor();
    ~TextEditor(){}
    struct Handler{
        TextEditor* instance = nullptr;
        ~Handler(){ delete instance; }
    };
    static Handler handler;

    public:

    static TextEditor& getEditor() {
        if(handler.instance == nullptr)
            handler.instance = new TextEditor;
        return *handler.instance;
    }
    static void libererEditor() {
        delete handler.instance;  handler.instance = nullptr;
    }
    void setupFileActions();
    void setupEditActions();
    void setupFormatActions();
    void changeFormat(const QTextCharFormat &format);
    void fontChanged(const QFont &f);

    private slots:

    void actionNew();
    void actionOpen();
    void actionSave();
    void actionSaveAs();
    void actionPrint();

    void actionUndo();
    void actionRedo();
    void actionCopy();
    void actionPaste();

    void actionBold();
    void actionItalic();
    void actionUnderline();
    void currentCharFormatChanged(const QTextCharFormat &format){ fontChanged(format.font()); }
};

#endif // TEXTEDITOR_H
