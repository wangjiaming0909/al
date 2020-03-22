#ifndef LINEEDIT_H
#define LINEEDIT_H
#include <QLineEdit>
#include <QDebug>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent);

public slots:
    void onTextChanged(const QString& text);

};

#endif // LINEEDIT_H
