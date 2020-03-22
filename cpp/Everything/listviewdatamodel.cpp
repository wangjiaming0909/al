#include "listviewdatamodel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
const QString ListViewDataModel::fileName = "settings.json";
const QString ListViewDataModel::delimeter = "      ";

ListViewDataModel::ListViewDataModel()
    :map_()
{
    initMap();
}

QStringListModel *ListViewDataModel::search(const QString &name, QObject* parent)
{
    QStringList list{};
    QStringListModel* ret = new QStringListModel(parent);
    for(auto it = map_.begin(); it != map_.end(); it++)
    {
        if(it.key() == name || it.value() == name)
            list.append(it.key() + "     " + it.value());
    }
    ret->setStringList(list);
    return ret;
}

QStringListModel *ListViewDataModel::searchWithRegex(const QString &pattern, QObject *parent)
{
    QStringList list{};
    QStringListModel* ret  = new QStringListModel(parent);
    QString tmp = "";
    if(!pattern.startsWith(".*"))
    {
        tmp.append(".*").append(pattern);
    }
    if(!pattern.endsWith(".*"))
    {
        tmp.append(".*");
    }
    QRegExp rx{tmp};
    rx.setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    for(auto it = map_.begin(); it != map_.end(); it++)
    {
        if(rx.exactMatch(it.key()) || rx.exactMatch(it.value()))
            list.append(it.key() + delimeter + it.value());
        else if(rx.exactMatch(it.value()) || rx.exactMatch(it.value()))
            list.append(it.key() + delimeter + it.value());
    }
    ret->setStringList(list);
    return ret;
}

void ListViewDataModel::initMap()
{
    QFile f(fileName);
    f.open(QFile::OpenModeFlag::ReadWrite);
    QByteArray bytes = f.readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(document.isNull() || error.error != QJsonParseError::NoError)
    {
        qDebug() << "error: " << error.error;
        return;
    }
    if(!document.isObject())
    {
        qDebug() << "docuemnt is not a json object";
        return;
    }

    auto jsonObject = document.object();
    auto commands = jsonObject.take("commands").toArray();
    for(auto it = commands.begin(); it != commands.end(); it++)
    {
        QJsonObject obj = it->toObject();
        if(!obj.contains("name") || !obj.contains("command"))
        {
            qDebug() << "json format error";
            return ;
        }
        auto name = obj["name"].toString();
        auto command = obj["command"].toString();
        map_.insert(name, command);
    }
}
