﻿#include "slidershow.h"
#include "QDir"
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "../PluginService/ui_unit/xrandrobject.h"
#include "../PluginService/ksystemsetting_unit.h"
#include "../PluginService/ui_unit/xrandrobject.h"
#include <QPainter>
#define RATIO_1 0.6

#define IMG_PATH  ":/data/png/slidershow/lingmo-slideshow"

namespace LMInstaller {
using namespace KServer;
SlideShow::SlideShow(QWidget *parent) : QWidget(parent),
    m_currentIndex(0)
{
    initUI();
    timer = new QTimer(this);
    timer->setInterval(5000);

    initAllConnect();
    addStyleSheet();
    start();
    this->setFocus();
}

QImage SlideShow::addtransterstr(QImage &image, QRect rct, QString str,QFont font,QColor color)
{

    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);

    QPen pen = painter.pen();
    pen.setColor(color);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rct, Qt::AlignHCenter|Qt::TextWordWrap, str);
    return image;
}

void SlideShow::loadSlides()
{
    sliderWidget = new QStackedWidget();
    QRect rect = KServer::GetScreenRect();


    QFont ft1,ft2;
    ft1.setFamily(QStringLiteral("bold"));
    ft1.setBold(true);
    ft1.setPointSize(24);

    ft2.setFamily(QStringLiteral("bold"));
    ft2.setBold(true);
    ft2.setPointSize(12);

    //载入背景图
    QDir *dir = new QDir(IMG_PATH);
    QStringList filter;
    filter << "*.png";
    dir->setNameFilters(filter);

    QFileInfoList fileInfoList = dir->entryInfoList(filter);
    int num =fileInfoList.count();

    for (int i = 1; i <= num; i++) {
        Slide *slide = new Slide(this);
         slide->setObjectName("slide_"+QString::number(i));
//      slide->hide();
         QString titlestr="";
         switch (i) {
            case 1:titlestr=tr("Nothing");break;
         default:
             break;
         }

    QImage image(QString(IMG_PATH).append("/%1.png").arg(i));

    //统一标准，将图片缩放至固定大小便于写字
    image = image.scaledToHeight(500, Qt::SmoothTransformation);
    QRect rect1,rect2;
    rect1=QRect(0,10,image.width(),50);
    rect2=QRect(0,60,image.width(),30);

    image = addtransterstr(image, rect1,titlestr.section('@',0,0),ft1,QColor(220,220,220));
    image = addtransterstr(image, rect2,titlestr.section('@',1,1),ft2,QColor(220,220,220));

    image = image.scaledToHeight(rect.height() * RATIO_1, Qt::SmoothTransformation);

    slide->setPixmap(QPixmap::fromImage(image));
    slide->setScaledContents(true);
     sliderWidget->addWidget(slide);
    }
    sliderWidget->setCurrentIndex(m_currentIndex);
}

void SlideShow::initUI()
{
    QHBoxLayout* hlayout = new QHBoxLayout(this);
    this->setLayout(hlayout);

    m_prev = new QPushButton(this);
    m_prev->setObjectName("prev");
    m_prev->installEventFilter(this);

    m_prev->setIcon(QIcon(":/data/png/slidershow/prev.svg"));
//    m_prev->setIconSize(QSize(16, 16));
    m_prev->setStyleSheet(QLatin1String("QPushButton{border: none;outline: none;color: white;padding:16px 16px 16px 16px;background:transparent;border:none;}"
                                        "QPushButton:hover{"
                                        "width: 48px;height: 48px;background: rgba(255, 255, 255, 0.35);border-radius: 6px;}"
                                        " QPushButton:checked{ "
                                        " height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"
                                        "QPushButton:pressed{height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"
                                        "QPushButton:foucs{height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"));
    hlayout->addWidget(m_prev, Qt::AlignCenter);
    loadSlides();

    hlayout->addWidget(sliderWidget);
    m_next = new QPushButton(this);
    m_next->setObjectName("next");
    m_next->installEventFilter(this);
    m_next->setIcon(QIcon(":/data/png/slidershow/next.svg"));
//    m_next->setIconSize(QSize(16, 16));
    m_next->setStyleSheet(QLatin1String("QPushButton{border: none;outline: none;color: white;padding:16px 16px 16px 16px;background:transparent;border:none;}"
                                        "QPushButton:hover{"
                                        "width: 48px;height: 48px;background: rgba(255, 255, 255, 0.35);border-radius: 6px;}"
                                        " QPushButton:checked{ "
                                        " height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"
                                        "QPushButton:pressed{height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"
                                        "QPushButton:foucs{height: 48px;background: rgba(255, 255, 255, 0.25); border-radius: 6px;}"));
    hlayout->addWidget(m_next, Qt::AlignCenter);


}

void SlideShow::initAllConnect()
{
    connect(timer, &QTimer::timeout, this, &SlideShow::next);
    connect(m_prev, &QPushButton::clicked, this, &SlideShow::prev);
    connect(m_next, &QPushButton::clicked, this, &SlideShow::next);
}

void SlideShow::addStyleSheet()
{
    QFile file(":/data/qss/InstallingFrame.css");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    file.close();

    this->setStyleSheet(stylesheet);
}

void SlideShow::changeSlide(int index)
{
    if (index > 0) {
        m_currentIndex++;
        if (m_currentIndex > sliderWidget->count() -1)
            m_currentIndex = 0;
        sliderWidget->setCurrentIndex(m_currentIndex);

    } else if (index < 0 ) {
        m_currentIndex--;
        if (m_currentIndex < 0) {
            m_currentIndex = sliderWidget->count() - 1;
        }
        sliderWidget->setCurrentIndex(m_currentIndex) ;
    }

}

void SlideShow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << Q_FUNC_INFO << "keyPressEvent";
    if(event->key() == Qt::Key_Left) {
        prev();
    } else if(event->key() == Qt::Key_Right) {
        next();
    } else if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_Control) {
        this->setFocus();
    } else
        QWidget::keyPressEvent(event);
}

bool SlideShow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *ev = (QKeyEvent*) event;
        switch (ev->key()) {
        case Qt::Key_Left:
            prev();
            break;
        case Qt::Key_Right:
            next();
            break;
        }
        return true;
    }
    return false;
}

void SlideShow::next()
{
    timer->stop();
    changeSlide(1);
    timer->start();
}

void SlideShow::prev()
{
    timer->stop();
    changeSlide(-1);
    timer->start();
}

void SlideShow::start()
{
    changeSlide(0);
    timer->start();
}

}
