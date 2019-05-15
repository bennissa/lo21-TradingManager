#include "texteditor.h"

const QString pathImg = ":/img";
TextEditor::Handler TextEditor::handler=TextEditor::Handler();

TextEditor::TextEditor(){
    // Init
    this->setFixedSize(400, 900);
    text.setText(QString());

    // Init sections du menu
    file.setTitle("Fichier");
    edit.setTitle("Edition");
    menu.addMenu(&file);
    menu.addMenu(&edit);

    // Init actions
    setupFileActions();
    setupEditActions();
    setupFormatActions();

    // Layout
    layout.addWidget(&menu);
    layout.addWidget(&format);
    layout.addWidget(&text);
    this->setLayout(&layout);
}

void TextEditor::setupFileActions(){
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
    return;
}

void TextEditor::setupEditActions(){
    undo = edit.addAction("Annuler");
    undo->setShortcut(QKeySequence::Undo);
    connect(undo, SIGNAL(triggered()), this, SLOT(actionUndo()));

    redo = edit.addAction("Rétablir");
    redo->setShortcut(QKeySequence::Redo);
    connect(redo, SIGNAL(triggered()), this, SLOT(actionRedo()));

    copy = edit.addAction("Copier");
    copy->setShortcut(QKeySequence::Copy);
    connect(copy, SIGNAL(triggered()), this, SLOT(actionCopy()));

    paste = edit.addAction("Coller");
    paste->setShortcut(QKeySequence::Paste);
    connect(paste, SIGNAL(triggered()), this, SLOT(actionPaste()));

    // Griser sauvegarde, retour et suivant quand il faut
    connect((&text)->document(), SIGNAL(modificationChanged(bool)), save, SLOT(setEnabled(bool)));
    connect(&text, SIGNAL(undoAvailable(bool)), undo, SLOT(setEnabled(bool)));
    connect(&text, SIGNAL(redoAvailable(bool)), redo, SLOT(setEnabled(bool)));
    save->setEnabled(text.document()->isModified());
    undo->setEnabled(text.document()->isUndoAvailable());
    redo->setEnabled(text.document()->isRedoAvailable());
    return;
}

void TextEditor::setupFormatActions(){
    bold = format.addAction(QIcon(pathImg + "/bold.png"), "Gras");
    bold->setShortcut(QKeySequence::Bold);
    bold->setCheckable(true);
    connect(bold, SIGNAL(triggered()), this, SLOT(actionBold()));

    italic = format.addAction(QIcon(pathImg + "/italic.png"), "Italique");
    italic->setShortcut(QKeySequence::Italic);
    italic->setCheckable(true);
    connect(italic, SIGNAL(triggered()), this, SLOT(actionBold()));

    underline = format.addAction(QIcon(pathImg + "/underline.png"), "Souligné");
    underline->setShortcut(QKeySequence::Underline);
    underline->setCheckable(true);
    connect(underline, SIGNAL(triggered()), this, SLOT(actionBold()));

    // Vérif s'il faut checker un format ou pas quand le curseur change de place
    connect(&text, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)), this, SLOT(currentCharFormatChanged(const QTextCharFormat&)));
    return;
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
    return;
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
    if(f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream contenu(&f);
        text.setText(contenu.readAll());
        f.close();
    }else{
        QMessageBox(QMessageBox::Warning, "Erreur", "Impossible d'ouvrir le fichier : "+f.errorString());
    }
    return;
}

void TextEditor::actionSave(){
    if(current!=""){
        if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream contenu(&f);
            contenu << text.toPlainText();
            f.close();
            text.document()->setModified(false);
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
    return;
}

void TextEditor::actionSaveAs(){
    current = QFileDialog::getSaveFileName(this, "Sauvegarder sous ...", "", "Text Files (*.txt)");
    f.setFileName(current);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream contenu(&f);
        contenu << text.toPlainText();
        f.close();
        text.document()->setModified(false);
    }else{
        QMessageBox(QMessageBox::Warning, "Erreur", "Impossible d'ouvrir le fichier : "+f.errorString());
    }
    return;
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
    return;
}

void TextEditor::actionUndo(){
    text.undo();
    return;
}

void TextEditor::actionRedo(){
    text.redo();
    return;
}

void TextEditor::actionCopy(){
    text.copy();
    return;
}

void TextEditor::actionPaste(){
    text.paste();
    return;
}

void TextEditor::actionBold(){
    QTextCharFormat fmt;
    if(bold->isChecked()){
        fmt.setFontWeight(QFont::Bold);
    }else{
        fmt.setFontWeight(QFont::Normal);
    }
    changeFormat(fmt);
    return;
}

void TextEditor::actionItalic(){
    QTextCharFormat fmt;
    fmt.setFontItalic(italic->isChecked());
    changeFormat(fmt);
    return;
}

void TextEditor::actionUnderline(){
    QTextCharFormat fmt;
    fmt.setFontUnderline(underline->isChecked());
    changeFormat(fmt);
    return;
}

void TextEditor::changeFormat(const QTextCharFormat &format){
    QTextCursor cursor = text.textCursor();
    cursor.mergeCharFormat(format);
    text.mergeCurrentCharFormat(format);
}

void TextEditor::fontChanged(const QFont &f){
    bold->setChecked(f.bold());
    italic->setChecked(f.italic());
    underline->setChecked(f.underline());
}
