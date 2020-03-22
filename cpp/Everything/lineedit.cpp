#include "lineedit.h"

LineEdit::LineEdit(QWidget* parent) : QLineEdit(parent)
{
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(onTextChanged(const QString&)));
}

void LineEdit::onTextChanged(const QString &text)
{
}




