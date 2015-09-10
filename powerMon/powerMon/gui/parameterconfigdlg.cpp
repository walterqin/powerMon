/**
* @file parameterconfigdlg.cpp
* @brief 参数设置窗口类实现
* @author qgj
* @date 2015-06-15
* @ingroup gui
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-15    QinGenjian    create
*
*/
#include <QtGui>
#include "../database/profile.h"
#include "parameterconfigdlg.h"

/**
* @brief 构造函数
* @param [I] 父指针
*/
ParameterConfigDlg::ParameterConfigDlg(QWidget *parent) :
    ParentWindow(parent)
{
    init();
    setupUi();
}

/**
* @brief 初始化
*/
void ParameterConfigDlg::init()
{
    m_nCount = profile.counts();
    m_nPages = (m_nCount+ItemsPerPage - 1) / ItemsPerPage;
    m_nPage = 0;

    paramKeys = profile.keys();
    foreach (QString key, paramKeys)
    {
        //!!!NOTICE:此处的变量取值和profile.cpp中对数据库中数据读取赋值要相同
        QStringList tmp = profile.value(key).toStringList();
        value << tmp.at(0);
        tip << tmp.at(1);
        editType << tmp.at(2);
    }

    setFocusPolicy(Qt::NoFocus);
}

/**
* @brief 创建窗体控件
*/
void ParameterConfigDlg::createWidget()
{

}

/**
* @brief 窗体布局
*/
void ParameterConfigDlg::setupUi()
{
    defaultTitle();

    m_PageLabel = new QLabel;
    m_PageLabel->setText(QString("%1%2%3")
                         .arg(m_nPage+1, 2, 10,QChar('0'))
                         .arg("/")
                         .arg(m_nPages, 2, 10, QChar('0')));

    QVBoxLayout *v1Layout = new QVBoxLayout;
    QSignalMapper *signalMapper = new QSignalMapper(this);
    int num = m_nCount > ItemsPerPage ? ItemsPerPage : m_nCount;
    QStringList param;
    for (int i = 0; i < ItemsPerPage; i++)
    {
        m_Buttons[i] = new PushButton;
        m_Buttons[i]->setText(QString("配置"));

        m_Tips[i] = new QLabel;
        m_Labels[i] = new QLabel;

        if (i < num)
        {
            m_Tips[i]->setText(tip.at(i));
            m_Labels[i]->setText(value.at(i));
        }
        else
        {
            m_Buttons[i]->hide();
            m_Tips[i]->hide();
            m_Labels[i]->hide();
        }

        m_Buttons[i]->setFixedHeight(50);
        m_Buttons[i]->setFocusPolicy(Qt::NoFocus);
//        m_Buttons[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        m_Buttons[i]->setFixedWidth(80);
        m_Tips[i]->setFixedHeight(50);
        m_Tips[i]->setFocusPolicy(Qt::NoFocus);
//        m_Tips[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        m_Tips[i]->setFixedWidth(470);
        m_Tips[i]->setFrameStyle(QFrame::Panel);
        m_Tips[i]->setWordWrap(true);
        m_Tips[i]->setAlignment(Qt::AlignLeft|Qt::AlignTop);
        m_Labels[i]->setFixedHeight(50);
        m_Labels[i]->setFocusPolicy(Qt::NoFocus);
        m_Labels[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(m_Buttons[i],1);
        hLayout->addWidget(m_Tips[i],8);
        hLayout->addWidget(m_Labels[i],1);
        v1Layout->addLayout(hLayout);
        v1Layout->setAlignment(hLayout,Qt::AlignTop);
        connect(m_Buttons[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(m_Buttons[i],i);
    }

    v1Layout->addStretch(5);
    v1Layout->setAlignment(Qt::AlignTop);
    v1Layout->setContentsMargins(15,0,15,0);
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(onDealItemParams(int)));

    m_RetButton = new PushButton;
    m_RetButton->setIcon(QPixmap("../icons/general/exit.png"));
    m_RetButton->setIconSize(QPixmap("../icons/general/exit.png").size());
    m_RetButton->setFixedSize(60,60);
    m_RetButton->setFocusPolicy(Qt::NoFocus);

    connect(m_RetButton,SIGNAL(pressed()),this,SLOT(updateButtonTip()));
    connect(m_RetButton,SIGNAL(released()),this,SLOT(close()));

    m_LeftButton = new ToolButton;
    m_LeftButton->setArrowType(Qt::LeftArrow);
    m_LeftButton->setFixedSize(60,60);
    m_LeftButton->setFocusPolicy(Qt::NoFocus);
    m_LeftButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    connect(m_LeftButton,SIGNAL(pressed()),this,SLOT(updateButtonTip()));
    connect(m_LeftButton,SIGNAL(released()),this,SLOT(onLeftPage()));

    m_RightButton = new ToolButton;
    m_RightButton->setArrowType(Qt::RightArrow);
    m_RightButton->setFixedSize(60,60);
    m_RightButton->setFocusPolicy(Qt::NoFocus);
    m_RightButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    connect(m_RightButton,SIGNAL(pressed()),this,SLOT(updateButtonTip()));
    connect(m_RightButton,SIGNAL(released()),this,SLOT(onRightPage()));

    QHBoxLayout *h3Layout = new QHBoxLayout;
    h3Layout->addWidget(m_RetButton);
    h3Layout->addStretch();
    h3Layout->addWidget(m_LeftButton);
    h3Layout->addWidget(m_PageLabel);
    h3Layout->addWidget(m_RightButton);
    h3Layout->addStretch();
    h3Layout->setSizeConstraint(QLayout::SetFixedSize);
    h3Layout->setContentsMargins(20,0,20,0);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(myTitleBar);
//    topLayout->addLayout(h1Layout);
//    topLayout->setStretchFactor(h1Layout,1);
//    topLayout->setAlignment(h1Layout,Qt::AlignTop);
    topLayout->addLayout(v1Layout);
    topLayout->setStretchFactor(v1Layout,7);
    topLayout->setAlignment(v1Layout,Qt::AlignVCenter);
    topLayout->addLayout(h3Layout);
    topLayout->setStretchFactor(h3Layout,2);
    topLayout->setAlignment(h3Layout,Qt::AlignBottom);
    topLayout->setSizeConstraint(QLayout::SetMaximumSize);
    topLayout->setContentsMargins(0,0,0,10);

    setLayout(topLayout);

    if(m_nPages==1)
    {
        m_LeftButton->setDisabled(true);
        m_RightButton->setDisabled(true);
    }
    else
    {
        if(m_nPage==0)
            m_LeftButton->setDisabled(true);
        else if(m_nPage == m_nPages-1)
            m_RightButton->setDisabled(true);
    }

    retranslateUi();
}

void ParameterConfigDlg::updateCurPage()
{

}

void ParameterConfigDlg::retranslateUi()
{

}

void ParameterConfigDlg::inputValueRender(QString item, int i)
{
    m_Labels[i]->setText(item);
}


/**
* @brief 左翻页键槽函数
*/
void ParameterConfigDlg::onLeftPage()
{
    m_nPage--;
    if(m_nPage <= 0)
    {
        m_nPage = 0;
        m_LeftButton->setEnabled(false);
        m_RightButton->setEnabled(true);
    }
    else
    {
        m_RightButton->setEnabled(true);
    }

    for(int i = 0; i < ItemsPerPage; i++)
    {
        int index = m_nPage*ItemsPerPage + i;
        if(index < m_nCount)
        {
            m_Buttons[i]->setText(QString("配置"));
            m_Buttons[i]->show();
            m_Tips[i]->setText(tip.at(index));
            m_Tips[i]->show();
            m_Labels[i]->setText(value.at(index));
            m_Labels[i]->show();
        }
        else
        {
            m_Buttons[i]->hide();
            m_Tips[i]->hide();
            m_Labels[i]->hide();
        }
    }
    m_PageLabel->setText(QString("%1%2%3")
                         .arg(m_nPage + 1, 2, 10, QChar('0'))
                         .arg("/").
                         arg(m_nPages, 2, 10, QChar('0')));
    updateMainTip();
}

/**
* @brief 右翻页键槽函数
*/
void ParameterConfigDlg::onRightPage()
{
    m_nPage++;
    if(m_nPage >= m_nPages - 1)
    {
        m_nPage = m_nPages - 1;
        m_RightButton->setEnabled(false);
        m_LeftButton->setEnabled(true);
    }
    else
    {
        m_LeftButton->setEnabled(true);
    }

    for(int i = 0; i < ItemsPerPage; i++)
    {
        int index = m_nPage * ItemsPerPage + i;
        if(index < m_nCount)
        {
            m_Buttons[i]->setText(QString("配置"));
            m_Buttons[i]->show();
            m_Tips[i]->setText(tip.at(index));
            m_Tips[i]->show();
            m_Labels[i]->setText(value.at(index));
            m_Labels[i]->show();
        }
        else
        {
            m_Buttons[i]->hide();
            m_Tips[i]->hide();
            m_Labels[i]->hide();
        }
    }
    m_PageLabel->setText(QString("%1%2%3")
                         .arg(m_nPage + 1, 2, 10, QChar('0'))
                         .arg("/").
                         arg(m_nPages, 2, 10, QChar('0')));
    updateMainTip();
}

void ParameterConfigDlg::onDealItemParams(int i)
{
    int index = m_nPage * ItemsPerPage + i;
    QString key = paramKeys.at(index);
    QString val = value.at(index);

    ParamInputDialog dlg(val);
    if (dlg.exec() == QDialog::Accepted)
    {
        val =  dlg.getInputText();
        value.replace(index, val);
//        qDebug() << value << index << key;
        QStringList tmp;
        tmp.append(value.at(index));
        tmp.append(tip.at(index));
        tmp.append(editType.at(index));
        profile.setValue(key, tmp);
    }
    inputValueRender(val, i);
}

void ParameterConfigDlg::changeEvent(QEvent *e)
{

}

/**
* @brief 标题栏
*/
void ParameterConfigDlg::defaultTitle()
{
    myTitleBar->setMyWindowTitle(tr("系统参数设置"));
}
