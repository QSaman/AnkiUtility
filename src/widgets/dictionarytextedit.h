#ifndef DICTIONARYTEXTEDIT_H
#define DICTIONARYTEXTEDIT_H

#include <QTextEdit>
#include "src/core/htmlmodfier.h"
#include <QList>
#include <QString>

class QAction;
class QWidget;
class QKeyEvent;
class QContextMenuEvent;

class DictionaryTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DictionaryTextEdit(QWidget *parent = 0);
    void populateEditWidget(QWidget * widget);
    void populateFontWidget(QWidget * widget);
protected:
    void insertFromMimeData(const QMimeData * source);
    void contextMenuEvent(QContextMenuEvent *e);
    
signals:
    
public slots:
    void copy();
    void cut();
    void copyImages(bool copy);
    void deleteImages();
    void clipboardDataChanged();

protected:
    void keyPressEvent(QKeyEvent *e);

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
    QAction * pasteAction;
    QList<QAction *> _editActionsList;

    QAction * zoomInAction;
    QAction * zoomOutAction;
    QAction * defaultSizeAction;
    QList<QAction *> _fontActionsList;

    TextEditState textEditState;
};

#endif // DICTIONARYTEXTEDIT_H
