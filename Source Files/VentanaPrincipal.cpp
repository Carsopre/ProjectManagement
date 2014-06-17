#include "VentanaPrincipal.h"
#include "ui_VentanaPrincipal.h"
#include <QTextCodec>
#include <stdio.h>

//MAINWINDOW FUNCTIONS AND SLOTS.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    this->setWindowIcon(QIcon("./icons/logo.ico"));
    this->setWindowIconText(QString(tr("Project Manager")));
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    ui->setupUi(this);

    //Initializing variables
    project = new Project();
    festivos = new QVector<QDate>(0);
    laborables = new QVector<QDate>(0);
    workDates = new QVector<QDate>(0);
    weekEnd = new QString("NONE");
    firstCal=firstAn=true;
    proGenerated = false;

    ui->ActivTable->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->actionGuardar_Proyecto->setEnabled(false);

    ui->acGen_Ultimo->setEnabled(false); //At least we need a Starting day.
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    calOp=resOp=gen1Op=gen2Op=act1Op=act2Op=genAnOp=anOp=true;

    //Creating windows
    cal = new calendar(festivos,laborables,weekEnd);
    newAct = new ActivityView(project,ui->ActivTable);
    res = new resourcesView(project);
    options=false;

    posWinX = this->geometry().x();
    posWinY = this->geometry().y();

    connectAll();
}

void MainWindow:: connectAll()
{
    //ICONS
    ui->addButton->setIcon(QPixmap("./icons/add.png"));             ui->analysisButton->setIconSize(QSize(40,40));
    ui->analysisButton->setIcon(QPixmap("./icons/analysis.png"));   ui->analysisButton->setIconSize(QSize(40,40));
    ui->calendarButton->setIcon(QPixmap("./icons/calendar.png"));   ui->calendarButton->setIconSize(QSize(40,40));
    ui->editButton->setIcon(QPixmap("./icons/edit.png"));           ui->editButton->setIconSize(QSize(40,40));
    ui->reportButton->setIcon(QPixmap("./icons/report.png"));       ui->reportButton->setIconSize(QSize(40,40));
    ui->deleteButton->setIcon(QPixmap("./icons/minus.png"));        ui->deleteButton->setIconSize(QSize(40,40));
    ui->resourceButton->setIcon(QPixmap("./icons/resources.png"));  ui->resourceButton->setIconSize(QSize(40,40));
    ui->actionAcerca_de->setIcon(QPixmap("./icons/about.png"));     ui->actionAcerca_de->setIconVisibleInMenu(true);
    ui->actionManual->setIcon(QPixmap("./icons/manual.png"));       ui->actionManual->setIconVisibleInMenu(true);
    ui->actionGuardar_Como->setIcon(QPixmap("./icons/save_file.png"));  ui->actionGuardar_Como->setIconVisibleInMenu(true);
    ui->actionCargar_Proyecto->setIcon(QPixmap("./icons/open_file.png"));   ui->actionCargar_Proyecto->setIconVisibleInMenu(true);
    ui->actionSalir->setIcon(QPixmap("./icons/exit.png"));          ui->actionSalir->setIconVisibleInMenu(true);
    ui->acGen_Informe->setIcon(QPixmap("./icons/report.png"));      ui->acGen_Informe->setIconVisibleInMenu(true);
    ui->actionNuevo_Proyecto->setIcon(QPixmap("./icons/new_file.png")); ui->actionNuevo_Proyecto->setIconVisibleInMenu(true);
    //Tooltips
    ui->addButton->setToolTip(QString("Nueva Actividad"));
    ui->editButton->setToolTip(QString("Editar Actividad"));
    ui->deleteButton->setToolTip(QString("Eliminar Actividad"));
    ui->calendarButton->setToolTip(QString("Mostrar Calendario"));
    ui->resourceButton->setToolTip(QString("Mostrar Recursos"));
    ui->reportButton->setToolTip(QString("Generar Informe"));

    //Buttons activation
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(AddActivity()));
    connect(ui->editButton,SIGNAL(clicked()),this,SLOT(EditActivity()));    
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(DeleteActivity()));
    connect(ui->calendarButton,SIGNAL(clicked()),this,SLOT(OpenCalendar()));
    connect(ui->resourceButton,SIGNAL(clicked()),this,SLOT(OpenResources()));
    connect(ui->acGen_Informe,SIGNAL(triggered()),this,SLOT(GenerateReport()));
    connect(ui->reportButton,SIGNAL(clicked()),this,SLOT(GenerateReport()));
    connect(ui->acGen_Ultimo,SIGNAL(triggered()),this,SLOT(GenerateLast()));
    connect(ui->analysisButton,SIGNAL(clicked()),this,SLOT(OpenAnalysis()));
    //show options
    connect(ui->actionSalir,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionManual,SIGNAL(triggered()),this,SLOT(ShowManual()));
    connect(ui->actionAcerca_de,SIGNAL(triggered()),this,SLOT(ShowAcercaDe()));
    connect(ui->actionAcerca_de_QT,SIGNAL(triggered()),this,SLOT(ShowAcercaDeQT()));
    connect(ui->actionGuardar_Proyecto,SIGNAL(triggered()),this,SLOT(SaveProject()));
    connect(ui->actionGuardar_Como,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(ui->actionCargar_Proyecto,SIGNAL(triggered()),this,SLOT(LoadProject()));
    connect(ui->actionNuevo_Proyecto,SIGNAL(triggered()),this,SLOT(doNewProject()));
    connect(ui->actionModo_Completo,SIGNAL(triggered()),this,SLOT(windowFull()));
    connect(ui->actionModo_Simple,SIGNAL(triggered()),this,SLOT(windowBasic()));
    connect(ui->actionPersonalizar,SIGNAL(triggered()),this,SLOT(showOptions()));
    //Table
    connect(ui->ActivTable,SIGNAL(cellClicked(int,int)),this,SLOT(ChangeCurrentRow(int,int)));
    connect(ui->ActivTable,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(update()));
	

    //ShortCuts
    QShortcut *shortcut1 = new QShortcut(QKeySequence("Ctrl+N"), this);
    QShortcut *shortcut2 = new QShortcut(QKeySequence("Ctrl+E"), this);
    QShortcut *shortcut3 = new QShortcut(QKeySequence("Ctrl+X"), this);
    QShortcut *shortcut4 = new QShortcut(QKeySequence("Ctrl+C"), this);
    QShortcut *shortcut5 = new QShortcut(QKeySequence("Ctrl+R"), this);
    QShortcut *shortcut6 = new QShortcut(QKeySequence("Ctrl+O"), this);
    QShortcut *shortcut7 = new QShortcut(QKeySequence("Ctrl+G"), this); //Generate with last report config
    QShortcut *shortcut8 = new QShortcut(QKeySequence("Ctrl+S"), this);
    QShortcut *shortcut9 = new QShortcut(QKeySequence("Ctrl+L"), this);
    QShortcut *shortcut10 = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QShortcut *shortcut11 = new QShortcut(QKeySequence("Ctrl+A"), this);

    QObject::connect(shortcut1, SIGNAL(activated()), this, SLOT(AddActivity()));
    QObject::connect(shortcut2, SIGNAL(activated()), this, SLOT(EditActivity()));
    QObject::connect(shortcut3, SIGNAL(activated()), this, SLOT(DeleteActivity()));
    QObject::connect(shortcut4, SIGNAL(activated()), this, SLOT(OpenCalendar()));
    QObject::connect(shortcut5, SIGNAL(activated()), this, SLOT(OpenResources()));
    QObject::connect(shortcut6, SIGNAL(activated()), this, SLOT(GenerateReport()));
    QObject::connect(shortcut7, SIGNAL(activated()), this, SLOT(GenerateLast())); //Generate with last report config
    QObject::connect(shortcut8, SIGNAL(activated()), this, SLOT(SaveProject()));
    QObject::connect(shortcut9, SIGNAL(activated()), this, SLOT(LoadProject()));
    QObject::connect(shortcut10, SIGNAL(activated()), this, SLOT(SaveAs()));
    QObject::connect(shortcut11, SIGNAL(activated()), this, SLOT(OpenAnalysis()));
}
MainWindow::~MainWindow(){
    delete ui;
}

//  LOAD & SAVE
Project* MainWindow::loadPro(std::string filename)
{

   int n_activities;
   string name, name2;
   string date_project;
   int t_normal;
   int t_tope;
   int units_max;
   float cost_normal;
   float cost_oportunity;
   int n_resources;
   ifstream infile;
   string line;

   /*Devuelve el proyecto o NULL si no se ha podido cargar*/
   int pos=filename.find_last_of(".gpi");
   if(pos<0)
   {
       showErrorInfo(E_FORMAT_FILE, filename);
       return NULL;
   }
   if(pos+1!=filename.size())
   {
       showErrorInfo(E_FORMAT_FILE, filename);
       return NULL;
   }

   infile.open(filename.c_str(), std::ios_base::binary);

   if(!infile.is_open())
   {
       showErrorInfo(E_OPEN_FILE, filename);
       return NULL;
   }

   Project * p= new Project();

   do
   {
       infile>>line;
   }
   while(line.compare("ACTIVITIES")!=0);

   infile>>n_activities;

   for(int i=0; i<n_activities;i++)
   {
       infile>>name>>t_normal>>cost_normal>>t_tope>>cost_oportunity;
       p->addActivity(name,t_normal,t_tope, cost_normal,cost_oportunity);
   }

   do
   {
       infile>>line;
   }
   while(line.compare("RELATIONS")!=0);
   do
   {
       infile>>name>>name2;
       if(name.compare("RESOURCES")==0)
           break;
       p->addRelation(name, name2);
   }
   while(!name.empty());

   if(name.compare("RESOURCES")!=0)
   {
       do
       {
           infile>>line;
       }
       while(line.compare("RESOURCES")!=0);
       infile>>n_resources;
   }else
       n_resources=atoi(name2.c_str());
   for(int i=0; i<n_resources;i++)
   {
       infile>>name>>units_max;
       p->addResource(name, units_max);
   }

   infile>>name>>name2>>units_max;
   while(!infile.eof()&&name.compare("")!=0)
   {
       p->allocateResourceActivity(name,name2,units_max);
       infile>>name>>name2>>units_max;

   }
   infile.close();
   return p;
}
void MainWindow:: LoadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Project"),"",tr("Project (*.gpi);;All Files (*)"));
    if(fileName.isEmpty())
        return;
    else{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(this,tr("No se puede abrir el fichero"),file.errorString());
            return;
        }
        delete (project);
        project = loadPro(fileName.toStdString());
        if(project==NULL){
            QMessageBox mes;
            mes.warning(0,QString("Aviso"),QString(tr("No se ha podido cargar el fichero")));
            mes.show();
        }else{
            updateProject();
            //QMessageBox::information(this,tr("Se ha cargado correctamente el fichero"),file.errorString());
            QMessageBox mes;
            mes.information(0,QString("Aviso"),QString(tr("Se ha cargado correctamente el fichero")));
            mes.show();
        }

    }
}
void MainWindow::save(std::string filename, Project * p){

   ofstream outfile;

   outfile.open(filename.c_str(), std::ios_base::binary);

   if(!outfile.is_open())
       showErrorInfo(E_OPEN_FILE, filename);
   else
   {
       //outfile<<date_project<<endl;


       outfile<<"ACTIVITIES"<<endl;
       outfile<<p->getActivities().size()<<endl;
       for(int i=0; i<p->getActivities().size();i++)
       {
           outfile<<p->getActivities().at(i)->getName()<<" ";
           outfile<<p->getActivities().at(i)->getTNormal()<<" ";
		   outfile<<p->getActivities().at(i)->getCostNormal()<<" ";
           outfile<<p->getActivities().at(i)->getTTope()<<" ";
           outfile<<p->getActivities().at(i)->getOportunityCost()<<endl;
       }

       outfile<<"RELATIONS"<<endl;
       for(int i=0; i<p->getActivities().size();i++)
           if(!p->getActivities().at(i)->succesorIsEnd())
               for(int j=0; j<p->getActivities().at(i)->getList_Activities_suc()->size();j++)
                   outfile<<p->getActivities().at(i)->getName()<<" "<<p->getActivities().at(i)->getList_Activities_suc()->at(j)->activity->getName()<<endl;


       outfile<<"RESOURCES"<<endl;
       outfile<<p->getResources()->size()<<endl;
       for(int i=0; i<p->getResources()->size();i++)
           outfile<<p->getResources()->at(i)->getName()<<" "<<p->getResources()->at(i)->getUnitsMax()<<endl;

       for(int i=0; i<p->getActivities().size();i++)
           for(int j=0; j<p->getActivities().at(i)->getResources().size();j++)
           {
               outfile<<p->getActivities().at(i)->getResources().at(j)->resource_asig->getName()<<" ";
               outfile<<p->getActivities().at(i)->getName()<<" ";
               outfile<<p->getActivities().at(i)->getResources().at(j)->units_asig<<endl;
           }

   }

   outfile.close();

}
void MainWindow:: SaveAs()
{
    fileName = QFileDialog::getSaveFileName(this,tr("Save Project"),"",tr("GPI files (*.gpi);;All Files (*)"));
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QFileInfo info(file);
        if(info.suffix()!=QString("gpi")){
            fileName.append(".gpi");
            QMessageBox::information(this,tr("Aviso"),tr("Se añade la extension .gpi"));            
        }

        save(fileName.toStdString(),project);
        ui->actionGuardar_Proyecto->setEnabled(true);
    }else{
        return;
    }
}
void MainWindow:: SaveProject()
{
    if(fileName.isEmpty()) SaveAs();
    else{
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
            save(fileName.toStdString(),project);
            ui->actionGuardar_Proyecto->setEnabled(true);
        }else{
            QMessageBox::information(this,tr("Unable to open file"),file.errorString());
            return;
        }
    }

}
// UPDATE & CLEAN
void MainWindow:: doNewProject(){

    //deleting & closing
    delete(project);
    delete(festivos); delete(laborables);

    ui->ActivTable->clearContents();
    ui->ActivTable->setRowCount(0);

    if(!calOp)    cal->close();
    if(!resOp)    res->close();
    if(!gen1Op)   genRep->close();
    if(!gen2Op)   genAgain->close();
    if(!genAnOp)  genAn->close();
    if(!act1Op)   newAct->close();
    if(!act2Op){  editAct->close();delete(editAct);}

    delete(cal);    delete(newAct);    delete(res);

    //initializing
    festivos = new QVector<QDate>(0);
    laborables = new QVector<QDate>(0);
    workDates = new QVector<QDate>(0);

    ui->ActivTable->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->actionGuardar_Proyecto->setEnabled(false);
    weekEnd->clear();
    ui->acGen_Ultimo->setEnabled(false); //At least we need a Starting day.
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);

    calOp=resOp=gen1Op=gen2Op=act1Op=act2Op=genAnOp=true;
    options=false;

    project = new Project();
    res=new resourcesView(project);
    cal = new calendar(festivos,laborables,weekEnd);
    newAct = new ActivityView(project,ui->ActivTable);

}
void MainWindow:: updateProject(){
    //Cleaning project

    delete(festivos); delete(laborables);

    ui->ActivTable->clearContents();
    ui->ActivTable->setRowCount(0);

    if(!calOp)    cal->close();
    if(!resOp)    res->close();
    if(!gen1Op)   genRep->close();
    if(!gen2Op)   genAgain->close();
    if(!genAnOp)  genAn->close();
    if(!act1Op)   newAct->close();
    if(!act2Op){  editAct->close();delete(editAct);}

    delete(cal);    delete(newAct);    delete(res);

    ui->ActivTable->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->actionGuardar_Proyecto->setEnabled(false);
    ui->acGen_Ultimo->setEnabled(false); //At least we need a Starting day.

    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    calOp=resOp=gen1Op=gen2Op=act1Op=act2Op=genAnOp=true;
    options=false;
    weekEnd->clear();
    //UpdateMainTable
    updateMainTable();

    //Update Widnows:
    festivos = new QVector<QDate>(0);
    laborables = new QVector<QDate>(0);
    workDates = new QVector<QDate>(0);

    cal = new calendar(festivos,laborables,weekEnd);
    newAct = new ActivityView(project,ui->ActivTable);
    res = new resourcesView(project);
}
void MainWindow:: updateMainTable(){
    //Loading project
    vector<Activity*> vecAct = project->getActivities();
    vector<Resource*> *vecRes = project->getResources();
    int actSize = project->sizeActivities();
    int resSize = project->sizeResources();
    //Update Ventana Principal
    for(int i=0;i<actSize;i++)
    {
        ui->ActivTable->setRowCount(i+1);
        ui->ActivTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(vecAct.at(i)->getName())));

        int tn = vecAct.at(i)->getTNormal();    int tp = vecAct.at(i)->getTTope();
        float cn = vecAct.at(i)->getCostNormal(); float co = vecAct.at(i)->getOportunityCost();

        ui->ActivTable->setItem(i,2,new QTableWidgetItem(QString::number(tn)));
        ui->ActivTable->setItem(i,4,new QTableWidgetItem(QString::number(tp)));
        ui->ActivTable->setItem(i,5,new QTableWidgetItem(QString::number(cn)));
        ui->ActivTable->setItem(i,6,new QTableWidgetItem(QString::number(co)));
        //Resources
        QString *textRes = new QString(" ");
        bool any=false;
        Activity *actAux = vecAct.at(i);

		for(int j=0;j<actAux->getResources().size();j++){
			int auxN = actAux->getResources().at(j)->units_asig;
            if(any)    textRes->append(", ");
            textRes->append(QString::fromStdString(actAux->getResources().at(j)->resource_asig->getName()));
            textRes->append("(");textRes->append(QString::number(auxN));
            textRes->append(")");				
			any=true;                   
        }
        ui->ActivTable->setItem(i,1,new QTableWidgetItem(*textRes));


        //Predecesores
        QString *textPred = new QString("");
        any=false;
        int nSize=actSize;
        for(int j=0;j<nSize;j++){
            if(vecAct.at(i)->getPositionRelationPredecesor(vecAct.at(j)->getName())>=0){
                if(any) textPred->append("-");
                any=true;
                textPred->append(QString::fromStdString(vecAct.at(j)->getName()));
            }
        }
        ui->ActivTable->setItem(i,3,new QTableWidgetItem(*textPred));
    }
}
// MAIN WINDOW OPTIONS
void MainWindow:: ShowManual()
{
    QString path="/Docs/Manual.pdf";
    QString appath=qApp->applicationDirPath();
    QString fullpath=appath.prepend("file:///")+path;
    QDesktopServices::openUrl(QUrl(fullpath, QUrl::TolerantMode));
}
void MainWindow:: ShowAcercaDe()
{
    QMessageBox aboutBox(this);
    aboutBox.setText(tr("Curso 2012/2013 - ETSINF - UPV.\n"
                        "GPI - Intensifiacion Sistemas de Informacion"));
    aboutBox.setInformativeText(tr(
                        "Este Software ha sido desarrollado con motivo academico"
                        "para facilitar la gestion de proyectos informaticos segun"
                        "los conceptos e ideas impartidas durante el curso en la"
                        "asignatura Gestion de Proyectos Informaticos.\n\n"
                        "Autores: \nPaula Navarro Alfonso\nCarles S. Soriano Perez\n"));
    aboutBox.setWindowTitle(tr("Acerca de"));
    aboutBox.setIconPixmap(QPixmap("./icons/upv.png"))
            ;
    //aboutBox.setIcon();
    aboutBox.exec();
}
void MainWindow:: ShowAcercaDeQT()
{
    QMessageBox aboutMes;
    aboutMes.aboutQt(0,QString("Acerca de QT"));
}
void MainWindow:: showOptions(){

    ui->actionPersonalizar->setEnabled(false);
    optionsWin = new QWidget();
    checks.clear();

    accepOpt = new QPushButton(QString("Aceptar"));
    connect(accepOpt,SIGNAL(clicked()),this,SLOT(windowCustom()));

    for(int i=0;i<6;i++){
        QCheckBox *check = new QCheckBox();
        if(ui->ActivTable->isColumnHidden(i+1))   check->setChecked(false);
        else    check->setChecked(true);
        checks.insert(i,check);
    }
    checks[0]->setText(QString("Recursos"));        checks[1]->setText(QString("Duración"));
    checks[2]->setText(QString("Predecesores"));    checks[3]->setText(QString("T. Tope"));
    checks[4]->setText(QString("Coste Normal"));    checks[5]->setText(QString("COste Oport."));

    QGroupBox *basicOp = new QGroupBox(QString("Básicas"));
    QHBoxLayout *hbox1 = new QHBoxLayout();
        hbox1->addWidget(checks[0]);
        hbox1->addWidget(checks[1]);
        hbox1->addWidget(checks[2]);
        basicOp->setLayout(hbox1);
    QGroupBox *advancedOp = new QGroupBox(QString("Avanzadas"));
    QHBoxLayout *hbox2 = new QHBoxLayout();
        hbox2->addWidget(checks[3]);
        hbox2->addWidget(checks[4]);
        hbox2->addWidget(checks[5]);
        advancedOp->setLayout(hbox2);
    QHBoxLayout *hbox3 = new QHBoxLayout();
        hbox3->addWidget(accepOpt);
    QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget(basicOp);
        vbox->addWidget(advancedOp);
        vbox->addLayout(hbox3);
    optionsWin->setLayout(vbox);
    optionsWin->setWindowTitle(QString("Personalizar Vista"));
    optionsWin->show();
}
void MainWindow:: windowCustom(){
    optionsWin->close();

    for(int i=0;i<6;i++){
        if(!checks[i]->isChecked())
            ui->ActivTable->setColumnHidden(i+1,true);
        else    ui->ActivTable->setColumnHidden(i+1,false);
    }

    ui->actionModo_Simple->setEnabled(true);
    ui->actionModo_Completo->setEnabled(true);
    ui->actionPersonalizar->setEnabled(false);
}
void MainWindow:: windowFull(){
    for(int i=0;i<7;i++)    ui->ActivTable->setColumnHidden(i,false);
    ui->actionModo_Simple->setEnabled(true);
    ui->actionModo_Completo->setEnabled(false);
    ui->actionPersonalizar->setEnabled(true);
}
void MainWindow:: windowBasic(){

    for(int i=0;i<7;i++){
        if(i<4) ui->ActivTable->setColumnHidden(i,false);
        else    ui->ActivTable->setColumnHidden(i,true);
    }
    ui->actionModo_Simple->setEnabled(false);
    ui->actionModo_Completo->setEnabled(true);
    ui->actionPersonalizar->setEnabled(true);
}
void MainWindow:: ChangeCurrentRow(int row,int col){

     ui->ActivTable->setCurrentCell(row,col);
     ui->ActivTable->selectRow(row);
     ui->editButton->setEnabled(true);
     ui->deleteButton->setEnabled(true);
     ui->analysisButton->setEnabled(true);
     if(row<0){
         ui->editButton->setEnabled(false);
         ui->deleteButton->setEnabled(false);
         ui->analysisButton->setEnabled(false);
     }

}
//ACTIVITY
void MainWindow:: AddActivity(){
    if(!newAct->isVisible()){
        newAct = new ActivityView(project,ui->ActivTable);
        newAct->show();
        //newAct->setGeometry(posWinX+40,posWinY+50,newAct->width(),newAct->height());
        newAct->setFixedSize(newAct->size());
        act1Op = false;
    }
}
void MainWindow:: EditActivity(){
   int row2edit = ui->ActivTable->currentRow();

   if(row2edit>=0){
       act = new Activity();
       act = project->getActivity(ui->ActivTable->item(row2edit,0)->text().toStdString());
       if(act2Op){
           editAct=new ActivityView(project,act,ui->ActivTable,row2edit);
           editAct->show(); editAct->setFixedSize(editAct->size());
           act2Op = false;
       }else{
           if(!editAct->isVisible()){
               editAct = new ActivityView(project,act,ui->ActivTable,row2edit);
               editAct->show(); editAct->setFixedSize(editAct->size());
           }
       }
       //editAct->setGeometry(posWinX+40,posWinY+50,editAct->width(),editAct->height());
       ui->deleteButton->setEnabled(false);
       ui->editButton->setEnabled(false);
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
   }

}
void MainWindow:: DeleteActivity(){

    int row2delete = ui->ActivTable->currentRow();
    if(row2delete>=0){
        QMessageBox mes;
        mes.setText(QString("Eliminar Actividad"));
        mes.setInformativeText(QString(tr("Se borrarán todas las relaciones de esta actividad ¿Esta seguro?\n")));
        QAbstractButton *cancelDelete = mes.addButton(("Cancelar"),QMessageBox::NoRole);
        QAbstractButton *acceptDelete = mes.addButton(("Aceptar"),QMessageBox::YesRole);
        mes.setIcon(QMessageBox::Question);
        mes.exec();
        if(mes.clickedButton()==acceptDelete){
            std::string aux = ui->ActivTable->item(row2delete,0)->text().toStdString();
            project->deleteActivity(aux);
            ui->ActivTable->removeRow(row2delete);
            ui->ActivTable->setCurrentCell(-1,-1);
            ui->deleteButton->setEnabled(false);
            ui->editButton->setEnabled(false);
            updateMainTable();
        }
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
    }

}
// NEW WINDOWS
void MainWindow:: chooseAnalysis(){
    bool cont = true;
    if(!genAnOp){
        genAnOp=true;
        if(genAn->isVisible())  genAn->close();
    }
    if(!anOp && optionsAnalysis->isVisible())    return;


    optionsAnalysis = new QWidget();
    proRadio = new QRadioButton(QString("Analizar Proyecto"));
        proRadio->setChecked(true);
    actRadio = new QRadioButton(QString("Actividad: "));
    actCombo = new QComboBox();

    for(int i=0;i<project->sizeActivities();i++)
        actCombo->addItem(ui->ActivTable->item(i,0)->text());

    contButton = new QPushButton(QString("Continuar"));
    backButton = new QPushButton(QString("Cancelar"));
    connect(contButton,SIGNAL(clicked()),this,SLOT(OpenAnalysis()));
    connect(backButton,SIGNAL(clicked()),optionsAnalysis,SLOT(close()));

    QHBoxLayout *hact = new QHBoxLayout;
        hact->addWidget(actRadio);
        hact->addWidget(actCombo);
    QVBoxLayout *vbox1 = new QVBoxLayout;
        vbox1->addWidget(proRadio);
        vbox1->addLayout(hact);
    QHBoxLayout *hbot = new QHBoxLayout;
         hbot->addWidget(backButton);
        hbot->addWidget(contButton);

    QVBoxLayout *vbox2 = new QVBoxLayout;
        vbox2->addLayout(vbox1);
        vbox2->addLayout(hbot);

    optionsAnalysis->setLayout(vbox2);
    anOp=false;

    optionsAnalysis->setWindowIcon(QIcon("./icons/management.png"));
    optionsAnalysis->setWindowIconText(QString(tr("Project Manager")));
    optionsAnalysis->setWindowTitle(QString("Elige Tipo"));
    optionsAnalysis->show();
    //optionsAnalysis->setGeometry(posWinX+100,posWinY+100,206,110);

}

void MainWindow:: OpenAnalysis(){

    //optionsAnalysis->close();

    if(project->getEnd()->getTMax()<0){//
        QMessageBox::information(this,QString("Información"),QString("Debe de generar el proyecto antes."));
    }else{
        if(!genAnOp){
            genAnOp=true;
            genAn->close();
        }
        if(firstAn) QMessageBox::information(this,QString("Información"),QString("Los análisis corresponden al último informe generado."));
        genAn = new GenerateAnalysis(project,workDates);
        genAn->show();
        genAnOp=firstAn=false;
        genAn->setFixedSize(genAn->size());
       // genAn->setGeometry(posWinX+50,posWinY+50,genAn->width(),genAn->height());
    }
}

//CALENDAR
void MainWindow:: OpenCalendar(){
    if(firstCal)    QMessageBox::information(this,QString("Información"),QString("Se han introducido los festivos Nacionales de España"));
    firstCal=false;
    if(!calOp){
        if(cal->isVisible()){
            cal->close();
            calOp=true;
        }else{
            cal->show();
            calOp=false;
        }
    }else{
        cal->show();    cal->setFixedSize(cal->size());
        calOp=false;
    }
    //cal->setGeometry(posWinX+80,posWinY+20,cal->width(),cal->height());
}

//RESOURCES
void MainWindow:: OpenResources(){
    
	res->show();			 	
	if(resOp)			connect(res,SIGNAL(resDeleted()),this,SLOT(resourceDeleted()));
	resOp=false;   
		
    //res->setGeometry(posWinX+20,posWinY+20,res->width(),res->height());
}

//REPORT
void MainWindow:: GenerateReport(){    
    if(project->sizeActivities()==0){
        QMessageBox mes;
        mes.information(0,QString("Información"), QString("El proyecto debe tener actividades."));
        return;
    }
    if(!gen1Op){
        if(genRep->isVisible()){
            genRep->close();
            gen1Op=true;
        }else{
            genRep->show();
            gen1Op=false;
        }
    }else{
        genRep = new Window(project,festivos,laborables,weekEnd,0);
        genRep->show();
        ui->acGen_Ultimo->setEnabled(false);
        connect(genRep->save,SIGNAL(clicked()),this,SLOT(changeReport()));
        connect(genRep->submit,SIGNAL(clicked()),this,SLOT(changeReport()));
        connect(genRep->cancel,SIGNAL(clicked()),this,SLOT(changeReport2()));
        //gen1Op=false;
    }
    //genRep->setGeometry(posWinX+20,posWinY+20,genRep->width(),genRep->height());
}
void MainWindow:: GenerateLast(){
     if(!gen2Op){
         genAgain->generate();
         updateMainTable();
         workDates=genRep->workDays;
     }
}
void MainWindow:: changeReport(){
    //Means we've saved the options
    ui->acGen_Informe->setEnabled(true);
    ui->acGen_Ultimo->setEnabled(true);
    gen1Op=true;
    gen2Op=false;
    this->genAgain=genRep;
    //Patch here.
    workDates=genRep->workDays;
    updateMainTable();

}
void MainWindow:: changeReport2(){
    if(!ui->acGen_Informe->isEnabled()){
        ui->acGen_Informe->setEnabled(true);
    }
}

