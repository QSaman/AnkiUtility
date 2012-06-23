#include "dictionarytextedit.h"
#include "xmlbasedsettings.h"
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
