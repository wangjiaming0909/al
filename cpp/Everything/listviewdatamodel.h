#ifndef LISTVIEWDATAMODEL_H
#define LISTVIEWDATAMODEL_H
#include <QStringListModel>
#include <QMap>


class ListViewDataModel
{
public:
    ListViewDataModel();
    QStringListModel* getResult(const QString& str);

public:
    QStringListModel* search(const QString& name, QObject* parent);
    QStringListModel* searchWithRegex(const QString& pattern, QObject* parent);
    static const QString delimeter;

private:
    void initMap();

private:
    QMap<QString, QString> map_;
    static const QString fileName;
};

#endif // LISTVIEWDATAMODEL_H
