#ifndef DICTIONARYTEXTEDIT_H
#define DICTIONARYTEXTEDIT_H

#include <QTextEdit>
#include "htmlmodfier.h"

class DictionaryTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DictionaryTextEdit(QWidget *parent = 0);
protected:
    void insertFromMimeData(const QMimeData * source);
    
signals:
    
public slots:
private:
    HtmlModifier htmlModifier;
    
};

#endif // DICTIONARYTEXTEDIT_H
