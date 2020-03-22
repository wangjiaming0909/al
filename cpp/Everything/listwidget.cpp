#include "listwidget.h"
#include <QListWidget>
#include <qstringlistmodel.h>

ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
{
    listWidget_ = new QListWidget(this);
    model_   = new QStringListModel();
    this->setWindowFlag(Qt::FramelessWindowHint);
}
