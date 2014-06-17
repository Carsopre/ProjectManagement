#ifndef ACTIVITYVIEW_H
#define ACTIVITYVIEW_H

#include <QWidget>
#include <qevent.h>
#include <QTableWidget>
#include <QMessageBox>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QLayout>
#include "Project.h"
#include "Exceptions.h"

namespace Ui {
class ActivityView;
}

class ActivityView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ActivityView(Project *project, QTableWidget *tabla, QWidget *parent = 0);
    explicit ActivityView(Project *project, Activity *activ,QTableWidget *tabla, int rowtoupdate, QWidget *parent = 0);
    QTableWidget *table;
    Project *pro;
    Activity *act;
    int row2upd;

    ~ActivityView();

private slots:
    void SaveDataClose();
    void UpdateDataClose();
    void ShowPred();
    void ShowRes();
    void SaveRes();
    void SavePred();
    //void SavePred();

protected:
    void closeEvent(QCloseEvent * event){
        if(ResViewVisible)  asigResView->close();
        if(PredViewVisible) asigPredView->close();
        this->close();
    }

private:
    Ui::ActivityView *ui;

    void SetValues();
    void ClearData();
    void connectAll();
    void createResWindow();
    void createPredWindow();

    QWidget *asigResView, *asigPredView;
    bool ResViewVisible, PredViewVisible, edit;
    bool firstRes, firstPred;
    int nSize, resSize;
    QPushButton *resClose, *resSave, *predClose, *predSave;
    QVBoxLayout *toShow;
    QVector<QSpinBox*> spiners; QVector<QLabel*> labels;
    QVector<QCheckBox*>    checks;
};

#endif // ACTIVITYVIEW_H
