#include "dictionarytextedit.h"
#include "xmlbasedsettings.h"
#include "mainwindow.h"
#include <QMimeData>

DictionaryTextEdit::DictionaryTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    XmlBasedSettings::loadXmlBasedSettings();    
}

void DictionaryTextEdit::insertFromMimeData(const QMimeData * source)
{
    if (source->hasHtml())
        setText(htmlModifier.normalizeHtml(source->html()));
    else
        QTextEdit::insertFromMimeData(source);
}

void DictionaryTextEdit::copyImages(bool copy)
{
    m_copyImage = copy;
}

void DictionaryTextEdit::copy()
{
    if (!m_copyImage)
        deleteImages();
    QTextEdit::copy();
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
