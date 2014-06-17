/********************************************************************************
** Form generated from reading UI file 'activityview.ui'
**
** Created: Sun 16. Dec 22:53:53 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTIVITYVIEW_H
#define UI_ACTIVITYVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ActivityView
{
public:
    QWidget *formLayoutWidget;
    QGridLayout *gridLayout;
    QSpinBox *RetardAct;
    QLabel *RetardLab;
    QLabel *DurLab;
    QLabel *OverRunLab;
    QLabel *RecLab;
    QLabel *PredLab;
    QPushButton *AsignarRec_Button;
    QLineEdit *NomAct;
    QLabel *NomLab;
    QDoubleSpinBox *OverRunSpin;
    QSpinBox *DurAct;
    QLabel *PriceLab;
    QPushButton *PredBut;
    QDoubleSpinBox *PriceSpin;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *canc_Button;
    QPushButton *save_Button;
    QWidget *gridLayoutWidget;
    QGridLayout *asignPanel;

    void setupUi(QWidget *ActivityView)
    {
        if (ActivityView->objectName().isEmpty())
            ActivityView->setObjectName(QString::fromUtf8("ActivityView"));
        ActivityView->resize(447, 172);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ActivityView->sizePolicy().hasHeightForWidth());
        ActivityView->setSizePolicy(sizePolicy);
        formLayoutWidget = new QWidget(ActivityView);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 444, 132));
        gridLayout = new QGridLayout(formLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(20, 5, 20, 0);
        RetardAct = new QSpinBox(formLayoutWidget);
        RetardAct->setObjectName(QString::fromUtf8("RetardAct"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RetardAct->sizePolicy().hasHeightForWidth());
        RetardAct->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(RetardAct, 1, 3, 1, 1);

        RetardLab = new QLabel(formLayoutWidget);
        RetardLab->setObjectName(QString::fromUtf8("RetardLab"));

        gridLayout->addWidget(RetardLab, 1, 2, 1, 1);

        DurLab = new QLabel(formLayoutWidget);
        DurLab->setObjectName(QString::fromUtf8("DurLab"));

        gridLayout->addWidget(DurLab, 1, 0, 1, 1);

        OverRunLab = new QLabel(formLayoutWidget);
        OverRunLab->setObjectName(QString::fromUtf8("OverRunLab"));

        gridLayout->addWidget(OverRunLab, 3, 2, 1, 1);

        RecLab = new QLabel(formLayoutWidget);
        RecLab->setObjectName(QString::fromUtf8("RecLab"));

        gridLayout->addWidget(RecLab, 3, 0, 1, 1);

        PredLab = new QLabel(formLayoutWidget);
        PredLab->setObjectName(QString::fromUtf8("PredLab"));

        gridLayout->addWidget(PredLab, 2, 0, 1, 1);

        AsignarRec_Button = new QPushButton(formLayoutWidget);
        AsignarRec_Button->setObjectName(QString::fromUtf8("AsignarRec_Button"));

        gridLayout->addWidget(AsignarRec_Button, 3, 1, 1, 1);

        NomAct = new QLineEdit(formLayoutWidget);
        NomAct->setObjectName(QString::fromUtf8("NomAct"));

        gridLayout->addWidget(NomAct, 0, 1, 1, 2);

        NomLab = new QLabel(formLayoutWidget);
        NomLab->setObjectName(QString::fromUtf8("NomLab"));

        gridLayout->addWidget(NomLab, 0, 0, 1, 1);

        OverRunSpin = new QDoubleSpinBox(formLayoutWidget);
        OverRunSpin->setObjectName(QString::fromUtf8("OverRunSpin"));
        OverRunSpin->setMinimumSize(QSize(120, 0));
        OverRunSpin->setMaximum(1e+08);
        OverRunSpin->setSingleStep(0.5);

        gridLayout->addWidget(OverRunSpin, 3, 3, 1, 1);

        DurAct = new QSpinBox(formLayoutWidget);
        DurAct->setObjectName(QString::fromUtf8("DurAct"));
        sizePolicy1.setHeightForWidth(DurAct->sizePolicy().hasHeightForWidth());
        DurAct->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(DurAct, 1, 1, 1, 1);

        PriceLab = new QLabel(formLayoutWidget);
        PriceLab->setObjectName(QString::fromUtf8("PriceLab"));

        gridLayout->addWidget(PriceLab, 2, 2, 1, 1);

        PredBut = new QPushButton(formLayoutWidget);
        PredBut->setObjectName(QString::fromUtf8("PredBut"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(PredBut->sizePolicy().hasHeightForWidth());
        PredBut->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(PredBut, 2, 1, 1, 1);

        PriceSpin = new QDoubleSpinBox(formLayoutWidget);
        PriceSpin->setObjectName(QString::fromUtf8("PriceSpin"));
        PriceSpin->setMinimumSize(QSize(120, 0));
        PriceSpin->setMaximum(1e+08);
        PriceSpin->setSingleStep(0.5);

        gridLayout->addWidget(PriceSpin, 2, 3, 1, 1);

        horizontalLayoutWidget = new QWidget(ActivityView);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(90, 130, 261, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        canc_Button = new QPushButton(horizontalLayoutWidget);
        canc_Button->setObjectName(QString::fromUtf8("canc_Button"));

        horizontalLayout->addWidget(canc_Button);

        save_Button = new QPushButton(horizontalLayoutWidget);
        save_Button->setObjectName(QString::fromUtf8("save_Button"));

        horizontalLayout->addWidget(save_Button);

        gridLayoutWidget = new QWidget(ActivityView);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(450, 0, 211, 171));
        asignPanel = new QGridLayout(gridLayoutWidget);
        asignPanel->setObjectName(QString::fromUtf8("asignPanel"));
        asignPanel->setContentsMargins(0, 0, 0, 0);

        retranslateUi(ActivityView);

        QMetaObject::connectSlotsByName(ActivityView);
    } // setupUi

    void retranslateUi(QWidget *ActivityView)
    {
        ActivityView->setWindowTitle(QApplication::translate("ActivityView", "A\303\261adir Actividad", 0, QApplication::UnicodeUTF8));
        RetardLab->setText(QApplication::translate("ActivityView", "Retraso Max", 0, QApplication::UnicodeUTF8));
        DurLab->setText(QApplication::translate("ActivityView", "Duracion ", 0, QApplication::UnicodeUTF8));
        OverRunLab->setText(QApplication::translate("ActivityView", "Sobrecoste", 0, QApplication::UnicodeUTF8));
        RecLab->setText(QApplication::translate("ActivityView", "Uso Recursos", 0, QApplication::UnicodeUTF8));
        PredLab->setText(QApplication::translate("ActivityView", "Predecesores", 0, QApplication::UnicodeUTF8));
        AsignarRec_Button->setText(QApplication::translate("ActivityView", "Asignar", 0, QApplication::UnicodeUTF8));
        NomLab->setText(QApplication::translate("ActivityView", "Nombre ", 0, QApplication::UnicodeUTF8));
        OverRunSpin->setSuffix(QString());
        PriceLab->setText(QApplication::translate("ActivityView", "Coste", 0, QApplication::UnicodeUTF8));
        PredBut->setText(QApplication::translate("ActivityView", "Asignar", 0, QApplication::UnicodeUTF8));
        PriceSpin->setSuffix(QString());
        canc_Button->setText(QApplication::translate("ActivityView", "Cancelar", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        save_Button->setToolTip(QApplication::translate("ActivityView", "<html><head/><body><p>Guada los cambios</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        save_Button->setText(QApplication::translate("ActivityView", "Guardar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ActivityView: public Ui_ActivityView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTIVITYVIEW_H
