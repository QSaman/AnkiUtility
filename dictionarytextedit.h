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
    void copy();
    void cut();
    void copyImages(bool copy);
    void deleteImages();
private:
    HtmlModifier htmlModifier;
    bool m_copyImage;
    
};

#endif // DICTIONARYTEXTEDIT_H
