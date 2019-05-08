#include "texteditor.h"

TextEditor::~TextEditor(){}
TextEditor::TextEditor(){
    // Init
    this->setFixedSize(400, 900);
    text.setText(QString());

    // Sections
    file.setTitle("Fichier");
    edit.setTitle("Edition");
    menu.addMenu(&file);
    menu.addMenu(&edit);

    // Actions Fichier
    nouv = file.addAction("Nouveau");
    nouv->setShortcut(QKeySequence::New);
    nouv->setStatusTip("Créer un nouveau fichier de notes");
    connect(nouv, SIGNAL(triggered()), this, SLOT(actionNew()));

    open = file.addAction("Ouvrir");
    open->setShortcut(QKeySequence::Open);
    open->setStatusTip("Créer un nouveau fichier de notes");
    connect(open, SIGNAL(triggered()), this, SLOT(actionOpen()));

    save = file.addAction("Sauvegarder");
    save->setShortcut(QKeySequence::Save);
    save->setStatusTip("Créer un nouveau fichier de notes");
    connect(save, SIGNAL(triggered()), this, SLOT(actionSave()));

    saveas = file.addAction("Sauvegarder sous ...");
    saveas->setShortcut(QKeySequence::SaveAs);
    saveas->setStatusTip("Créer un nouveau fichier de notes");
    connect(saveas, SIGNAL(triggered()), this, SLOT(actionSaveAs()));

    print = file.addAction("Imprimer");
    print->setShortcut(QKeySequence::Print);
    print->setStatusTip("Créer un nouveau fichier de notes");
    connect(print, SIGNAL(triggered()), this, SLOT(actionPrint()));

    // Actions Editer
    undo = edit.addAction("Précédent");
    undo->setShortcut(QKeySequence::Undo);
    connect(undo, SIGNAL(triggered()), this, SLOT(actionUndo()));

    redo = edit.addAction("Suivant");
    redo->setShortcut(QKeySequence::Redo);
    connect(redo, SIGNAL(triggered()), this, SLOT(actionRedo()));

    copy = edit.addAction("Copier");
    copy->setShortcut(QKeySequence::Copy);
    connect(copy, SIGNAL(triggered()), this, SLOT(actionCopy()));

    paste = edit.addAction("Coller");
    paste->setShortcut(QKeySequence::Paste);
    connect(paste, SIGNAL(triggered()), this, SLOT(actionPaste()));

    // Layout
    layout.addWidget(&menu);
    layout.addWidget(&text);
    this->setLayout(&layout);
}

void TextEditor::actionNew(){
    if(text.document()->isModified()){
        QMessageBox msgBox;
        msgBox.setText("Le document a été modifié");
        msgBox.setInformativeText("Voulez vous le sauvegarder avant d'en créer un nouveau ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              TextEditor::actionSaveAs();
              break;
          case QMessageBox::Discard:
              break;
          case QMessageBox::Cancel:
              return;
        }
    }
    current="";
    text.setText("");
}

void TextEditor::actionOpen(){
    if(text.document()->isModified()){
        QMessageBox msgBox;
        msgBox.setText("Le document a été modifié");
        msgBox.setInformativeText("Voulez vous le sauvegarder avant d'en créer un nouveau ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              TextEditor::actionSave();
              break;
          case QMessageBox::Discard:
              break;
          case QMessageBox::Cancel:
              return;
        }
    }
    current = QFileDialog::getOpenFileName(this, "Ouvrir ...", "", "Text Files (*.txt)");
    f.setFileName(current);
    if(f.open(QIODevice::ReadOnly, QIODevice::Text)){
        QTextStream contenu(&f);
        text.setText(contenu.readAll());
        f.close();
    }else{
        QMessageBox(QMessageBox::Warning, "Erreur", "Impossible d'ouvrir le fichier : "+f.errorString());
    }
}

void TextEditor::actionSave(){
    if(current!=""){
        if(f.open(QIODevice::WriteOnly, QIODevice::Text)){
            QTextStream contenu(&f);
            contenu << text.toPlainText();
            f.close();
        }else{
            QMessageBox(QMessageBox::Warning, "Erreur", "Impossible d'ouvrir le fichier : "+f.errorString());
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("Fichier vide");
        msgBox.setInformativeText("Voulez vous sauvegarder un nouveau fichier vide ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              TextEditor::actionSaveAs();
              break;
          case QMessageBox::Discard:
              break;
          case QMessageBox::Cancel:
              return;
        }
    }
}

void TextEditor::actionSaveAs(){
    current = QFileDialog::getSaveFileName(this, "Sauvegarder sous ...", "", "Text Files (*.txt)");
    f.setFileName(current);
    if(f.open(QIODevice::WriteOnly, QIODevice::Text)){
        QTextStream contenu(&f);
        contenu << text.toPlainText();
        f.close();
    }else{
        QMessageBox(QMessageBox::Warning, "Erreur", "Impossible d'ouvrir le fichier : "+f.errorString());
    }
}

void TextEditor::actionPrint(){
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (text.textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle("Imprimer");
    if (dlg->exec() == QDialog::Accepted){
        text.print(&printer);
    }else{
        QMessageBox(QMessageBox::Warning, "Erreur", "Imprimante inaccessible");
    }
    delete dlg;
}

void TextEditor::actionUndo(){
    text.undo();
}

void TextEditor::actionRedo(){
    text.undo();
}

void TextEditor::actionCopy(){
    text.copy();
}

void TextEditor::actionPaste(){
    text.paste();
}
