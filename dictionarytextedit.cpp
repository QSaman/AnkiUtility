#include "dictionarytextedit.h"
#include "xmlbasedsettings.h"
#include "mainwindow.h"
#include <QMimeData>
#include <QAction>
#include <QList>
#include <QDebug>
#include <QContextMenuEvent>
#include <QIcon>
#include <QWidget>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

#define var(x) #x << ": " << x

DictionaryTextEdit::DictionaryTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    XmlBasedSettings::loadXmlBasedSettings();
    initActions();
    //setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
}

void DictionaryTextEdit::clipboardDataChanged()
{
    const QMimeData * md = QApplication::clipboard()->mimeData();
    if (md == 0)
        return;
    pasteAction->setEnabled(md->hasText());
}

void DictionaryTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu;
    menu.addActions(_actionList);
    menu.exec(e->globalPos());

}

void DictionaryTextEdit::populateEditToolBar(QWidget *widget)
{
    widget->addActions(_actionList);
}

void DictionaryTextEdit::initActions()
{
    copyAction = new QAction("&Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setIcon(QIcon(":/Resources/Images/copy.png"));
    copyAction->setEnabled(false);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(this, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));
    addAction(copyAction);
    _actionList.push_back(copyAction);

    cutAction = new QAction("C&ut", this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setIcon(QIcon(":/Resources/Images/cut.png"));
    cutAction->setEnabled(false);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(this, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    addAction((cutAction));
    _actionList.push_back(cutAction);

    pasteAction = new QAction("&Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setIcon(QIcon(":/Resources/Images/paste.png"));
    clipboardDataChanged();
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    _actionList.push_back(pasteAction);
}

void DictionaryTextEdit::insertFromMimeData(const QMimeData * source)
{
    if (source->hasHtml())
        insertHtml(htmlModifier.normalizeHtml(source->html()));
    else
        QTextEdit::insertFromMimeData(source);
}

void DictionaryTextEdit::copyImages(bool copy)
{
    m_copyImage = copy;
}

void DictionaryTextEdit::copy()
{
    QTextCursor cur;

    if (!m_copyImage)
    {
        textEditState.saveState(this);
        deleteImages();
    }
    QTextEdit::copy();
    if (!m_copyImage)
        textEditState.restoreState(this);
}

void DictionaryTextEdit::cut()
{
    if (!m_copyImage)
        deleteImages();
    QTextEdit::cut();
}

void DictionaryTextEdit::deleteImages()
{
    QTextCursor cursor = textCursor();
    htmlModifier.DeleteImages(cursor);
    setTextCursor(cursor);
}

void DictionaryTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (!(e->modifiers() | Qt::ControlModifier))
    {
        QTextEdit::keyPressEvent(e);
        return;
    }
    if (e->key() == Qt::Key_C)    //CTRL+C
    {
        e->accept();
        if (copyAction->isEnabled())
            copy();
    }
    else if (e->key() == Qt::Key_X) //CTRL+X
    {
        e->accept();
        if (cutAction->isEnabled())
            cut();
    }
    else
        QTextEdit::keyPressEvent(e);
}

void DictionaryTextEdit::TextEditState::saveState(QTextEdit *textEidt)
{
    QTextCursor cursor = textEidt->textCursor();
    _selectionStart = cursor.selectionStart();
    _selectionEnd = cursor.selectionEnd();
    _htmlDoc = textEidt->toHtml();
    hasSavedState = true;
}

void DictionaryTextEdit::TextEditState::restoreState(QTextEdit *textEdit)
{
    if (!hasSavedState)
        return;
    textEdit->setHtml(_htmlDoc);
    QTextCursor cursor = textEdit->textCursor();
    cursor.setPosition(_selectionStart);
    cursor.setPosition(_selectionEnd, QTextCursor::KeepAnchor);
    textEdit->setTextCursor(cursor);
    hasSavedState = false;
}
