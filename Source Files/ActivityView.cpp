#include "ActivityView.h"
#include "ui_activityview.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QVector>


ActivityView::ActivityView(Project *project, QTableWidget *tabla, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivityView)
{
    this->table=tabla;
    this->pro=project;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));
    nSize = pro->getActivities().size();
    resSize = pro->getResources()->size();

    toShow = new QVBoxLayout();
    ResViewVisible=PredViewVisible=edit=firstRes=firstPred=false;

    ui->setupUi(this);

    connectAll();
    connect(ui->save_Button,SIGNAL(clicked()),this,SLOT(SaveDataClose()));
}
ActivityView::ActivityView(Project *project,Activity *activ, QTableWidget *tabla,int rowtoupdate, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivityView)
{
    this->act=activ;
    this->row2upd=rowtoupdate;
    this->table=tabla;
    this->pro=project;
    ui->setupUi(this);

    nSize = pro->getActivities().size();
    resSize = pro->getResources()->size();

    ResViewVisible=PredViewVisible=firstRes=firstPred=false;
    edit=true;

    SetValues();
    connectAll();
    connect(ui->save_Button,SIGNAL(clicked()),this,SLOT(UpdateDataClose()));
}

ActivityView::~ActivityView()
{
    delete ui;
}
//Building window
void ActivityView::connectAll()
{
    connect(ui->PredBut,SIGNAL(clicked()),this,SLOT(ShowPred()));
    connect(ui->AsignarRec_Button,SIGNAL(clicked()),this,SLOT(ShowRes()));
    connect(ui->canc_Button,SIGNAL(clicked()),this,SLOT(close()));
}
void ActivityView::createPredWindow()
{
    asigPredView = new QWidget();

    nSize = pro->getActivities().size();
    for (int i=0;i<nSize;i++){  //Inicialización Vector;
        QCheckBox *check = new QCheckBox();
        std::vector<Activity*> vecActs = pro->getActivities();
        //std::string text = pro->activities.at(i)->getName();
        std::string text = vecActs.at(i)->getName();
        if(edit){
            if(QString::fromStdString(text)!=ui->NomAct->text()){
                check->setText(QString::fromStdString(vecActs.at(i)->getName()));
                if(act->getPositionRelationPredecesor(check->text().toStdString())>=0)
                    check->setChecked(true);
                checks.insert(i,check);
            }else{
                check->setText(QString::fromStdString(vecActs.at(i)->getName()));
                checks.insert(i,check);
                checks[i]->setEnabled(false);

            }
        }else{
            check->setText(QString::fromStdString(vecActs.at(i)->getName()));
            checks.insert(i,check);
        }
    }

    int j = 0;  int it=0;
    if(nSize>=20)    j=5;        //mayor de 20 -> de 5 en 5
    else if(nSize>=10)   j=4;    //mayor de 10 -> de 4 en 4
    else if(nSize>6)    j=3;    //mayor de 5-> de 3 en 3.
    else                    j=2;    //resto de 2-> de 2 en 2.
    QGridLayout *grid = new QGridLayout();
    for (int i=0;i<nSize;i++){
        QGroupBox *first = new QGroupBox(); //Solo para agrupar
        QHBoxLayout *hbox = new QHBoxLayout();
            hbox->addWidget(checks[i]);
            first->setLayout(hbox);         //   first->setTitle(tr("Predecesores: "));
        grid->addWidget(first,(i/j),it);
        it++;
        if(it==j)   it=0;
    }

    QGroupBox *group = new QGroupBox(tr("Asignar: "));
        group->setLayout(grid);
    QHBoxLayout *hbuts = new QHBoxLayout();
        predClose = new QPushButton(tr("Cerrar"));  predClose->setFixedSize(80,40);
        predSave = new QPushButton(tr("Guardar"));  predSave->setFixedSize(80,40);
         hbuts->addWidget(predClose);   hbuts->addWidget(predSave);
        connect(predClose,SIGNAL(clicked()),asigPredView,SLOT(close()));
        connect(predSave,SIGNAL(clicked()),this,SLOT(SavePred()));

    QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget(group);
        vbox->addLayout(hbuts);
    asigPredView->setLayout(vbox);    asigPredView->setMinimumWidth(260);
    asigPredView->setWindowTitle(tr("Asignar Predecesores"));
    asigPredView->show();
    asigPredView->setFixedSize(asigPredView->size());
    PredViewVisible = true;
}
void ActivityView::createResWindow()
{
    asigResView = new QWidget();
    resSize = pro->getResources()->size();
    std::vector<Resource*> *resVec = pro->getResources();
    for(int i=0;i<resSize;i++)
    {
        QLabel *lab=new QLabel(QString::fromStdString(resVec->at(i)->getName()));
        labels.insert(i,lab);
        QSpinBox *spin = new QSpinBox();
        spin->setMaximum(resVec->at(i)->getUnitsMax());
        if(edit){
            if(act->getResource(lab->text().toStdString())){
                int n = act->getResource(lab->text().toStdString())->units_asig;
                spin->setValue(n);
            }

        }
        spiners.insert(i,spin);
    }

    QGridLayout *grid = new QGridLayout();
    int j = 0;  int it=0;
    if(resSize>=20)    j=5;        //mayor de 20 -> de 5 en 5
    else if(resSize>=10)   j=4;    //mayor de 10 -> de 4 en 4
    else if(resSize>6)    j=3;    //mayor de 5-> de 3 en 3.
    else                    j=2;    //resto de 2-> de 2 en 2.
    for (int i=0;i<resSize;i++){
        QGroupBox *first = new QGroupBox(); //Solo para agrupar
            spiners[i]->setMinimum(0);
            spiners[i]->setMaximumWidth(50);
        QHBoxLayout *hbox = new QHBoxLayout();
            hbox->addWidget(labels[i]);
            hbox->addWidget(spiners[i]);
            first->setLayout(hbox);
            first->setTitle(tr("Recursos: "));

        grid->addWidget(first,(i/j),it);
        it++;
        if(it==j)   it=0;
    }
    QGroupBox *group = new QGroupBox(tr("Asignar: "));
        group->setLayout(grid);
    QHBoxLayout *hbuts = new QHBoxLayout();
    resClose = new QPushButton(tr("Cancelar"));
    resSave = new QPushButton(tr("Guardar"));
        hbuts->addWidget(resClose); resClose->setFixedSize(80,40);
        hbuts->addWidget(resSave); resSave->setFixedSize(80,40);
        connect(resClose,SIGNAL(clicked()),asigResView,SLOT(close()));
        connect(resSave,SIGNAL(clicked()),this,SLOT(SaveRes()));

    QVBoxLayout *vbox = new QVBoxLayout();
        vbox->addWidget(group);        vbox->addLayout(hbuts);

    asigResView->setLayout(vbox);    asigResView->setMinimumWidth(260);
    asigResView->setWindowTitle(tr("Asignar Recursos"));
    asigResView->show();
    asigResView->setFixedSize(asigResView->size());
    ResViewVisible=true;
}
void ActivityView::ShowPred()
{
    if(nSize>0 && !PredViewVisible){        createPredWindow(); firstPred=true;}
    else if(PredViewVisible){   asigPredView->close(); PredViewVisible=false;}
    else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("No hay predecesores a asignar")));
    }

}
void ActivityView::ShowRes()
{
    if(resSize>0 && !ResViewVisible){        createResWindow(); firstRes=true;
    }else if(ResViewVisible){    asigResView->close(); ResViewVisible=false;}
    else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("No hay recursos a asignar")));
    }
}
void ActivityView::ClearData()
{
    ui->NomAct->clear();
    ui->DurAct->setValue(0);    ui->RetardAct->setValue(0);
    ui->PriceSpin->setValue(0.00);   ui->OverRunSpin->setValue(0.00);
}
void ActivityView::SetValues()
{
    std::string auxname = act->getName();
    ui->NomAct->setText(QString::fromStdString(auxname));
    ui->NomAct->setDisabled(true);
    ui->DurAct->setValue(act->getTNormal());
	ui->RetardAct->setValue(act->getTNormal()-act->getTTope());
	ui->PriceSpin->setValue(act->getCostNormal());
	ui->OverRunSpin->setValue(act->getOportunityCost());
	
}
//Saving Data
void ActivityView::SavePred()
{
    for(int i=0;i<nSize;i++)
    {
        checks[i]->setChecked(checks[i]->isChecked());
    }

    PredViewVisible=false;
    asigPredView->close();
}
void ActivityView::SaveRes()
{
    for(int i=0;i<resSize;i++)
    {
        spiners[i]->setValue(spiners[i]->text().toInt());
    }
    asigResView->close();
    ResViewVisible=false;
}
void ActivityView::UpdateDataClose()
{
    std::string acname = table->item(row2upd,0)->text().toStdString();   
    int tn = ui->DurAct->value();
    int tp= tn - ui->RetardAct->value();
    if(tp<0){
        QMessageBox mes;
        mes.information(this,QString(tr("Aviso")),QString(tr("El retraso no puede ser mayor que la duración.")));
        mes.show();
    }else{

        //table->setItem(row2upd,0,new QTableWidgetItem(ui->NomAct->text()));
        pro->modifyActivity(acname,tn,tp,ui->PriceSpin->value(),ui->OverRunSpin->value());
        table->setItem(row2upd,2,new QTableWidgetItem(ui->DurAct->text()));
        table->setItem(row2upd,4,new QTableWidgetItem(QString::number(tp)));
        table->setItem(row2upd,5,new QTableWidgetItem(ui->PriceSpin->text()));
        table->setItem(row2upd,6,new QTableWidgetItem(ui->OverRunSpin->text()));
        //Resources
        QString *textRes = new QString("");
        bool any=false;
        if(firstRes){
            for(int i=0;i<resSize;i++){
                if(!act->getResource(labels[i]->text().toStdString())){
					if(spiners[i]->value()>0)
					{
						//If we add by first time this resource to the activity it must be created.
						string nameRes = labels[i]->text().toStdString();
						int units = spiners[i]->value();
						pro->allocateResourceActivity(nameRes,act->getName(),units);
					}
                }else{
                    int newValue = (spiners[i]->value()) - (act->getResource(labels[i]->text().toStdString())->units_asig);
                    if(newValue>0)  pro->incrementUnitsResourceActivity(labels[i]->text().toStdString(),act->getName(),newValue);
                    if(newValue<0)  pro->decrementUnitsResourceActivity(labels[i]->text().toStdString(),act->getName(),-newValue);
				}
                if(spiners[i]->value()>0){
                    if(any) textRes->append(", ");
                    textRes->append(labels[i]->text().append("("));
                    textRes->append(spiners[i]->text().append(") "));
                    any=true;
                }
            }
            table->setItem(row2upd,1,new QTableWidgetItem(*textRes));

        }        
        table->setItem(row2upd,2,new QTableWidgetItem(ui->DurAct->text()));

        //Predecesores
        any=false;
        QString *textPred = new QString("");
        if(firstPred){
            for(int i=0;i<nSize;i++){
                if(checks[i]->isChecked()){
                    if(act->getPositionRelationPredecesor(checks[i]->text().toStdString())<0) {
                        pro->addRelation(checks[i]->text().toStdString(),act->getName());
                    }
                    if(any) textPred->append("-");
                    any=true;
                    textPred->append(checks[i]->text());
                }else{
                    if(act->getPositionRelationPredecesor(checks[i]->text().toStdString())>=0) {
                        pro->deleteRelation(checks[i]->text().toStdString(),act->getName());
                    }
                }
            }
            table->setItem(row2upd,3,new QTableWidgetItem(*textPred));
        }

        ClearData();
        this->close();
    }
}
void ActivityView::SaveDataClose()
{
    if(!(ui->NomAct->text().isEmpty())){
        if(!(pro->getActivity(ui->NomAct->text().toStdString()))){
            int tn = ui->DurAct->value();
            int tp= tn - ui->RetardAct->value();
            if(tp<0){
                QMessageBox mes;
                mes.information(this,QString(tr("Aviso")),QString(tr("El retraso no puede ser mayor que la duración.")));
                mes.show();
            }else{
                int aux = table->rowCount();
                table->setRowCount(aux+1);
                table->setItem(aux,0,new QTableWidgetItem(ui->NomAct->text()));
                //tn = duracio int ; tp = duracio - retrasoMax; cnormal = cost activitat cuoport=sobrecost
                pro->addActivity((ui->NomAct->text()).toStdString(),tn,tp,ui->PriceSpin->value(),ui->OverRunSpin->value());
                table->setItem(aux,4,new QTableWidgetItem(QString::number(tp)));
                table->setItem(aux,5,new QTableWidgetItem(ui->PriceSpin->text()));
                table->setItem(aux,6,new QTableWidgetItem(ui->OverRunSpin->text()));
                //Resources
                QString *textRes = new QString("");
                bool any=false;
                if(firstRes){
                    for(int i=0;i<resSize;i++){
                        if(spiners[i]->value()>0){
                            if(any) textRes->append(", ");
                            textRes->append(labels[i]->text().append("("));
                            textRes->append(spiners[i]->text().append(") "));
                            string nameRes = labels[i]->text().toStdString();
                            string nameAct = ui->NomAct->text().toStdString();
                            int units = spiners[i]->value();
                            pro->allocateResourceActivity(nameRes,nameAct,units);
                            any=true;
                        }
                    }
                }

                table->setItem(aux,1,new QTableWidgetItem(*textRes));
                table->setItem(aux,2,new QTableWidgetItem(ui->DurAct->text()));
                //Predecesores
                any=false;

                QString *textPred = new QString("");
                if(firstPred){
                    for(int i=0;i<nSize;i++){
                        if(checks[i]->isChecked()){
                            if(any) textPred->append("-");
                            any=true;
                            textPred->append(checks[i]->text());
                            pro->addRelation(checks[i]->text().toStdString(),ui->NomAct->text().toStdString());
                        }
                    }
                }
                table->setItem(aux,3,new QTableWidgetItem(*textPred));
                ClearData();
                this->close();
            }
       }else{
            showErrorInfo(E_ACTIVITY_ALREADY_EXIST,"");
        }
    }else{
        QMessageBox *help = new QMessageBox();
        help->information(0,QString("Ayuda"),QString("Dale nombre a la actividad"));
    }
}
