#ifndef REPORT_H
#define REPORT_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QCalendarWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QRadioButton>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTextFormat>
#include <Project.h>
#include <time.h>
#include <QMessageBox>
#include <qcustomplot.h>
#include <QComboBox>

class QGroupBox;
class Report : public QWidget
{
    Q_OBJECT

public:
    Report(QVector<QDate> *datesVec, Project *proyecto, QWidget *parent);
    ~Report();
    void clear(){
        this->workDays->clear();

    }
    QVector<QDate> *workDays;

protected:
    void closeEvent(QCloseEvent * event){
        if(ganttOpened) ganttWindow->close();
        if(resOpened)   customPlot->close();
        this->close();
    }

private slots:    
    void showGantt();       //Muestra el gráfico de Gantt
    void showResources();   //Muestra el gráfico de gestión de Recursos
    void zoomGanttIn(){
        if(maxZoomGantt==3) return;
        int h = ganttTable->rowHeight(0);
        for(int i=0;i<ganttTable->rowCount();i++)
            ganttTable->setRowHeight(i,h*2);
        int w =ganttTable->columnWidth(0);
        for(int i=0;i<ganttTable->columnCount();i++)
            ganttTable->setColumnWidth(i,w*2);
        maxZoomGantt++;
    }
    void zoomGanttOut(){
        if(maxZoomGantt==0) return;
        int h = ganttTable->rowHeight(0);
        for(int i=0;i<ganttTable->rowCount();i++)
            ganttTable->setRowHeight(i,h/2);
        int w =ganttTable->columnWidth(0);
        for(int i=0;i<ganttTable->columnCount();i++)
            ganttTable->setColumnWidth(i,w/2);
        maxZoomGantt--;
    }
    void showCritPath(){     //Muestra el diálogo con distintos caminos críticos.
		QMessageBox *mes = new QMessageBox();
		QString CamCritic("");
		int camCrits = pro->getCriticalPaths()->size();
		for(int i=0;i<camCrits;i++){
				//if(i>0 && i<camCrits) CamCritic.append(QString::fromStdString("\n"));
				CamCritic.append(QString("\nC. Crítico ").append(QString::number(i+1).append(QString(": "))));
				CamCritic.append(QString::fromStdString(pro->getCriticalPaths()->at(i)->toString()));
		}
		mes->setMinimumWidth(200);
		mes->information(0,QString("C. Crítico"),CamCritic);
	}
	void refreshCCrit(int op){
		int nAct = pro->sizeActivities();
		vector<Activity*> vecAct = pro->getActivities();
		for(int i=0;i<nAct;i++){					
			if(pro->getCriticalPaths()->at(op)->includesActivity(vecAct.at(i))) paintActivity(i,vecAct.at(i)->getTMin(),vecAct.at(i)->getTNormal(),true);
			else      paintActivity(i,vecAct.at(i)->getTMin(),vecAct.at(i)->getTNormal(),false);
		}
		ganttTable->update();
	}

private:
    void paintActivity(int row, int from, int to, bool crit);
    void saveReport();
    void genCalendar();     //Genera el calendario con las fechas del proyecto marcadas
    void genTableActiv();    //Genera la lista de actividades con sus fechas incio - fin
    void genGantt();        //Genera el diagrama de Gantt
    void genResources();    //Genera el diagrama de asignación de recursos.

private:

    Project *pro;
    QGridLayout *grid;
    QPushButton *print;
    bool ganttOpened, resOpened;
    QPushButton *showCrit;
	QComboBox *comboCrit;

    QGroupBox *GroupCal;
    QCalendarWidget *Cal;   QListWidget *calList;
    QTextCharFormat format;
    QBrush m_transparentBrush;    QBrush brus;
    QPainter painter;    QRect rect;

    QGroupBox *GroupList;
    QTableWidget *actTable; QVector<QLabel*> headerNames;
    QPushButton *OpenGant;  QPushButton *OpenRes; //Si se apretan debe cambiar a cerrar ventana.


    /*GANTT*/
    int maxZoomGantt;
    QWidget *ganttWindow;   QTableWidget *ganttTable;
    QPushButton *zoomIn, *zoomOut;
    /*Resources*/
    int maxZoomRes;
    QCustomPlot *customPlot;
    QPushButton *zoom2In, *zoom2Out;
    QGroupBox *GroupResources;
};


#endif // REPORT_H
