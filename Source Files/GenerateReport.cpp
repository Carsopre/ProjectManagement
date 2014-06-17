#include "GenerateReport.h"
#include <stdio.h>
Window::Window(Project *proyecto, QVector<QDate> *festivo, QVector<QDate> *laborable,QString *weekEndC, QWidget *parent)
    :QWidget(parent)
{
    //Linking
    this->pro=proyecto;
    this->fest=festivo;
    this->labo=laborable;
    this->weekEnd=weekEndC;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));
    mesOnce=false;
    repGen=false;

    createAndConnect();
}
Window::~Window()
{
    delete this;
}
//Building the window
void Window::createAndConnect()
{
    //Generate windows and stuff
    workDays = new QVector<QDate>(0);
    grid = new QGridLayout;

    modeDebug = new QCheckBox(QString("Modo Debug"));
        modeDebug->setChecked(false);
    chooseDebDir = new QPushButton(QString("Elegir directorio"));
        chooseDebDir->setIcon(QPixmap("./icons/open_file.png"));    //chooseDebDir->setIconSize(QSize(15,15));
    linePath = new QLineEdit();
        linePath->setReadOnly(true);
    submit = new QPushButton();
        submit->setFixedSize(100,108);  submit->setFlat(true);
        submit->setIcon(QPixmap("./icons/generate.png"));    submit->setIconSize(QSize(80,80));

    save = new QPushButton();
        save->setFixedSize(100,50); save->setFlat(true);
        save->setIcon(QPixmap("./icons/save_changes.png"));  save->setIconSize(QSize(50,50));
    cancel = new QPushButton();
        cancel->setFixedSize(100,50);        cancel->setFlat(true);
        cancel->setIcon(QPixmap("./icons/cancel_changes.png")); cancel->setIconSize(QSize(50,50));


    QHBoxLayout *buts1 = new QHBoxLayout;
        buts1->addWidget(cancel);
        buts1->addWidget(save);
        buts1->addWidget(submit);
    QVBoxLayout *vDeb = new QVBoxLayout;
    QHBoxLayout *hDeb = new QHBoxLayout;
        hDeb->addWidget(modeDebug);
        hDeb->addWidget(chooseDebDir);
        vDeb->addLayout(hDeb);
        vDeb->addWidget(linePath);
    QVBoxLayout *buts2 = new QVBoxLayout;
        buts2->addLayout(vDeb);
        buts2->addLayout(buts1);

    Group1 = new QGroupBox();   Group2 = new QGroupBox();
    Group3 = new QGroupBox();   Group4 = new QGroupBox();
    radioRes = new QGroupBox(); listGroup = new QGroupBox();

    //Creating window groups
    createStartDay();
    createMaxOverrun();
    createResourceMan();
    createNivRec();
    createBackForward();

    //Composing the window
    grid->addWidget(StartDay,0,0);
    grid->addWidget(CalcMinCos,0,1);
    grid->addWidget(LimAtAd,1,0);
    grid->addWidget(RetraAdel,1,1);
    grid->addWidget(NivRec,2,0);
    grid->addLayout(buts2,2,1);
    setLayout(grid);
    setWindowTitle(tr("Generar Informe"));
    resize(450,200);

    connect(submit,SIGNAL(clicked()),this,SLOT(generate()));
    connect(save,SIGNAL(clicked()),this,SLOT(close()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(chooseDebDir,SIGNAL(clicked()),this,SLOT(getDebugDir()));
}
void Window::getDebugDir()
{
    QDir directory;
    debPath = QFileDialog::getExistingDirectory(this,tr("Directorio"), directory.path());
    if(!debPath.isNull())   linePath->setText(debPath);
}

void Window::createStartDay()
{
    StartDay = new QGroupBox(tr("Dia deseado inicio"));

    date_iniLa = new QLabel(tr("Fecha Inicio: "));
    date_dd = new QSpinBox;
        date_dd->setMaximum(31);date_dd->setMinimum(1);
        date_dd->setMaximumWidth(50);
    date_mm = new QSpinBox;
        date_mm->setMaximum(12);date_mm->setMinimum(1);
        date_mm->setMaximumWidth(50);
    date_yyyy = new QSpinBox;
        date_yyyy->setMinimum(2012); date_yyyy->setMaximumWidth(110);
        date_yyyy->setMaximum(2100);
    //Let's place the predefined date to the actual day just by doing the following:
        date_dd->setValue(QDate::currentDate().day());
        date_mm->setValue(QDate::currentDate().month());
        date_yyyy->setValue(QDate::currentDate().year());

        hboxDate = new QHBoxLayout;
        hboxDate->addWidget(date_iniLa);
        hboxDate->addWidget(date_dd);  hboxDate->addWidget(date_mm);   hboxDate->addWidget(date_yyyy);
        hboxDate->setAlignment(Qt::AlignLeft);
        StartDay->setLayout(hboxDate);
}
void Window::createMaxOverrun()
{
    CalcMinCos = new QGroupBox(tr("Calcular Minimo Coste ").append(QChar(8364)));
        CalcMinCos->setCheckable(true);
        CalcMinCos->setChecked(false);
        connect(CalcMinCos,SIGNAL(clicked()),this,SLOT(disableOptions()));
    maxOverrun = new QDoubleSpinBox(); maxOverrun->setMaximumWidth(120);
        maxOverrun->setMinimum(0.00);  maxOverrun->setSingleStep(0.05);
        maxOverrun->setMaximum(99999999.99);// maxOverrun->setSuffix(QChar(8364)); //QChar(8364) = €
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *lab = new QLabel("Max. Sobrecoste:   ");
        hbox->addWidget(lab);
        hbox->addWidget(maxOverrun);
        hbox->setAlignment(Qt::AlignLeft);
    CalcMinCos->setLayout(hbox);
}
void Window::createNivRec()
{
    NivRec = new QGroupBox(tr("Nivelacion Recursos"));
        NivRec->setCheckable(true);
        NivRec->setChecked(false);

    incAllowed = new QDoubleSpinBox();
        incAllowed->setMaximum(100);        incAllowed->setMinimum(0);
        incAllowed->setSingleStep(0.05);
        incAllowed->setFixedWidth(100);
        //incAllowed->setSuffix(QString("%"));

    allRes = new QRadioButton(tr("Todos"));
    chooseRes = new QRadioButton(tr("Elige"));
        allRes->setChecked(true);
    connect(chooseRes,SIGNAL(toggled(bool)),this,SLOT(disableOptions()));

    resList = new QListWidget();
    resList->setEnabled(false);
    vector<Resource*> *vecRes = pro->getResources();
    int nvecRes = vecRes->size();
    for(int i=0;i<nvecRes;i++){
        string name = vecRes->at(i)->getName();
        QListWidgetItem *checkbox1 = new QListWidgetItem(QString::fromStdString(name));
        resList->addItem(checkbox1);
            resList->item(i)->setFlags(resList->item(i)->flags() |Qt::ItemIsUserCheckable);
            resList->item(i)->setCheckState(Qt::Unchecked);
    }
        resList->setFixedSize(200,120);

    /*QHBoxLayout *hbox = new QHBoxLayout;
        QLabel *lab = new QLabel("Empeoramiento permitido: % ");
            lab->setAlignment(Qt::AlignLeft);
            hbox->addWidget(lab);       hbox->setMargin(0);
            hbox->addWidget(incAllowed);*/
    QHBoxLayout *hbox1 = new QHBoxLayout;
        hbox1->addWidget(allRes); hbox1->addWidget(chooseRes);
        hbox1->setAlignment(Qt::AlignLeft);
    QVBoxLayout *vbox1 = new QVBoxLayout;
        vbox1->addLayout(hbox1); vbox1->addWidget(resList);
        vbox1->setAlignment(Qt::AlignLeft);
        radioRes->setLayout(vbox1);
        radioRes->setTitle("Aplicar a:");
    QVBoxLayout *vbox = new QVBoxLayout;
      //  vbox->addLayout(hbox);
        vbox->addWidget(radioRes);
    NivRec->setLayout(vbox);

}
void Window::createBackForward()
{
    RetraAdel = new QGroupBox(tr("Retraso-Adelanto"));
        RetraAdel->setCheckable(true);
        RetraAdel->setChecked(false);
        RetraAdel->setEnabled(false);
    connect(RetraAdel,SIGNAL(clicked()),this,SLOT(disableOptions()));

    ser1 = new QRadioButton(tr("En serie"));//tr("En serie")
    par1 = new QRadioButton(tr("En paralelo"));//tr("En paralelo")
    ser2 = new QRadioButton(tr("En serie"));
    par2 = new QRadioButton(tr("En paralelo"));
      ser1->setChecked(true);
      ser2->setChecked(true);

    QVBoxLayout *vbox1 = new QVBoxLayout;
        vbox1->addWidget(ser1);
        vbox1->addWidget(par1);
    QVBoxLayout *vbox2 = new QVBoxLayout;
        vbox2->addWidget(ser2);
        vbox2->addWidget(par2);
    QHBoxLayout *hbox1 = new QHBoxLayout;
        Group3->setLayout(vbox1); Group3->setTitle(tr("Adelanto"));
        Group4->setLayout(vbox2); Group4->setTitle(tr("Atraso"));
        hbox1->addWidget(Group3); hbox1->addWidget(Group4);
    RetraAdel->setLayout(hbox1);
}
void Window::createResourceMan()
{
    LimAtAd = new QGroupBox(tr("Limitar Recursos"));
        LimAtAd->setCheckable(true);
        LimAtAd->setChecked(false);
    connect(LimAtAd,SIGNAL(clicked()),this,SLOT(disableOptions()));

   ser = new QRadioButton(tr("En serie"));
   par = new QRadioButton(tr("En paralelo"));
   multi= new QRadioButton(tr("Multipasada"));
   connect(multi,SIGNAL(toggled(bool)),this,SLOT(disableOptions()));
     ser->setChecked(true);

   priorityRules = new QComboBox;
       priorityRules->addItem("LFT");   priorityRules->addItem("EFT");  priorityRules->addItem("EST");
       priorityRules->addItem("LST");   priorityRules->addItem("HT");   priorityRules->addItem("HL");
       priorityRules->addItem("DUR");   priorityRules->addItem("NSUC"); priorityRules->addItem("DEM");

   QVBoxLayout *vbox1 = new QVBoxLayout;
       vbox1->addWidget(ser);
       vbox1->addWidget(par);
       vbox1->addWidget(multi);
   QVBoxLayout *vbox2 = new QVBoxLayout;
       vbox2->addWidget(priorityRules);
   QHBoxLayout *hbox1 = new QHBoxLayout;
       Group1->setLayout(vbox1); Group1->setTitle(tr("Esquema"));
       Group2->setLayout(vbox2); Group2->setTitle(tr("Regla prioridad"));
       hbox1->addWidget(Group1); hbox1->addWidget(Group2);
   LimAtAd->setLayout(hbox1);
}
//Options & Generate
void Window::disableOptions(){

    if(CalcMinCos->isChecked()&&(!mesOnce)){
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Marcando esta opcion la duracion de\n"
                                                          "las actividades puede verse modificada")));
        mes.show();
        mesOnce=true;
    }
    if(LimAtAd->isChecked()){
        RetraAdel->setEnabled(true);
    }else{
        RetraAdel->setEnabled(false);        RetraAdel->setChecked(false);
    }
    if(multi->isChecked())    Group2->setEnabled(false);
    else if(LimAtAd->isChecked()) Group2->setEnabled(true);

    if(chooseRes->isChecked()){
        resList->setEnabled(true);

    }else{
        resList->setEnabled(false);
    }

}
bool Window::dayValid(QDate date)
{
    //If it's a work day just return TRUE
    for(int i=0;i<labo->size();i++) if(date.toString("dd/MM/yyyy")==labo->value(i).toString("dd/MM/yyyy")) return true;
    //Check in case it's a weekend.
    if(weekEnd!=QString("NONE")){
        if(date.dayOfWeek()==6){
            if(weekEnd==QString("BOTH") || weekEnd==QString("SAT")) return true;
            else    return false;
        }
        if(date.dayOfWeek()==7){
            if(weekEnd==QString("BOTH") || weekEnd==QString("SUN")) return true;
            else    return false;
        }
    }else{
        if(date.dayOfWeek()==6 || date.dayOfWeek()==7 ) return false;
    }

    //Finally check if it's a holiday when there's no work.
    //Check Nacionales.
    int yyyy= date.year();
    for(int i=0;i<2;i++){//calcula para 2 años
        if(date.toString("dd/MM/yyyy")==QString("01/01/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("06/01/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("19/03/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("01/05/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("15/08/").append(QString::number(yyyy)))  return false;
        if(date.toString("dd/MM/yyyy")==QString("12/10/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("01/11/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("06/12/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("08/12/").append(QString::number(yyyy)))   return false;
        if(date.toString("dd/MM/yyyy")==QString("25/12/").append(QString::number(yyyy)))   return false;
        yyyy+=1;
    }
    //Check additional Holidays.
    for(int i=0;i<fest->size();i++) if(date==fest->value(i)) return false;

    //and so, if there's no other choice we return true because it's a normal boring day...
    return true;
}
void Window::getWorkDays(){
    int auxDura = pro->getEnd()->getTMin();
    int i=0;
    workDays->clear();
    while(!dayValid(startingDate->addDays(i))){
        i++;
    }
    workDays->append(startingDate->addDays(i));
    auxDura=auxDura-1;
    int j=1;
    for(int i=0;i<auxDura;i++){
        j=1;
        while(!dayValid(workDays->at(i).addDays(j))){
            j++;
        }
        workDays->append(workDays->at(i).addDays(j));        
    }
}
void Window::generate(){
    bool valido;    mesOnce=false;
    QMessageBox mes;
	bool showIt=false;
    clear();
    valido = QDate::isValid(date_yyyy->text().toInt(),date_mm->text().toInt(),date_dd->text().toInt());    
    if(!valido){
        mes.warning(0,QString(tr("Error")),QString(tr("Fecha no valida")));        
    }else if(modeDebug->isChecked() && debPath.isNull()){
        mes.warning(0,QString(tr("Error")),QString(tr("No se ha seleccionado ningún directorio\nDesmarque la casilla o seleccione uno")));
    }else{

        startingDate = new QDate(date_yyyy->text().toInt(),date_mm->text().toInt(),date_dd->text().toInt());
		pro->setReportDebug(debPath.toStdString().append("\\"));
		if(modeDebug->isChecked())
			pro->enableDebug();
	
        //Camino crítico
        pro->startDebug("criticalPath");
        pro->calculCriticalPath(); //Realiza el algoritmo del camino crítico
		pro->finishDebug();
       
		getWorkDays();       
        rep = new Report(workDays,pro,0);
        rep->setWindowTitle(QString("Informe Camino Crítico"));
		  repGen=true;
		  rep->show();

        //Mínimo Coste
        if(CalcMinCos->isChecked()){
			showIt=false;
			pro->startDebug("minCostMinDuration");
            if(maxOverrun->value()>0){
                pro->calculPathMinTimeMinCost(maxOverrun->value());
            }else{
                pro->calculPathMinTimeMinCost();
            }
			pro->finishDebug();
						
			getWorkDays();
			rep = new Report(workDays,pro,0);
			rep->setWindowTitle(QString("Informe Mínimo Coste"));
			rep->show();		
        }
        //Limitación
        if(LimAtAd->isChecked()){
			showIt=false;
            int esquema=0;
            if(ser->isChecked())    esquema=_SERIE;
            if(par->isChecked())    esquema=_PARALEL;
            if(multi->isChecked())  esquema=_MULTIPASADA;

            string limName = priorityRules->currentText().toStdString();
            int reglaPrio=0;
            if(limName=="LFT") reglaPrio=MIN_LFT_FIFO;
            if(limName=="EFT") reglaPrio=MIN_EFT_FIFO;
            if(limName=="EST")  reglaPrio=MIN_EST_FIFO;
            if(limName=="LST")  reglaPrio=MIN_LST_FIFO;
            if(limName=="HT")   reglaPrio=MIN_HT_FIFO;
            if(limName=="HL")   reglaPrio=MIN_HL_FIFO;
            if(limName=="DUR")  reglaPrio=MIN_DUR_FIFO;
            if(limName=="NSUC") reglaPrio=MAX_NSUC_FIFO;
            if(limName=="DEM")  reglaPrio=MAX_DEM_FIFO;

			pro->startDebug("limitation");
            if(pro->limitarResources(reglaPrio,esquema))	showIt=true;
			pro->finishDebug();
			if(showIt){
				getWorkDays();
				rep = new Report(workDays,pro,0);
				rep->setWindowTitle(QString("Informe Limitación de Recursos"));
				rep->show();
			}
        }
       
        //Atraso-Adelanto
        if(RetraAdel->isChecked()&& showIt){
			showIt=false;
            int esquema1=0; int esquema2=0;
            if(ser1->isChecked())    esquema1=_SERIE;
            if(par1->isChecked())    esquema1=_PARALEL;
            if(ser2->isChecked())    esquema2=_SERIE;
            if(par2->isChecked())    esquema2=_PARALEL;

           pro->startDebug("compaction");
           if(pro->retrasoAdelantoActivities(esquema2,esquema1))	showIt=true;
           pro->finishDebug();
		   if(showIt){
			   getWorkDays();
			   rep = new Report(workDays,pro,0);
			   rep->setWindowTitle(QString("Informe Atraso-Adelanto"));
			   rep->show();
		   }            
        }

		 //Nivelacion Recursos
        if(NivRec->isChecked()&& showIt){
            showIt=false;
			pro->startDebug("leveling");
            if(allRes->isChecked()){
                if(pro->levelAllResources()) showIt=true;
            }else{
                vector<string> vecResNiv;
                int sizeList = resList->height();
                for(int i=0;i<sizeList;i++){
                    if(resList->item(i)->checkState()>0)    vecResNiv.push_back(resList->item(i)->text().toStdString());
                }
                if(pro->levelResources(vecResNiv))	showIt=true;
            }
         
			pro->finishDebug();
			if(showIt){
				getWorkDays();
				rep = new Report(workDays,pro,0);
				rep->setWindowTitle(QString("Informe Nivelación de Recursos"));
				rep->show();
			}
            //finestra
        }
		if(modeDebug->isChecked())
			pro->disableDebug();
        this->hide();
    }
}
