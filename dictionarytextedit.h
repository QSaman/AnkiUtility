#ifndef DICTIONARYTEXTEDIT_H
#define DICTIONARYTEXTEDIT_H

#include <QTextEdit>
#include "htmlmodfier.h"
#include <QList>
#include <QString>

class QAction;
class QToolBar;

class DictionaryTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DictionaryTextEdit(QWidget *parent = 0);
    void populateEditToolBar(QToolBar * editToolBar);
protected:
    void insertFromMimeData(const QMimeData * source);
    
signals:
    
public slots:
    void copy();
    void cut();
    void copyImages(bool copy);
    void deleteImages();

private:
    class TextEditState
    {
    public:
        TextEditState() : hasSavedState(false)  {}
        void saveState(QTextEdit *textEdit);
        void restoreState(QTextEdit *textEdit);
    private:
        int _selectionStart, _selectionEnd;
        QString _htmlDoc;
        bool hasSavedState;
    };

    void initActions();

    HtmlModifier htmlModifier;
    bool m_copyImage;
    QAction * copyAction;
    QAction * cutAction;
    QList<QAction *> _actionList;
    TextEditState textEditState;
};

#endif // DICTIONARYTEXTEDIT_H
