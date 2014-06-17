#include "ResourcesView.h"
#include "ui_ResourcesView.h"
#include <stdio.h>
resourcesView::resourcesView(Project *proyecto, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Resources)
{
    //intializing varibles
    this->pro=proyecto;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));
    //Creating windows
    edit = false;
	resAvailable = new QSpinBox();	resAvailable->setMaximum(99999999);
    resUnlimited = new QCheckBox(); resUnlimited->setText(QString("Recurso Ilimitado"));
    createAddResourceWindow();    
    ui->setupUi(this);

    ui->ResourceTable->clearContents();
    ui->ResourceTable->setRowCount(0);
    updateTable();
    ui->ResourceTable->setEditTriggers(QTableWidget::NoEditTriggers);




    //ICONS
    ui->AddResource_Button->setIcon(QPixmap("./icons/add.png"));           ui->AddResource_Button->setIconSize(QSize(40,40));
    ui->EditResource_Button->setIcon(QPixmap("./icons/edit.png"));			ui->EditResource_Button->setIconSize(QSize(40,40));
	ui->Exit_Button->setIcon(QPixmap("./icons/exit.png"));					ui->Exit_Button->setIconSize(QSize(40,40));
    ui->DeleteResource_Button->setIcon(QPixmap("./icons/minus.png"));           ui->DeleteResource_Button->setIconSize(QSize(40,40));

    //Tooltips
    ui->AddResource_Button->setToolTip(QString("Añadir Recurso"));
    ui->EditResource_Button->setToolTip(QString("Editar recurso"));
    ui->DeleteResource_Button->setToolTip(QString("Eliminar recurso seleccionado"));
    ui->Exit_Button->setToolTip(QString("Salir"));

	ui->DeleteResource_Button->setEnabled(false);
    ui->EditResource_Button->setEnabled(false);
    //Connecting
    connect(ui->AddResource_Button,SIGNAL(clicked()),this,SLOT(createAddResource()));
    connect(ui->DeleteResource_Button,SIGNAL(clicked()),this,SLOT(deleteResource()));
	connect(ui->EditResource_Button, SIGNAL(clicked()),this,SLOT(editResource()));
    //connect(ui->UpdateResource_Button,SIGNAL(clicked()),this,SLOT(editResource()));
    connect(ui->ResourceTable,SIGNAL(cellClicked(int,int)),this,SLOT(changeCurrentRow(int,int)));
    connect(ui->Exit_Button,SIGNAL(clicked()),this,SLOT(close()));
	connect(ui->ResourceTable,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(update()));
}

resourcesView::~resourcesView()
{
    delete ui;
}

void resourcesView:: changeCurrentRow(int row, int col)
{
    ui->ResourceTable->setCurrentCell(row,col);
    ui->ResourceTable->selectRow(row);
	    
	ui->EditResource_Button->setEnabled(true);
	ui->DeleteResource_Button->setEnabled(true);		 
	if(row<0){
		 ui->EditResource_Button->setEnabled(false);
		 ui->DeleteResource_Button->setEnabled(false);		 
	}

}

void resourcesView:: createAddResourceWindow()
{

    newResource = new QWidget();
    //items
    save = new QPushButton(tr("Guardar"));    cancel = new QPushButton(tr("Cancelar"));  


    //layout
    QLabel *resNameLab = new QLabel(tr("Nombre Recurso: "));
    resName = new QLineEdit("");
    QHBoxLayout *hname = new QHBoxLayout;
        hname->addWidget(resNameLab);
        hname->addWidget(resName);
    QHBoxLayout *hbox1 = new QHBoxLayout;
    QLabel *resNLab = new QLabel(tr("Recursos Maximos: "));
        hbox1->addWidget(resNLab);
        hbox1->addWidget(resAvailable);
    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hbox1);
        vbox->addWidget(resUnlimited);
    QHBoxLayout *hbottom = new QHBoxLayout;
        hbottom->addWidget(cancel);
        hbottom->addWidget(save);
    QVBoxLayout *vbox1 = new QVBoxLayout;
        vbox1->addLayout(hname);
        vbox1->addLayout(vbox);
        vbox1->addLayout(hbottom);
    newResource->setWindowTitle(tr("Nuevo Recurso"));
    //connecting
    connect(save,SIGNAL(clicked()),this,SLOT(addResource()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(cancelNewResource()));
    newResource->setWindowIcon(QIcon("./icons/management.png"));
    newResource->setWindowIconText(QString(tr("Project Manager")));
    newResource->setLayout(vbox1);
    newResource->setFixedSize(250,200);
}

void resourcesView:: createAddResource()
{ 
    clearAll();       
    newResource->show();
}

void resourcesView:: addResource()
{
    if(resName->isEnabled()) //New
    {
        if(!(resName->text().isEmpty())){
            if(!(pro->getResource(resName->text().toStdString()))){
                int aux = ui->ResourceTable->rowCount();
                ui->ResourceTable->setRowCount(aux+1);
                QTableWidgetItem *item = new QTableWidgetItem(resName->text());
                item->setFlags(Qt::ItemIsSelectable);
                ui->ResourceTable->setItem(aux,0,item);
                if(resUnlimited->isChecked()){
                    ui->ResourceTable->setItem(aux,1,new QTableWidgetItem(QString("Ilimitado")));
                    pro->addResource(resName->text().toStdString(),99999999);
                }else{
                    ui->ResourceTable->setItem(aux,1,new QTableWidgetItem(resAvailable->text()));
                    pro->addResource(resName->text().toStdString(),resAvailable->text().toInt());
                }
                clearAll();
                newResource->close();
           }else{
                showErrorInfo(E_RESOURCE_ALREADY_EXIST,"");
            }
        }else{
            QMessageBox *help = new QMessageBox();
            help->information(0,QString("Ayuda"),QString("Dale nombre al Recurso"));
        }
	}else{
        int aux = row2edit;
		bool canEdit=true;
		int unAsig=0;
		for(int i=0; i<pro->sizeActivities();i++){			
			if(pro->getActivities().at(i)->getResource(resName->text().toStdString())!=NULL){
				unAsig=pro->getActivities().at(i)->getResource(resName->text().toStdString())->units_asig;
				if(unAsig>0)	canEdit=false;
			}
		}
		if(canEdit){
			QTableWidgetItem *item = new QTableWidgetItem(resName->text());
			item->setFlags(Qt::ItemIsSelectable);
			ui->ResourceTable->setItem(aux,0,item);
			if(resUnlimited->isChecked()){
				ui->ResourceTable->setItem(aux,1,new QTableWidgetItem(QString("Ilimitado")));
				pro->modifyResource(resName->text().toStdString(),99999999);                    
			}else{
				ui->ResourceTable->setItem(aux,1,new QTableWidgetItem(resAvailable->text()));
				pro->modifyResource(resName->text().toStdString(),resAvailable->text().toInt());                   
			}
		}else{
			showErrorInfo(E_VALUE_RESOURCE, "No se puede reducir el numero de unidades si el recurso "+ resName->text().toStdString()+" esta asignado");
		}
        clearAll();
        newResource->close();
	}
}

void resourcesView:: cancelNewResource()
{
    clearAll();
    newResource->close();
}
void resourcesView::setValues(){
	clearAll();
    resName->setText(ui->ResourceTable->item(row2edit,0)->text());
    resName->setEnabled(false);

    int nResMax = pro->getResource(resName->text().toStdString())->getUnitsMax();

    if(nResMax>=99999999){
        resUnlimited->setChecked(true);
        resAvailable->setValue(0);
    }
    else    resAvailable->setValue(nResMax);
	newResource->setWindowTitle(tr("Edita Recurso"));
	edit = true;
}
void resourcesView:: clearAll()
{
    resName->clear();
    resName->setEnabled(true);
    resAvailable->setValue(0);
    resUnlimited->setChecked(false);
	newResource->setWindowTitle(tr("Nuevo Recurso"));
	edit = false;
}

void resourcesView:: updateTable()
{
    vector<Resource*> *vecRes = pro->getResources();
    int resSize=pro->sizeResources();
    for(int i=0;i<resSize;i++){
      ui->ResourceTable->setRowCount(ui->ResourceTable->rowCount()+1);
      QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(vecRes->at(i)->getName()));
      item->setFlags(Qt::ItemIsSelectable);
      ui->ResourceTable->setItem(i,0,item);
      ui->ResourceTable->setItem(i,1,new QTableWidgetItem(QString::number(vecRes->at(i)->getUnitsMax())));
    }
}

void resourcesView:: deleteResource()
{
	int row2delete = ui->ResourceTable->currentRow();

    if(row2delete>=0){
        QMessageBox mes;
        mes.setText(QString("Eliminar Actividad"));
        mes.setInformativeText(QString(tr("Se borrarán todas las relaciones de esta actividad ¿Esta seguro?\n")));
        QAbstractButton *cancelDelete = mes.addButton(("Cancelar"),QMessageBox::NoRole);
        QAbstractButton *acceptDelete = mes.addButton(("Aceptar"),QMessageBox::YesRole);
        mes.setIcon(QMessageBox::Question);
        mes.exec();
        if(mes.clickedButton()==acceptDelete){
            pro->deleteResource(ui->ResourceTable->item(row2delete,0)->text().toStdString());
            ui->ResourceTable->removeRow(row2delete);
			ui->ResourceTable->setCurrentCell(-1,-1);
			ui->DeleteResource_Button->setEnabled(false);
            ui->EditResource_Button->setEnabled(false);
			emit resDeleted();			
        }
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
    }
}

void resourcesView:: editResource()
{
    row2edit = ui->ResourceTable->currentRow();
    if(row2edit>=0){
		setValues();		
        newResource->show();
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
    }
}
