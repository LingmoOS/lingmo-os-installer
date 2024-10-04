#include "languageframe.h"
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QApplication>

#include <QFile>
#include <QTextStream>
#include "../PluginService/klinguist.h"
#include "../PluginService/ksystemenv.h"
#include "../PluginService/ksystemsetting_unit.h"
#include "../PluginService/kdir.h"
#include "../PluginService/ui_unit/keyeventcontrol.h"
#include "../PluginService/ui_unit/xrandrobject.h"

using namespace KServer;

namespace LMInstaller {

LanguageFrame::LanguageFrame(QWidget *parent) : MiddleFrameManager(parent)
{
    m_pCurTranslator = new QTranslator;

    initUI();
    addStyleSheet();
    initAllConnect();
    initBtn();
    setBoardTabOrder();
    this->setFocus();
}

void LanguageFrame::initUI()
{
    QGridLayout* gLayoutl = new QGridLayout();
    gLayoutl->setContentsMargins(0, 0, 0, 0);
    m_Widget->setLayout(gLayoutl);
    gLayoutl->setColumnStretch(0, 3);
    gLayoutl->setColumnStretch(1, 2);
    gLayoutl->setColumnStretch(2, 3);
    gLayoutl->setVerticalSpacing(10);

    gLayoutl->addItem(new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Preferred), 0, 1, 1, 1);
    m_mainTitle = new QLabel();
    m_mainTitle->setObjectName("mainTitle");

    gLayoutl->addWidget(m_mainTitle, 1, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(10, 40, QSizePolicy::Expanding, QSizePolicy::Preferred), 2, 1, 1, 1);

    m_ZHCNBtn = new PushButtonCheck("中文(简体)");
    m_ZHCNBtn->setObjectName("zh_CN");
    m_ZHCNBtn->setCheckable(true);

    m_ZHHKBtn = new PushButtonCheck("中文(繁体)");
    m_ZHHKBtn->setObjectName("zh_HK");
    m_ZHHKBtn->setCheckable(true);

    m_KKKZBtn = new PushButtonCheck("Қазақ(哈萨克语)");
    m_KKKZBtn->setObjectName("kk_KZ");
    m_KKKZBtn->setCheckable(true);

    m_UGCNBtn = new PushButtonCheck("Uyghur(维吾尔语)");
    m_UGCNBtn->setObjectName("ug_CN");
    m_UGCNBtn->setCheckable(true);

    m_KYKGBtn = new PushButtonCheck("قىرعىز تىلى(柯尔克孜语)");
    m_KYKGBtn->setObjectName("ky_KG");
    m_KYKGBtn->setCheckable(true);

    m_BOCNBtn = new PushButtonCheck("བོད་ཡིག(藏语)");
    m_BOCNBtn->setObjectName("bo_CN");
    m_BOCNBtn->setCheckable(true);

    m_MNMNBtn = new PushButtonCheck("ᠮᠣᠩᠭᠣᠯ ᠪᠢᠴᠢᠭ᠌(蒙古语)");
    m_MNMNBtn->setObjectName("mn_MN");
    m_MNMNBtn->setCheckable(true);

    m_ENUSBtn = new PushButtonCheck("English(英语)");
    m_ENUSBtn->setObjectName("en_US");
    m_ENUSBtn->setCheckable(true);

    gLayoutl->addWidget(m_ZHCNBtn, 3, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 4, 1, 1, 1);

    gLayoutl->addWidget(m_ZHHKBtn, 4, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 6, 1, 1, 1);

    gLayoutl->addWidget(m_KKKZBtn, 5, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 8, 1, 1, 1);

    gLayoutl->addWidget(m_UGCNBtn, 6, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 10, 1, 1, 1);

    gLayoutl->addWidget(m_KYKGBtn, 7, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 12, 1, 1, 1);

    gLayoutl->addWidget(m_BOCNBtn, 8, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 14, 1, 1, 1);

    gLayoutl->addWidget(m_MNMNBtn, 9, 1, 1, 1, Qt::AlignCenter);
//    gLayoutl->addItem(new QSpacerItem(8, 24, QSizePolicy::Expanding, QSizePolicy::Minimum), 16, 1, 1, 1);

    gLayoutl->addWidget(m_ENUSBtn, 10, 1, 1, 1, Qt::AlignCenter);

    gLayoutl->addItem(new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 11, 1, 1, 1);
//    gLayoutl->setRowStretch(6, 1);
    translateStr();
}

void LanguageFrame::setBoardTabOrder()
{
    QWidget::setTabOrder(m_ZHCNBtn, m_ZHHKBtn);
    QWidget::setTabOrder(m_ZHHKBtn, m_KKKZBtn);
    QWidget::setTabOrder(m_KKKZBtn, m_UGCNBtn);
    QWidget::setTabOrder(m_UGCNBtn, m_KYKGBtn);
    QWidget::setTabOrder(m_KYKGBtn, m_BOCNBtn);
    QWidget::setTabOrder(m_BOCNBtn, m_MNMNBtn);
    QWidget::setTabOrder(m_MNMNBtn, m_ENUSBtn);
    QWidget::setTabOrder(m_ENUSBtn, m_nextBtn);
}

void LanguageFrame::initBtn()
{
    m_curSystenlocal = QString(getenv("LANGUAGE")).section('.',0,0);
    qDebug()<<"locale LANGUAGE="<<m_curSystenlocal;
    m_curSystenlocal = setAllBtnStatus(m_curSystenlocal);
    qDebug()<<"locale language="<<m_curSystenlocal;
    WriteSettingToIni("config", "language", m_curSystenlocal);
//    if (m_curSystenlocal == "en_US") {
//        m_ENUSBtn->setLabelShow(true);
//        m_ENUSBtn->setChecked(true);
//    } else {
//        m_ZHCNBtn->setLabelShow(true);
//        m_ZHCNBtn->setChecked(true);
//    }

//    emit changeLanguage_sig(m_curSystenlocal);
}

//void LanguageFrame::changeLanguageEn()
//{
//    QTranslator* curTranslator = new QTranslator;
//    QString langPath = QString("");
//    langPath = GetLingmoInstallPath() + "/language/";

//    //清理原来的翻译文件
//    if(!curTranslator->isEmpty())
//    {
//        qApp->removeTranslator(curTranslator);
//        delete curTranslator;
//    }
//    //加载新的翻译文件
//    m_curSystenlocal = "en_US";
//    if (curTranslator->load(langPath + m_curSystenlocal + ".qm")) {
//        qApp->installTranslator(curTranslator);
//    }
//    m_ZHCNBtn->setLabelShow(false);
//    m_ENUSBtn->setLabelShow(true);

//    unsetenv("LANG");
//     setenv("LANG", "en_US.UTF-8",1);
//    unsetenv("LANGUAGE");
//    setenv("LANGUAGE", "en_US.UTF-8",1);
//    WriteSettingToIni("config", "language", m_curSystenlocal);
//}

//void LanguageFrame::changeLanguageZh()
//{
//    QTranslator* curTranslator = new QTranslator;
//    QString langPath = QString("");
//    langPath = GetLingmoInstallPath() + "/language/";
//    //清理原来的翻译文件
//    if(!curTranslator->isEmpty())
//    {
//        qApp->removeTranslator(curTranslator);
//        delete curTranslator;
//    }
//    //安装新的翻译
//    m_curSystenlocal = "zh_CN";
//    if (curTranslator->load(langPath + m_curSystenlocal + ".qm")) {
//        qApp->installTranslator(curTranslator);
//    }
//    m_ENUSBtn->setLabelShow(false);
//    m_ZHCNBtn->setLabelShow(true);
//    unsetenv("LANG");
//    setenv("LANG", "zh_CN.UTF-8",1);
//    unsetenv("LANGUAGE");
//    setenv("LANGUAGE", "zh_CN.UTF-8",1);
//    WriteSettingToIni("config", "language", m_curSystenlocal);
//}

void LanguageFrame::initLanguageBox()//已不用
{
    m_languageBox->clear();
    m_languageBox->setInsertPolicy(QComboBox::InsertAtBottom);
    m_model = new ComboxListModel(m_languageBox);
    m_languageBox->setModel(m_model);
//    ComboxDelegate* delegate = new ComboxDelegate;
    ComboxDelegate delegate;
    m_languageBox->setItemDelegate(&delegate);
    m_curSystenlocal = QString(getenv("LANGUAGE")).section('.',0,0);
    QModelIndex index = m_model->getIndex(m_curSystenlocal);
    if (index.isValid()) {
        m_comboxItem = m_model->getItemLanguage(index);
        m_languageBox->setCurrentIndex(index.row());
    }
    m_languageBox->setMaxVisibleItems(5);
}

void LanguageFrame::initAllConnect()
{
//    connect(m_languageBox, static_cast< void ( QComboBox::* )( int ) >(&QComboBox::currentIndexChanged), this, &LanguageFrame::changeLanguage);
//    connect(m_ZHCNBtn, &PushButtonCheck::clicked, this, &LanguageFrame::changeLanguageZh);
//    connect(m_ENUSBtn, &PushButtonCheck::clicked, this, &LanguageFrame::changeLanguageEn);

    connect(m_ZHCNBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_ZHCNBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_ZHHKBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_ZHHKBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_KKKZBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_KKKZBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_UGCNBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_UGCNBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_KYKGBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_KYKGBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_BOCNBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_BOCNBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_MNMNBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_MNMNBtn,&PushButtonCheck::other_clicked_slot);

    connect(m_ENUSBtn,&PushButtonCheck::clicked_obj,this,&LanguageFrame::changeLanguage);
    connect(this,&LanguageFrame::changeLanguage_sig,m_ENUSBtn,&PushButtonCheck::other_clicked_slot);
}



void LanguageFrame::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        translateStr();
    } else {
        QWidget::changeEvent(event);
    }
}

void LanguageFrame::addStyleSheet()
{
    QFile file(":/res/qss/KChoiceLanguage.css");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = QString("");
    stylesheet = filetext.readAll();
    file.close();

    this->setStyleSheet(stylesheet);
}

void LanguageFrame::translateStr()
{
    m_mainTitle->setText(tr("Select Language"));
//    m_titleOS->setText(tr("Welcome to use Galaxy Lingmo desktop operating system"));
//    m_littleOS->setText(tr("LINGMO OS V10.1.0.4"));
    m_nextBtn->setText(tr("Next"));
}

void LanguageFrame::clickNextButton()
{
    emit signalStackPageChanged(1);
}

void LanguageFrame::keyPressEvent(QKeyEvent *event)
{
    this->setFocus();
    qDebug() << event->key() << "key pressed!";
    qDebug() << Q_FUNC_INFO << "keyPressEvent";
    if(event->key() == Qt::Key_Up)
    {
        //qDebug() << "up key pressed!";
        if(m_ZHCNBtn->isChecked() == true)
            changeLanguage(m_ENUSBtn);
        else if(m_ENUSBtn->isChecked() == true)
            changeLanguage(m_MNMNBtn);
        else if(m_MNMNBtn->isChecked() == true)
            changeLanguage(m_BOCNBtn);
        else if(m_BOCNBtn->isChecked() == true)
            changeLanguage(m_KYKGBtn);
        else if(m_KYKGBtn->isChecked() == true)
            changeLanguage(m_UGCNBtn);
        else if(m_UGCNBtn->isChecked() == true)
            changeLanguage(m_KKKZBtn);
        else if(m_KKKZBtn->isChecked() == true)
            changeLanguage(m_ZHHKBtn);
        else if(m_ZHHKBtn->isChecked() == true)
            changeLanguage(m_ZHCNBtn);
        else
            changeLanguage(m_ZHCNBtn);
    }
    else if(event->key() == Qt::Key_Down)
    {
        //qDebug() << "down key pressed!";
        if(m_ZHCNBtn->isChecked() == true)
            changeLanguage(m_ZHHKBtn);
        else if(m_ZHHKBtn->isChecked() == true)
            changeLanguage(m_KKKZBtn);
        else if(m_KKKZBtn->isChecked() == true)
            changeLanguage(m_UGCNBtn);
        else if(m_UGCNBtn->isChecked() == true)
            changeLanguage(m_KYKGBtn);
        else if(m_KYKGBtn->isChecked() == true)
            changeLanguage(m_BOCNBtn);
        else if(m_BOCNBtn->isChecked() == true)
            changeLanguage(m_MNMNBtn);
        else if(m_MNMNBtn->isChecked() == true)
            changeLanguage(m_ENUSBtn);
        else if(m_ENUSBtn->isChecked() == true)
            changeLanguage(m_ZHCNBtn);
        else
            changeLanguage(m_ENUSBtn);
    }
    else if(event->key() == Qt::Key_Enter)
    {
        emit enterpressed();
    }
    else if(event->key() == Qt::Key_Return)
    {
        emit enterpressed();
    }
    else if(event->key() == Qt::Key_Backspace)
    {
        emit backspacepressed();
    }
    else
        QWidget::keyPressEvent(event);
}

void LanguageFrame::changeLanguage(PushButtonCheck* obj)
{
    QString langPath = GetLingmoInstallPath() + "language/";
    emit changeLanguage_sig(obj->objectName());

    if (m_curSystenlocal != obj->objectName()) {
        qApp->removeTranslator(m_pCurTranslator);
//        qApp->removeTranslator(m_pQtTranslator);
    }

    m_curSystenlocal = obj->objectName();
    if (m_pCurTranslator->load(langPath + m_curSystenlocal + ".qm")) {
        qApp->installTranslator(m_pCurTranslator);
    }

//    if (m_pQtTranslator->load(langPath + "qt_"+m_curSystenlocal+".qm")) {
//        qApp->installTranslator(m_pQtTranslator);
//    }

    qDebug()<<"language qm file path="<<langPath;
    qDebug()<<"current language="<<m_curSystenlocal;
    unsetenv("LANG");
    setenv("LANG", obj->objectName().toStdString().c_str(),1);
    unsetenv("LANGUAGE");
    setenv("LANGUAGE", obj->objectName().toStdString().c_str(),1);
    WriteSettingToIni("config", "language", m_curSystenlocal);
    this->setFocus();
    obj->setChecked(true);
    qApp->setProperty("Language", m_curSystenlocal);
}

QString LanguageFrame::setAllBtnStatus(QString strCurrLang)
{
    QString strCurrLangLocal;

    if(strCurrLang.isEmpty())
        strCurrLangLocal = m_ZHCNBtn->objectName();

    if(m_ZHCNBtn->objectName() == strCurrLang)
    {
        m_ZHCNBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_ZHCNBtn->setLabelShow(false);

    if(m_ZHHKBtn->objectName() == strCurrLang)
    {
        m_ZHHKBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_ZHHKBtn->setLabelShow(false);

    if(m_KKKZBtn->objectName() == strCurrLang)
    {
        m_KKKZBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_KKKZBtn->setLabelShow(false);

    if(m_UGCNBtn->objectName() == strCurrLang)
    {
        m_UGCNBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_UGCNBtn->setLabelShow(false);

    if(m_KYKGBtn->objectName() == strCurrLang)
    {
        m_KYKGBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_KYKGBtn->setLabelShow(false);

    if(m_BOCNBtn->objectName() == strCurrLang)
    {
        m_BOCNBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_BOCNBtn->setLabelShow(false);

    if(m_MNMNBtn->objectName() == strCurrLang)
    {
        m_MNMNBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_MNMNBtn->setLabelShow(false);

    if(m_ENUSBtn->objectName() == strCurrLang)
    {
        m_ENUSBtn->setLabelShow(true);
        strCurrLangLocal = strCurrLang;
    }
    else
        m_ENUSBtn->setLabelShow(false);

    if(strCurrLangLocal.isEmpty())//默认中文
    {
        m_ZHCNBtn->setLabelShow(true);
        return m_ZHCNBtn->objectName();
    }
    else
        return strCurrLangLocal;
}

}
