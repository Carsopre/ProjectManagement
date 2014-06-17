#include "Report.h"
#include <stdio.h>


Report::Report(QVector<QDate> *datesVec,Project *proyecto,QWidget *parent)
    :QWidget(parent)
{
    //Linking
    this->workDays=datesVec;
    this->pro = proyecto;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));

    //Creating Windows & stuff
    maxZoomGantt=maxZoomRes=2; //this means it's in state 2, but it's value can be among 0 and 4.
    grid = new QGridLayout;
    print = new QPushButton(tr("Imprimir Informe"));
    GroupCal = new QGroupBox();    GroupList = new QGroupBox();
    GroupResources = new QGroupBox();

    genCalendar();
    genTableActiv();
    genGantt();
    genResources();

    resOpened=ganttOpened = false;

    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(GroupList);
        vbox->addWidget(GroupCal);


    setLayout(vbox);
	this->setMinimumWidth(550);

    //this->adjustSize();

    //Listenners
    connect(OpenGant,SIGNAL(clicked()),this,SLOT(showGantt()));
    connect(OpenRes,SIGNAL(clicked()),this,SLOT(showResources()));
}
Report::~Report(){
    delete this;
}

//PRIVATE SLOTS
void Report::showGantt()
{
    if(!ganttOpened){
        ganttOpened=true;
        ganttWindow->show();
    }else{
        ganttWindow->show();
    }


}
void Report::showResources()
{
QMessageBox mes;
    if(!pro->sizeResources()){
        //QMessageBox mes;
        mes.information(0,QString("No hay recursos"),QString("No puede generarse este informe porqué no hay recursos."));
        return;
    }else{
        if(!resOpened){
            customPlot->show();
            resOpened=true;
        }else   customPlot->show();

    }
}
//PUBLIC FUNCTIONS
void Report:: genCalendar()
{
    GroupCal = new QGroupBox(tr("Calendario Proyecto"));

    OpenGant = new QPushButton();
        OpenGant->setToolTip(QString("Muestra Diagrama\n de Gantt"));
        OpenGant->setIcon(QPixmap("./icons/gantt.png"));    OpenGant->setIconSize(QSize(60,60));
        OpenGant->setFlat(true);
    OpenRes = new QPushButton();
        OpenRes->setToolTip(QString("Muestra Asignacion\n de Recursos"));
        OpenRes->setIcon(QPixmap("./icons/plot_res.png"));        OpenRes->setIconSize(QSize(60,60));
        OpenRes->setFlat(true);

    Cal = new QCalendarWidget(); //w:340 h:200
        Cal->setFirstDayOfWeek(Qt::Monday);  Cal->setGridVisible(true);

    calList = new QListWidget();
    int i=-1;
    do{
        i++;
        brus.setColor(Qt::black);
        format.setForeground(brus);
        format.setBackground(QColor(25,125,35,127));
        Cal->setDateTextFormat(workDays->at(i),format);
        calList->addItem(new QListWidgetItem(workDays->at(i).toString("dd/MM/yyyy")));
    }while(workDays->at(i)!=workDays->last());


    QHBoxLayout *hbox2 = new QHBoxLayout;
        hbox2->addWidget(OpenGant);
        hbox2->addWidget(OpenRes);

    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(Cal);
        vbox->addLayout(hbox2);

    GroupCal->setLayout(vbox);
}

void Report:: genTableActiv()
{
    GroupList = new QGroupBox(tr("Informe Actividades"));
    actTable = new QTableWidget();

    actTable->setColumnCount(8);
    actTable->setRowCount(pro->getActivities().size()+2);

    QStringList listNames;
    listNames<<"Nombre"<<"F. Inicio Temprano"<< "F. Inicio Tardío";
    listNames<<"F. Fin Temprano"<<"F. Fin Tardío"<<"HT"<<"HL"<<"Sucesoras";
        actTable->setHorizontalHeaderLabels(listNames);
    //Análisis básico
    showCrit = new QPushButton(QString("Muestra caminos cricos"));
    showCrit->setFixedSize(200,40);
    connect(showCrit,SIGNAL(clicked()),this,SLOT(showCritPath()));

    int camCrits = pro->getCriticalPaths()->size();
    QString CamCritic("");
    if(camCrits==1)  CamCritic.append(QString::fromStdString(pro->getCriticalPaths()->at(0)->toString()));


    QString fechaIni("Fecha Inicio: "); fechaIni.append(workDays->first().toString("dd/MM/yyyy"));
    QString fechaFin(" Fecha fin: ");   fechaFin.append(workDays->last().toString("dd/MM/yyyy"));
    QString fechaIniFin; fechaIniFin = fechaIni+fechaFin;
    QString dura("Duración: ");

    dura.append(QString::number(pro->getEnd()->getTMax()).append(" días laborables"));

    int nAct = pro->sizeActivities();
    vector<Activity*> vecAct = pro->getActivities();
    for(int i=0;i<nAct;i++){
        actTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(vecAct.at(i)->getName())));
        actTable->setItem(i,1,new QTableWidgetItem(workDays->at(vecAct.at(i)->getEarlyStart()).toString("dd/MM/yyyy")));
        actTable->setItem(i,2,new QTableWidgetItem(workDays->at(vecAct.at(i)->getLateStart()).toString("dd/MM/yyyy")));
        actTable->setItem(i,3,new QTableWidgetItem(workDays->at(vecAct.at(i)->getEarlyEnd()-1).toString("dd/MM/yyyy")));
        actTable->setItem(i,4,new QTableWidgetItem(workDays->at(vecAct.at(i)->getLateEnd()-1).toString("dd/MM/yyyy")));
        actTable->setItem(i,5,new QTableWidgetItem(QString::number(vecAct.at(i)->getHTotal())));
        actTable->setItem(i,6,new QTableWidgetItem(QString::number(vecAct.at(i)->getHFree())));
        actTable->setItem(i,7,new QTableWidgetItem(QString::fromStdString(vecAct.at(i)->succesorsToString())));
    }


    QVBoxLayout *vbox1 = new QVBoxLayout;
    if(camCrits==1){ vbox1->addWidget(new QLabel(CamCritic));
    }else{    vbox1->addWidget(showCrit);}
        vbox1->addWidget(new QLabel(fechaIniFin));
        vbox1->addWidget(new QLabel(dura));
        QString total("");
        if(pro->getOverrun()>0){
            total.append("Sobrecoste:").append(QString::number(pro->getOverrun())).append(" euros              ");
        }
        if(pro->getTotalCost()>pro->getOverrun()){
            total.append("Coste Total:").append(QString::number(pro->getTotalCost())).append("euros");
        }
        if(pro->getOverrun()>0||pro->getTotalCost()>pro->getOverrun())	vbox1->addWidget(new QLabel(total));
        vbox1->addWidget(actTable);
    GroupList->setLayout(vbox1);
}

void Report:: paintActivity(int row, int from, int to, bool crit){
    
	for(int i=from;i<from+to;i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        ganttTable->setItem(row,i,item);
        if(crit)	ganttTable->item(row,i)->setBackgroundColor(QColor(220,0,0,175));
		else	ganttTable->item(row,i)->setBackgroundColor(QColor(100,200,215,175));
    }
}

void Report:: genGantt()
{
    ganttWindow = new QWidget();

    zoomIn = new QPushButton();
        zoomIn->setIcon(QPixmap("./icons/zoom_in"));
        zoomIn->setFixedSize(40,40);
        zoomIn->setIconSize(QSize(40,40));    zoomIn->setFlat(true);
    zoomOut = new QPushButton();
        zoomOut->setIcon(QPixmap("./icons/zoom_out"));  zoomOut->setFixedSize(40,40);
        zoomOut->setIconSize(QSize(40,40));   zoomOut->setFlat(true);
        connect(zoomIn,SIGNAL(clicked()),this,SLOT(zoomGanttIn()));
        connect(zoomOut,SIGNAL(clicked()),this,SLOT(zoomGanttOut()));



    int nAct = pro->sizeActivities();
    ganttTable = new QTableWidget();
    ganttTable->setRowCount(nAct);  ganttTable->setColumnCount(workDays->size());
    //Columns
    QStringList daysList;
    for(int i=0;i<workDays->size();i++)
        daysList.append(workDays->at(i).toString("dd/MM/yyyy"));
    ganttTable->setHorizontalHeaderLabels(daysList);
    //rows
    QStringList actList;

    vector<Activity*> vecAct = pro->getActivities();
	//Creating ComboBox to display the Critical paths.
	int camCrits = pro->getCriticalPaths()->size();
	comboCrit = new QComboBox();   
	comboCrit->setFixedWidth(120);
	for(int i=0;i<camCrits;i++){
		comboCrit->addItem(QString("Camino crítico ").append(QString::number(i+1))); 
	}	
	connect(comboCrit,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCCrit(int)));	

    for(int i=0;i<nAct;i++){		
        actList.append(QString::fromStdString(vecAct.at(i)->getName()));		
		if(pro->getCriticalPaths()->at(0)->includesActivity(vecAct.at(i))) paintActivity(i,vecAct.at(i)->getTMin(),vecAct.at(i)->getTNormal(),true);
		else      paintActivity(i,vecAct.at(i)->getTMin(),vecAct.at(i)->getTNormal(),false);
    }
	
    ganttTable->setVerticalHeaderLabels(actList);

	QHBoxLayout *hbox = new QHBoxLayout;
		hbox->addWidget(comboCrit);
        hbox->addWidget(zoomIn);        hbox->addWidget(zoomOut);
        hbox->setAlignment(zoomIn,Qt::AlignRight);
        hbox->setAlignment(zoomOut,Qt::AlignLeft);

    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hbox);
        vbox->addWidget(ganttTable);

    ganttWindow->setLayout(vbox);

    ganttWindow->setWindowIcon(QIcon("./icons/management.png"));
    ganttWindow->setWindowIconText(QString(tr("Project Manager")));
    //ganttWindow->setMinimumWidth(300);    ganttWindow->setMinimumHeight(250);
    ganttWindow->setMinimumSize(700,500);
    ganttWindow->setWindowTitle(QString("Diagrama Gantt"));
}

void Report:: genResources()
{
    int nvecRes = pro->sizeResources();
    vector<Resource*> *vecRes = pro->getResources();

    customPlot= new QCustomPlot();
    // give the axes some labels:
    customPlot->xAxis->setLabel("Días");
    customPlot->yAxis->setLabel("Recursos Utilizados");
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setPositionStyle(QCPLegend::psTopRight);
    QPen pen;        QStringList lineNames;

    int xLength = pro->getEnd()->getTMin();
    int max1=0;
    for(double i=0;i<nvecRes;i++){
        lineNames<< QString::fromStdString(vecRes->at(i)->getName());
        //generate data
        QVector<double> yValRes = QVector<double>::fromStdVector(pro->getAllocationResourcePerDay(vecRes->at(i)->getName()));
        QVector<double> xVal(xLength);
        //paint plot for this resource
        customPlot->addGraph();
        pen.setColor(QColor(sin(i*2)*80+80, sin(i*3)*80+80, sin(i*5)*80+80));
        pen.setWidthF(2.5);
        customPlot->graph()->setName(lineNames.at(i));
        customPlot->graph()->setPen(pen);
        customPlot->graph()->setLineStyle(QCPGraph::lsStepLeft);
        customPlot->graph()->setScatterStyle(QCP::ssNone);
        for(int j=0;j<xLength;j++){
            if(yValRes.toStdVector().at(j)>max1)
                max1=yValRes.toStdVector().at(j);
            xVal[j]=j;
        }
        customPlot->graph()->setData(xVal,yValRes);
        customPlot->graph()->rescaleAxes(true);

    }
    customPlot->xAxis->setRange(0, xLength+2);
    customPlot->yAxis->setRange(0, max1);
    customPlot->yAxis->scaleRange(1, customPlot->yAxis->range().center());
    customPlot->xAxis->scaleRange(1, customPlot->xAxis->range().center());
    QVector<QString> dayLabel;
    customPlot->xAxis->setAutoTickStep(false); customPlot->xAxis->setTickStep(2);
    customPlot->yAxis->setAutoTickStep(false); customPlot->yAxis->setTickStep(2);

    for(int i=0;i<workDays->size();i+=2) dayLabel.append(workDays->at(i).toString("dd/MM/yyyy"));
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVectorLabels(dayLabel);

    customPlot->xAxis->setTickLabelRotation(70);

    customPlot->setWindowTitle(QString("Histograma Uso de Recursos"));
    customPlot->setGeometry(this->geometry().x()+100,this->geometry().y()+100,customPlot->width(),customPlot->height());

}
