#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>

class QListWidget;
class QStringListModel;
class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);

signals:

private:
    QListWidget *listWidget_;
    QStringListModel *model_;
};

#endif // LISTWIDGET_H
