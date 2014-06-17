/********************************************************************************
** Form generated from reading UI file 'VentanaPrincipal.ui'
**
** Created: Sat 15. Dec 18:51:28 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANAPRINCIPAL_H
#define UI_VENTANAPRINCIPAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionGuardar_Proyecto;
    QAction *actionCargar_Proyecto;
    QAction *actionSalir;
    QAction *actionGenerar;
    QAction *acGen_Informe;
    QAction *acGen_Ultimo;
    QAction *actionManual;
    QAction *actionAcerca_de;
    QAction *actionAcerca_de_QT;
    QAction *actionGuardar_Como;
    QAction *actionModo_Simple;
    QAction *actionModo_Completo;
    QAction *actionPersonalizar;
    QAction *actionNuevo_Proyecto;
    QWidget *centralWidget;
    QTableWidget *ActivTable;
    QWidget *horizontalLayoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QWidget *horizontalLayoutWidget_2;
    QGridLayout *gridLayout;
    QPushButton *reportButton;
    QPushButton *resourceButton;
    QPushButton *calendarButton;
    QPushButton *analysisButton;
    QMenuBar *menuBar;
    QMenu *menuArchivo;
    QMenu *menuInformes;
    QMenu *menuMostrar;
    QMenu *menuAyuda;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(715, 395);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(715, 395));
        MainWindow->setMaximumSize(QSize(715, 395));
        MainWindow->setIconSize(QSize(24, 24));
        actionGuardar_Proyecto = new QAction(MainWindow);
        actionGuardar_Proyecto->setObjectName(QString::fromUtf8("actionGuardar_Proyecto"));
        actionCargar_Proyecto = new QAction(MainWindow);
        actionCargar_Proyecto->setObjectName(QString::fromUtf8("actionCargar_Proyecto"));
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName(QString::fromUtf8("actionSalir"));
        actionGenerar = new QAction(MainWindow);
        actionGenerar->setObjectName(QString::fromUtf8("actionGenerar"));
        acGen_Informe = new QAction(MainWindow);
        acGen_Informe->setObjectName(QString::fromUtf8("acGen_Informe"));
        acGen_Ultimo = new QAction(MainWindow);
        acGen_Ultimo->setObjectName(QString::fromUtf8("acGen_Ultimo"));
        actionManual = new QAction(MainWindow);
        actionManual->setObjectName(QString::fromUtf8("actionManual"));
        actionAcerca_de = new QAction(MainWindow);
        actionAcerca_de->setObjectName(QString::fromUtf8("actionAcerca_de"));
        actionAcerca_de_QT = new QAction(MainWindow);
        actionAcerca_de_QT->setObjectName(QString::fromUtf8("actionAcerca_de_QT"));
        actionGuardar_Como = new QAction(MainWindow);
        actionGuardar_Como->setObjectName(QString::fromUtf8("actionGuardar_Como"));
        actionModo_Simple = new QAction(MainWindow);
        actionModo_Simple->setObjectName(QString::fromUtf8("actionModo_Simple"));
        actionModo_Completo = new QAction(MainWindow);
        actionModo_Completo->setObjectName(QString::fromUtf8("actionModo_Completo"));
        actionPersonalizar = new QAction(MainWindow);
        actionPersonalizar->setObjectName(QString::fromUtf8("actionPersonalizar"));
        actionNuevo_Proyecto = new QAction(MainWindow);
        actionNuevo_Proyecto->setObjectName(QString::fromUtf8("actionNuevo_Proyecto"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMinimumSize(QSize(712, 0));
        ActivTable = new QTableWidget(centralWidget);
        if (ActivTable->columnCount() < 7)
            ActivTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        ActivTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        ActivTable->setObjectName(QString::fromUtf8("ActivTable"));
        ActivTable->setEnabled(true);
        ActivTable->setGeometry(QRect(0, 50, 711, 321));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(ActivTable->sizePolicy().hasHeightForWidth());
        ActivTable->setSizePolicy(sizePolicy1);
        ActivTable->setSizeIncrement(QSize(10, 10));
        ActivTable->setAutoFillBackground(true);
        ActivTable->setStyleSheet(QString::fromUtf8(""));
        ActivTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ActivTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ActivTable->setRowCount(0);
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 200, 51));
        gridLayout_2 = new QGridLayout(horizontalLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        addButton = new QPushButton(horizontalLayoutWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy);
        addButton->setMinimumSize(QSize(0, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8("icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addButton->setIcon(icon);
        addButton->setIconSize(QSize(40, 40));
        addButton->setFlat(true);

        gridLayout_2->addWidget(addButton, 0, 0, 1, 1);

        editButton = new QPushButton(horizontalLayoutWidget);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        sizePolicy.setHeightForWidth(editButton->sizePolicy().hasHeightForWidth());
        editButton->setSizePolicy(sizePolicy);
        editButton->setMinimumSize(QSize(0, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        editButton->setIcon(icon1);
        editButton->setIconSize(QSize(40, 40));
        editButton->setFlat(true);

        gridLayout_2->addWidget(editButton, 0, 1, 1, 1);

        deleteButton = new QPushButton(horizontalLayoutWidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);
        deleteButton->setMinimumSize(QSize(0, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icons/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        deleteButton->setIcon(icon2);
        deleteButton->setIconSize(QSize(40, 40));
        deleteButton->setAutoDefault(false);
        deleteButton->setDefault(false);
        deleteButton->setFlat(true);

        gridLayout_2->addWidget(deleteButton, 0, 2, 1, 1);

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(430, 0, 268, 52));
        gridLayout = new QGridLayout(horizontalLayoutWidget_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        reportButton = new QPushButton(horizontalLayoutWidget_2);
        reportButton->setObjectName(QString::fromUtf8("reportButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../ProjectManagement-build-desktop-Qt_4_8_1_in_PATH__System__Debug/icons/report.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportButton->setIcon(icon3);
        reportButton->setIconSize(QSize(40, 40));
        reportButton->setFlat(true);

        gridLayout->addWidget(reportButton, 0, 1, 1, 1);

        resourceButton = new QPushButton(horizontalLayoutWidget_2);
        resourceButton->setObjectName(QString::fromUtf8("resourceButton"));
        sizePolicy.setHeightForWidth(resourceButton->sizePolicy().hasHeightForWidth());
        resourceButton->setSizePolicy(sizePolicy);
        resourceButton->setMinimumSize(QSize(0, 25));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("icons/resources.png"), QSize(), QIcon::Normal, QIcon::Off);
        resourceButton->setIcon(icon4);
        resourceButton->setIconSize(QSize(40, 40));
        resourceButton->setFlat(true);

        gridLayout->addWidget(resourceButton, 0, 3, 1, 1);

        calendarButton = new QPushButton(horizontalLayoutWidget_2);
        calendarButton->setObjectName(QString::fromUtf8("calendarButton"));
        sizePolicy.setHeightForWidth(calendarButton->sizePolicy().hasHeightForWidth());
        calendarButton->setSizePolicy(sizePolicy);
        calendarButton->setMinimumSize(QSize(0, 25));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("icons/calendar.png"), QSize(), QIcon::Normal, QIcon::Off);
        calendarButton->setIcon(icon5);
        calendarButton->setIconSize(QSize(40, 40));
        calendarButton->setFlat(true);

        gridLayout->addWidget(calendarButton, 0, 2, 1, 1);

        analysisButton = new QPushButton(horizontalLayoutWidget_2);
        analysisButton->setObjectName(QString::fromUtf8("analysisButton"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../ProjectManagement-build-desktop-Qt_4_8_1_in_PATH__System__Debug/icons/analysis.png"), QSize(), QIcon::Normal, QIcon::Off);
        analysisButton->setIcon(icon6);
        analysisButton->setIconSize(QSize(40, 40));
        analysisButton->setFlat(true);

        gridLayout->addWidget(analysisButton, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 715, 25));
        menuBar->setLayoutDirection(Qt::LeftToRight);
        menuBar->setAutoFillBackground(false);
        menuArchivo = new QMenu(menuBar);
        menuArchivo->setObjectName(QString::fromUtf8("menuArchivo"));
        menuInformes = new QMenu(menuBar);
        menuInformes->setObjectName(QString::fromUtf8("menuInformes"));
        menuMostrar = new QMenu(menuInformes);
        menuMostrar->setObjectName(QString::fromUtf8("menuMostrar"));
        menuAyuda = new QMenu(menuBar);
        menuAyuda->setObjectName(QString::fromUtf8("menuAyuda"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuArchivo->menuAction());
        menuBar->addAction(menuInformes->menuAction());
        menuBar->addAction(menuAyuda->menuAction());
        menuArchivo->addAction(actionNuevo_Proyecto);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionGuardar_Proyecto);
        menuArchivo->addAction(actionGuardar_Como);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionCargar_Proyecto);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionSalir);
        menuInformes->addAction(acGen_Informe);
        menuInformes->addAction(acGen_Ultimo);
        menuInformes->addSeparator();
        menuInformes->addAction(menuMostrar->menuAction());
        menuMostrar->addAction(actionModo_Simple);
        menuMostrar->addAction(actionModo_Completo);
        menuMostrar->addAction(actionPersonalizar);
        menuAyuda->addAction(actionManual);
        menuAyuda->addAction(actionAcerca_de);
        menuAyuda->addAction(actionAcerca_de_QT);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Gestor de Proyectos", 0, QApplication::UnicodeUTF8));
        actionGuardar_Proyecto->setText(QApplication::translate("MainWindow", "Guardar Proyecto", 0, QApplication::UnicodeUTF8));
        actionCargar_Proyecto->setText(QApplication::translate("MainWindow", "Cargar Proyecto", 0, QApplication::UnicodeUTF8));
        actionSalir->setText(QApplication::translate("MainWindow", "Salir", 0, QApplication::UnicodeUTF8));
        actionGenerar->setText(QApplication::translate("MainWindow", "Generar", 0, QApplication::UnicodeUTF8));
        acGen_Informe->setText(QApplication::translate("MainWindow", "Generar Informe", 0, QApplication::UnicodeUTF8));
        acGen_Ultimo->setText(QApplication::translate("MainWindow", "Generar \303\232ltimo", 0, QApplication::UnicodeUTF8));
        actionManual->setText(QApplication::translate("MainWindow", "Manual", 0, QApplication::UnicodeUTF8));
        actionAcerca_de->setText(QApplication::translate("MainWindow", "Acerca de", 0, QApplication::UnicodeUTF8));
        actionAcerca_de_QT->setText(QApplication::translate("MainWindow", "Acerca de QT", 0, QApplication::UnicodeUTF8));
        actionGuardar_Como->setText(QApplication::translate("MainWindow", "Guardar Como", 0, QApplication::UnicodeUTF8));
        actionModo_Simple->setText(QApplication::translate("MainWindow", "Modo Simple", 0, QApplication::UnicodeUTF8));
        actionModo_Completo->setText(QApplication::translate("MainWindow", "Modo Completo", 0, QApplication::UnicodeUTF8));
        actionPersonalizar->setText(QApplication::translate("MainWindow", "Personalizar", 0, QApplication::UnicodeUTF8));
        actionNuevo_Proyecto->setText(QApplication::translate("MainWindow", "Nuevo Proyecto", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = ActivTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Actividad", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = ActivTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Recursos", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = ActivTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Duraci\303\263n", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = ActivTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Predecesores", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = ActivTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Tiempo Tope", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = ActivTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Coste N.", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = ActivTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Coste Op.", 0, QApplication::UnicodeUTF8));
        addButton->setText(QString());
        editButton->setText(QString());
        deleteButton->setText(QString());
        reportButton->setText(QString());
        resourceButton->setText(QString());
        calendarButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        analysisButton->setToolTip(QApplication::translate("MainWindow", "Analisis Flexibilidad", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        analysisButton->setText(QString());
        menuArchivo->setTitle(QApplication::translate("MainWindow", "Archivo", 0, QApplication::UnicodeUTF8));
        menuInformes->setTitle(QApplication::translate("MainWindow", "Opciones", 0, QApplication::UnicodeUTF8));
        menuMostrar->setTitle(QApplication::translate("MainWindow", "Mostrar", 0, QApplication::UnicodeUTF8));
        menuAyuda->setTitle(QApplication::translate("MainWindow", "Ayuda", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANAPRINCIPAL_H
