#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDate>
#include <QComboBox>
#include <QMessageBox>

#include "Project.h"
#include "Rules.h"
#include <time.h>
#include <Report.h>
#include <QDir>
#include <QFileDialog>


class QGroupBox;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(Project *proyecto, QVector<QDate> *festivo, QVector<QDate> *laborable, QString *weekEndC, QWidget *parent);
    ~Window();
    void updateProject(Project *proyecto){ this->pro=proyecto; }
    void updateDates(QVector<QDate> *festivo, QVector<QDate> *laborable){
        this->fest=festivo;
        this->labo=laborable;
    }
    void clear(){
        this->workDays->clear();
        workDays = new QVector<QDate>(0);
    }

    bool OptionsSaved;
    QVector<QDate> *workDays;
    QPushButton *save, *submit, *cancel;    

protected:
    void closeEvent(QCloseEvent * event){
        if(repGen)  rep->close();
        this->close();
    }

public slots:
    void generate();
private slots:
    void disableOptions();
    void getDebugDir();

private:
    void createStartDay();
    void createMaxOverrun();
    void createResourceMan();
    void createNivRec();
    void createBackForward();
    void getWorkDays();
    bool dayValid(QDate date);
    void createAndConnect();

    QVector<QDate> *fest;
    QVector<QDate> *labo;

    QDate  *startingDate;
    QString *weekEnd;
    Report *rep;
    Project *pro;
    bool repGen;
    //Main window
    QGridLayout *grid;
    QCheckBox *modeDebug;   QString debPath;
    QPushButton *chooseDebDir; QLineEdit *linePath;
    //Groups and stuff
    QGroupBox *StartDay;
    QGroupBox *CalcMinCos;
    QGroupBox *LimAtAd;    QGroupBox *Group1, *Group2, *Group3, *Group4;
    QGroupBox *RetraAdel;      QGroupBox *NivRec;
    //Date
    QLabel *date_iniLa;   QHBoxLayout *hboxDate;
    QSpinBox *date_dd;    QSpinBox *date_mm;    QSpinBox *date_yyyy;
    //Minimo Coste
    QDoubleSpinBox  *maxOverrun;
    //Serie
    QRadioButton *ser;    QRadioButton *par;    QRadioButton *multi;
    QComboBox   *priorityRules;
    //Atraso-Adelanto
    QRadioButton *ser1, *ser2;    QRadioButton *par1, *par2;
    QComboBox   *priorityRules1, *priorityRules2;
    QLabel  *priorityLab1, *priorityLab2;
    //Nivelacion
    bool mesOnce;
    QDoubleSpinBox  *incAllowed;
    QGroupBox *radioRes;
    QRadioButton *allRes, *chooseRes;
    QGroupBox *listGroup;  QListWidget *resList;
};

#endif // WINDOW_H
