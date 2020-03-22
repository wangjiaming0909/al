#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QListView>
#include <QAbstractNativeEventFilter>
#include <QSystemTrayIcon>
#include <QProcess>
#include "lineedit.h"
#include "listwidget.h"
#include "listviewdatamodel.h"
#include "hotkey.h"
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QShortcut;
class QSystemTrayIcon;

class Widget : public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

private slots:
    void onShowListView(const QString& str);
    void onEsc();
    void onLineEditorEnter();
    void onListViewEnter(const QModelIndex& index);
    void onQuit();
    void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason);
    void onGlobalWeakup();

private:
    QStringListModel* getListViewModel(const QString& str, QObject* parent);
    void resetSize();
    void setupShortcut();
    void initSystemTrayIcon();
    void startCommand(const QString& command);

private:
//    Ui::Widget *ui;
    QPoint lastPoint_;
    LineEdit *lineedit_;
    QVBoxLayout *verticalLayout_;
    QListView* listView_;
    ListViewDataModel* listViewDataModel_;
    std::map<Qt::Key, QShortcut*> shortCuts_;
    QSystemTrayIcon* systemTrayIcon_;
    Hotkey* globalHotKey_;
    std::map<QString, QProcess*> processes_;
};
#endif // WIDGET_H
