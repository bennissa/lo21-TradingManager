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
    QToolBar toolbar;
    QMenuBar menu;

    QFile f;
    QString current="";
    QTextEdit text;

    QMenu file;
    QMenu edit;

    QAction* nouv;
    QAction* open;
    QAction* save;
    QAction* saveas;
    QAction* print;

    QAction* undo;
    QAction* redo;
    QAction* copy;
    QAction* paste;

    // Singleton

    public:
    TextEditor();
    ~TextEditor();
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
};

#endif // TEXTEDITOR_H
