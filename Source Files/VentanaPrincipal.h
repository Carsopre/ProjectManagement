#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <iostream>
#include <stdlib.h>
#include <qevent.h>
#include <iomanip>
#include <QMainWindow>
#include <QCalendarWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVector>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextBlock>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QDesktopServices>
#include <QUrl>
#include <fstream>



#include "calendar.h"
#include "ResourcesView.h"
#include "GenerateReport.h"
#include "GenerateAnalysis.h"
#include "Project.h"
#include "Report.h"
#include "ActivityView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    int NAct;
    int NCals;
    bool res_op;
    bool cal_op;
    Project *project;
    Activity *act;
    ~MainWindow();

    
private slots:
    void AddActivity();
    void EditActivity();
    void DeleteActivity();
    void OpenCalendar();
    void OpenResources();
    void GenerateReport();
    void GenerateLast();
    void changeReport();
    void changeReport2();
    void ChangeCurrentRow(int row,int column);
    void ShowManual();
    void ShowAcercaDe();
    void ShowAcercaDeQT();
    void LoadProject();
    void SaveProject();
    void SaveAs();
    void doNewProject();
    void windowFull();
    void showOptions();
    void windowCustom();
    void windowBasic();
    void OpenAnalysis();
    void chooseAnalysis();
	void resourceDeleted(){
		updateMainTable();
	};

private:
    void updateProject();
    void updateMainTable();
    void save(std::string filename, Project * p);
    Project * loadPro(std::string filename);

    int posWinX, posWinY;
    calendar *cal;
    QString *weekEnd;
    resourcesView *res;
    Window *genRep, *genAgain;
    ActivityView *newAct;
    ActivityView *editAct;
    GenerateAnalysis *genAn;


    bool calOp,resOp, gen1Op,gen2Op,genAnOp, act1Op,act2Op,anOp;
    bool changes, options, firstCal, firstAn;
    bool proGenerated;
    QString fileName;

    //Choose Analysis Window
    QWidget *optionsAnalysis;
    QRadioButton *proRadio, *actRadio;
    QPushButton *contButton, *backButton;
    QComboBox *actCombo;
    //CustomWindow Options.
    QWidget *optionsWin;
    QVector<QCheckBox*> checks;
    QPushButton *accepOpt;

    QVector<QDate> *festivos;
    QVector<QDate> *laborables;    
    QVector<QDate> *workDates;

    Ui::MainWindow *ui;
    void connectAll();

protected:
    void closeEvent(QCloseEvent * event){

        if(!calOp)    cal->close();
        if(!resOp)    res->close();
        if(!gen1Op)   genRep->close();
        if(!gen2Op)   genAgain->close();
        if(!genAnOp)  genAn->close();
        if(!act1Op)   newAct->close();
        if(!act2Op)   editAct->close();
        //delete(project);
        //delete(act);
        this->close();
    }
};

#endif // MAINWINDOW_H
