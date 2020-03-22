#include "widget.h"
#include <QMouseEvent>
#include <QStringListModel>
#include <QShortcut>
#include <thread>
#include <chrono>
#include <QMenu>
#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , shortCuts_{}
    , processes_{}
//    , ui(new Ui::Widget)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    lineedit_ = new LineEdit(this);
    lineedit_->resize(600, 20);
    qDebug() << this->width() << " h: " << this->height();
    this->resize(lineedit_->width(), lineedit_->height() + 22);
    qDebug() << this->width() << " h: " << this->height();
    verticalLayout_ = new QVBoxLayout(this);
    verticalLayout_->addWidget(lineedit_);
    connect(lineedit_, SIGNAL(textEdited(const QString&)), this, SLOT(onShowListView(const QString&)));
    connect(lineedit_, SIGNAL(returnPressed()), this, SLOT(onLineEditorEnter()));
    this->setLayout(verticalLayout_);
    listView_ = new QListView();
    connect(listView_, SIGNAL(activated(const QModelIndex&) ), this, SLOT(onListViewEnter(const QModelIndex&)));
    listViewDataModel_ = new ListViewDataModel();

    setupShortcut();
    initSystemTrayIcon();
}

Widget::~Widget()
{
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    lastPoint_ = e->globalPos();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->globalX() - lastPoint_.x();
    int dy = event->globalY() - lastPoint_.y();
    lastPoint_ = event->globalPos();
    move(x()  + dx, y() + dy);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - lastPoint_.x();
    int dy = event->globalY() - lastPoint_.y();
    move(x()+dx, y()+dy);
}

bool Widget::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{

}

void Widget::onShowListView(const QString& str)
{
    qDebug() << str;
    if(str == "")
    {
        verticalLayout_->removeWidget(listView_);
        listView_->setParent(nullptr);
        resetSize();
        return;
    }
    listView_->setParent(this);
    QStringListModel* model = getListViewModel(str, listView_);
    listView_->setModel(model);
    verticalLayout_->addWidget(listView_);
}

QStringListModel *Widget::getListViewModel(const QString &str, QObject* parent)
{
    return listViewDataModel_->searchWithRegex(str, parent);
}

void Widget::resetSize()
{
    setMinimumWidth(lineedit_->width() + 22);
    setMinimumHeight(lineedit_->height() + 22);
    this->resize(lineedit_->width() + 22, lineedit_->height()  + 22);
    setMinimumSize(0, 0);
}

void Widget::setupShortcut()
{
    auto* escSc = new QShortcut(this);
    escSc->setKey(Qt::Key_Escape);
    escSc->setAutoRepeat(false);
    shortCuts_[Qt::Key_Escape] = escSc;
    connect(escSc, SIGNAL(activated()), this, SLOT(onEsc()));

    globalHotKey_ = new Hotkey(this);
    globalHotKey_->registerHotkey(QKeySequence{"ctrl+shift+m"});
    connect(globalHotKey_, SIGNAL(activated()), this, SLOT(onGlobalWeakup()));
}

void Widget::initSystemTrayIcon()
{
    systemTrayIcon_ = new QSystemTrayIcon(this);
    systemTrayIcon_->setIcon(QIcon("image/search.jpg"));
    systemTrayIcon_->show();
    systemTrayIcon_->setToolTip("Everything");

    QMenu* trayMenu = new QMenu();
    QAction* quitAction = new QAction(tr("Quit"), trayMenu);
    trayMenu->addAction(quitAction);
    systemTrayIcon_->setContextMenu(trayMenu);
    connect(systemTrayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(onQuit()));
}

void Widget::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        if(isHidden()) show();
        else
            activateWindow();
        break;
    default:
        break;
    }
}

void Widget::onGlobalWeakup()
{
    qDebug() << "on global weakup...";
    if(isHidden())
    {
        show();
        lineedit_->setFocus();
        lineedit_->selectAll();
    }
    else if (!this->isActiveWindow())
    {
        this->activateWindow();
    }else
    {
        this->hide();
    }
}

void Widget::onQuit()
{
    qDebug() << "on quit";
    this->close();
}

void Widget::onEsc()
{
    qDebug() << "on esc...";
    this->hide();
}

void Widget::onLineEditorEnter()
{
    if(this->focusWidget() != listView_)
    {
        listView_->setFocus();
        listView_->setCurrentIndex(listView_->rootIndex());
        QModelIndex firstIndex = listView_->model()->index(0, 0);
        listView_->setCurrentIndex(firstIndex);
    }
}

void Widget::onListViewEnter(const QModelIndex& index)
{
    auto dataVar = listView_->model()->data(index);
    auto dataStr = dataVar.toString();
    auto keyAndCommand = dataStr.split(ListViewDataModel::delimeter);
    if(keyAndCommand.size() < 2)
    {
        qDebug() << "data model error: " << dataStr;
        return;
    }
    if(!QFile::exists(keyAndCommand[1]))
    {
        qDebug() << "command file not existed: " << dataStr;
        return;
    }
    startCommand(keyAndCommand[1]);
}

void Widget::startCommand(const QString &command)
{
    qDebug() << "start command: " << command;
    QProcess* process = new QProcess();
    process->setProgram(command);
    auto ret = process->startDetached();
    if(ret)
    {
        hide();
    }
    qDebug() << ret;
}
