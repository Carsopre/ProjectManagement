#ifndef GENERATEANALYSIS_H
#define GENERATEANALYSIS_H

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

class QGroupBox;
class GenerateAnalysis : public QWidget
{
    Q_OBJECT

public:
   explicit GenerateAnalysis(Project *proyec,QVector<QDate> *workDates, QWidget *parent=0);
    ~GenerateAnalysis();

private slots:
    void changeOptions();
    void refreshFlex(int index);
    void submit();

private:
    //Functions
    void buildFlex();
    void buildVar();
    void buildProb();
    void joinGroups();

    Project *pro;
    Activity *act;
    QVector<QDate> *datesVec;

    QHBoxLayout *hGroups;
    QGroupBox *groupFlex,*groupVar, *groupProb;
    QPushButton *submitAnalysis,*cancelAnalysis;
    //Flex
    QComboBox *actCombo;
    QLineEdit *hlrLine, *hlaLine;
    QRadioButton *forRadio, *backRadio;
    QSpinBox    *forSpinner, *backSpinner;    
    //Var
    QDoubleSpinBox    *realCost,*varian;
    QSpinBox    *realDur;
    //Prob
    QGroupBox *prob1, *prob2, *prob3;
    QRadioButton *radioProb1, *radioProb2, *radioProb3;
    QDoubleSpinBox *probProb;
    QSpinBox *durProb, *int1Prob, *int2Prob;
};
#endif // GENERATEANALYSIS_H

