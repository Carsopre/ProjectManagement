/********************************************************************************
** Form generated from reading UI file 'calendar.ui'
**
** Created: Sun 16. Dec 22:53:53 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALENDAR_H
#define UI_CALENDAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_calendar
{
public:
    QFrame *frame;
    QTabWidget *tabWidget;
    QWidget *CalTab;
    QCalendarWidget *MyCal;
    QWidget *FesTab;
    QTableWidget *FesTable;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *addFes_Button;
    QPushButton *delFes_Button;
    QPushButton *updFes_Button;
    QWidget *LabExTab;
    QTableWidget *LabTable;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *addLab_Button;
    QPushButton *delLab_Button;
    QPushButton *updLab_Button;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QCheckBox *Sab_Labo;
    QCheckBox *Dom_Labo;

    void setupUi(QWidget *calendar)
    {
        if (calendar->objectName().isEmpty())
            calendar->setObjectName(QString::fromUtf8("calendar"));
        calendar->resize(440, 232);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(calendar->sizePolicy().hasHeightForWidth());
        calendar->setSizePolicy(sizePolicy);
        frame = new QFrame(calendar);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(-1, -1, 531, 241));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 441, 271));
        CalTab = new QWidget();
        CalTab->setObjectName(QString::fromUtf8("CalTab"));
        MyCal = new QCalendarWidget(CalTab);
        MyCal->setObjectName(QString::fromUtf8("MyCal"));
        MyCal->setGeometry(QRect(0, 0, 441, 201));
        tabWidget->addTab(CalTab, QString());
        FesTab = new QWidget();
        FesTab->setObjectName(QString::fromUtf8("FesTab"));
        FesTable = new QTableWidget(FesTab);
        if (FesTable->columnCount() < 2)
            FesTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        FesTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        FesTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        FesTable->setObjectName(QString::fromUtf8("FesTable"));
        FesTable->setGeometry(QRect(0, 0, 341, 201));
        verticalLayoutWidget = new QWidget(FesTab);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(360, 0, 64, 194));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        addFes_Button = new QPushButton(verticalLayoutWidget);
        addFes_Button->setObjectName(QString::fromUtf8("addFes_Button"));
        addFes_Button->setMinimumSize(QSize(0, 60));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../ProjectManagement-build-desktop-Qt_4_8_1_in_PATH__System__Debug/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addFes_Button->setIcon(icon);
        addFes_Button->setIconSize(QSize(40, 40));
        addFes_Button->setFlat(true);

        verticalLayout->addWidget(addFes_Button);

        delFes_Button = new QPushButton(verticalLayoutWidget);
        delFes_Button->setObjectName(QString::fromUtf8("delFes_Button"));
        delFes_Button->setMinimumSize(QSize(0, 60));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../ProjectManagement-build-desktop-Qt_4_8_1_in_PATH__System__Debug/icons/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        delFes_Button->setIcon(icon1);
        delFes_Button->setIconSize(QSize(40, 40));
        delFes_Button->setFlat(true);

        verticalLayout->addWidget(delFes_Button);

        updFes_Button = new QPushButton(verticalLayoutWidget);
        updFes_Button->setObjectName(QString::fromUtf8("updFes_Button"));
        updFes_Button->setMinimumSize(QSize(0, 60));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../ProjectManagement-build-desktop-Qt_4_8_1_in_PATH__System__Debug/icons/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        updFes_Button->setIcon(icon2);
        updFes_Button->setIconSize(QSize(40, 40));
        updFes_Button->setFlat(true);

        verticalLayout->addWidget(updFes_Button);

        tabWidget->addTab(FesTab, QString());
        LabExTab = new QWidget();
        LabExTab->setObjectName(QString::fromUtf8("LabExTab"));
        LabTable = new QTableWidget(LabExTab);
        if (LabTable->columnCount() < 1)
            LabTable->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        LabTable->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        LabTable->setObjectName(QString::fromUtf8("LabTable"));
        LabTable->setGeometry(QRect(0, 30, 341, 171));
        verticalLayoutWidget_2 = new QWidget(LabExTab);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(360, 0, 64, 194));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        addLab_Button = new QPushButton(verticalLayoutWidget_2);
        addLab_Button->setObjectName(QString::fromUtf8("addLab_Button"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addLab_Button->sizePolicy().hasHeightForWidth());
        addLab_Button->setSizePolicy(sizePolicy1);
        addLab_Button->setMinimumSize(QSize(0, 60));
        addLab_Button->setIcon(icon);
        addLab_Button->setIconSize(QSize(40, 40));
        addLab_Button->setFlat(true);

        verticalLayout_2->addWidget(addLab_Button);

        delLab_Button = new QPushButton(verticalLayoutWidget_2);
        delLab_Button->setObjectName(QString::fromUtf8("delLab_Button"));
        sizePolicy1.setHeightForWidth(delLab_Button->sizePolicy().hasHeightForWidth());
        delLab_Button->setSizePolicy(sizePolicy1);
        delLab_Button->setMinimumSize(QSize(0, 60));
        delLab_Button->setIcon(icon1);
        delLab_Button->setIconSize(QSize(40, 40));
        delLab_Button->setFlat(true);

        verticalLayout_2->addWidget(delLab_Button);

        updLab_Button = new QPushButton(verticalLayoutWidget_2);
        updLab_Button->setObjectName(QString::fromUtf8("updLab_Button"));
        sizePolicy.setHeightForWidth(updLab_Button->sizePolicy().hasHeightForWidth());
        updLab_Button->setSizePolicy(sizePolicy);
        updLab_Button->setMinimumSize(QSize(0, 60));
        updLab_Button->setIcon(icon2);
        updLab_Button->setIconSize(QSize(40, 40));
        updLab_Button->setFlat(true);

        verticalLayout_2->addWidget(updLab_Button);

        horizontalLayoutWidget = new QWidget(LabExTab);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 341, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Sab_Labo = new QCheckBox(horizontalLayoutWidget);
        Sab_Labo->setObjectName(QString::fromUtf8("Sab_Labo"));

        horizontalLayout->addWidget(Sab_Labo);

        Dom_Labo = new QCheckBox(horizontalLayoutWidget);
        Dom_Labo->setObjectName(QString::fromUtf8("Dom_Labo"));

        horizontalLayout->addWidget(Dom_Labo);

        tabWidget->addTab(LabExTab, QString());

        retranslateUi(calendar);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(calendar);
    } // setupUi

    void retranslateUi(QWidget *calendar)
    {
        calendar->setWindowTitle(QApplication::translate("calendar", "Calendario", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(CalTab), QApplication::translate("calendar", "Calendario", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = FesTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("calendar", "Fecha", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = FesTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("calendar", "Motivo", 0, QApplication::UnicodeUTF8));
        addFes_Button->setText(QString());
        delFes_Button->setText(QString());
        updFes_Button->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(FesTab), QApplication::translate("calendar", "Festivos", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = LabTable->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("calendar", "Fecha", 0, QApplication::UnicodeUTF8));
        addLab_Button->setText(QString());
        delLab_Button->setText(QString());
        updLab_Button->setText(QString());
        Sab_Labo->setText(QApplication::translate("calendar", "S\303\241bados", 0, QApplication::UnicodeUTF8));
        Dom_Labo->setText(QApplication::translate("calendar", "Domingos", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(LabExTab), QApplication::translate("calendar", "Laborables Extra", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class calendar: public Ui_calendar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALENDAR_H
