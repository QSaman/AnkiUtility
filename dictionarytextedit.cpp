#include "dictionarytextedit.h"
#include "xmlbasedsettings.h"
#include "mainwindow.h"
#include <QMimeData>
#include <QAction>
#include <QList>
#include <QDebug>
#include <QContextMenuEvent>
#include <QIcon>
#include <QToolBar>

#define var(x) #x << ": " << x

DictionaryTextEdit::DictionaryTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    XmlBasedSettings::loadXmlBasedSettings();
    initActions();
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void DictionaryTextEdit::populateEditToolBar(QToolBar *editToolBar)
{
    editToolBar->addActions(_actionList);
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
//        cur = textCursor();
//        qDebug() << "Before save state: " << var(cur.selectionStart()) << " - " << var(cur.selectionEnd());
//        qDebug() << "************************************";
        textEditState.saveState(this);
//        cur = textCursor();
//        qDebug() << "After save state and before delete images: " << var(cur.selectionStart()) << " - " << var(cur.selectionEnd());
//        qDebug() << "************************************";
        deleteImages();
//        cur = textCursor();
//        qDebug() << "After delete images: " << var(cur.selectionStart()) << " - " << var(cur.selectionEnd());
//        qDebug() << "************************************";
    }
    QTextEdit::copy();
    if (!m_copyImage)
        textEditState.restoreState(this);
//    cur = textCursor();
//    qDebug() << "after restore state: "  << var(cur.selectionStart()) << " - " << var(cur.selectionEnd());
//    qDebug() << "################################################################";
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
