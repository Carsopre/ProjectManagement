#include "calendar.h"
#include "ui_calendar.h"
#include "stdio.h"

calendar::calendar(QVector<QDate> *fest, QVector<QDate> *labo,QString *weekEnd, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calendar)
{
    ui->setupUi(this);
    tam_festivos=0;    tam_laborables=0;
    addWin = true;
    this->festivos=fest;
    this->laborables=labo;
    this->weekEndC=weekEnd;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));

    ui->FesTable->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->LabTable->setEditTriggers(QTableWidget::NoEditTriggers);

    LoadCal();

    //ICONS
    ui->addFes_Button->setIcon(QPixmap("./icons/add.png"));     ui->addFes_Button->setIconSize(QSize(40,40));
    ui->addLab_Button->setIcon(QPixmap("./icons/add.png"));     ui->addLab_Button->setIconSize(QSize(40,40));
    ui->delLab_Button->setIcon(QPixmap("./icons/minus.png"));   ui->delLab_Button->setIconSize(QSize(40,40));
    ui->delFes_Button->setIcon(QPixmap("./icons/minus.png"));   ui->delFes_Button->setIconSize(QSize(40,40));
    ui->updLab_Button->setIcon(QPixmap("./icons/update.png"));   ui->updLab_Button->setIconSize(QSize(40,40));
    ui->updFes_Button->setIcon(QPixmap("./icons/update.png"));   ui->updFes_Button->setIconSize(QSize(40,40));


    //Tooltips
    ui->addFes_Button->setToolTip(QString("Añadir Festivo"));    ui->addLab_Button->setToolTip(QString("Añadir Laborable"));
    ui->updFes_Button->setToolTip(QString("Mostrar en calendario"));    ui->updLab_Button->setToolTip(QString("Mostrar en calendario"));
    ui->delFes_Button->setToolTip(QString("Eliminar Festivo"));  ui->delFes_Button->setToolTip(QString("Eliminar Laborable"));
    //Connecting
    connect(ui->addFes_Button,SIGNAL(clicked()),this,SLOT(AddFestivo()));
    connect(ui->delFes_Button,SIGNAL(clicked()),this,SLOT(DeleteFestivo()));
    connect(ui->addLab_Button,SIGNAL(clicked()),this,SLOT(AddExtra()));
    connect(ui->delLab_Button,SIGNAL(clicked()),this,SLOT(DeleteExtra()));
    connect(ui->updFes_Button,SIGNAL(clicked()),this,SLOT(UpdateCal()));
    connect(ui->updLab_Button,SIGNAL(clicked()),this,SLOT(UpdateCal()));
    connect(ui->Sab_Labo,SIGNAL(clicked()),this,SLOT(aboutWeekends()));
    connect(ui->Dom_Labo,SIGNAL(clicked()),this,SLOT(aboutWeekends()));

    //Tables
    connect(ui->FesTable,SIGNAL(cellClicked(int,int)),this,SLOT(ChangeFesRow(int,int)));
    connect(ui->LabTable,SIGNAL(cellClicked(int,int)),this,SLOT(ChangeLabRow(int,int)));
}

calendar::~calendar()
{
    delete ui;
}

void calendar:: aboutWeekends()
{
    weekEndC->clear();

    if((!ui->Dom_Labo->isChecked())&&(!ui->Sab_Labo->isChecked()))  weekEndC->append(QString("NONE"));

    if(ui->Dom_Labo->isChecked())   weekEndC->append(QString("SUN"));
    if(ui->Sab_Labo->isChecked()){
        weekEndC->clear();
        if(ui->Dom_Labo->isChecked())   weekEndC->append(QString("BOTH"));
        else    weekEndC->append(QString("SAT"));
    }
}

void calendar:: ChangeFesRow(int row, int col)
{
    ui->FesTable->setCurrentCell(row,col);
    ui->FesTable->selectRow(row);
}

void calendar:: ChangeLabRow(int row, int col)
{
    ui->LabTable->setCurrentCell(row,col);
    ui->LabTable->selectRow(row);
}

void calendar:: LoadCal()
{
  ui->MyCal->setGridVisible(true);
  ui->MyCal->setFirstDayOfWeek(Qt::Monday);
  getLabo();
  getFest();

}

void calendar:: UpdateCal()
{
    ui->MyCal->update();
    //UpdateFestivos
    for(int i=0;i<festivos->size();i++)     PaintCal(festivos->value(i),false);
    //UpdateLaborables
    for(int j=0;j<laborables->size();j++)   PaintCal(laborables->value(j),true);


}

void calendar:: PaintCal(QDate day,bool LabFes) //Rep un día i un bool que determina si el día es Laborable o festiu
{
    if(LabFes){ //Es laborable
        brus.setColor(Qt::black);
        format.setForeground(brus);
        format.setBackground(Qt::white);
        ui->MyCal->setDateTextFormat(day,format);
    }else{ //Es festiu
        brus.setColor(Qt::white);
        format.setBackground(Qt::red);
        format.setForeground(brus);
        ui->MyCal->setDateTextFormat(day,format);
    }

}

void calendar:: getLabo()
{

}

void calendar:: getFest()
{    
        /* National holidays
        Gener: 1, 6
        Març: 19
        Maig: 1;
        Agost: 15;
        Octubre: 12
        Novembre: 1
        Decembre: 6, 8, 25*/
    ui->FesTable->setRowCount(10);
    ui->FesTable->setItem(0,0,new QTableWidgetItem(QString("01/01"))); ui->FesTable->setItem(0,1,new QTableWidgetItem(QString("Año Nuevo")));
    ui->FesTable->setItem(1,0,new QTableWidgetItem(QString("06/01"))); ui->FesTable->setItem(1,1,new QTableWidgetItem(QString("Día de Reyes")));
    ui->FesTable->setItem(2,0,new QTableWidgetItem(QString("19/03"))); ui->FesTable->setItem(2,1,new QTableWidgetItem(QString("San José")));
    ui->FesTable->setItem(3,0,new QTableWidgetItem(QString("01/05"))); ui->FesTable->setItem(3,1,new QTableWidgetItem(QString("Fiesta del Trabajo")));
    ui->FesTable->setItem(4,0,new QTableWidgetItem(QString("15/08"))); ui->FesTable->setItem(4,1,new QTableWidgetItem(QString("La Asunción")));
    ui->FesTable->setItem(5,0,new QTableWidgetItem(QString("12/10"))); ui->FesTable->setItem(5,1,new QTableWidgetItem(QString("Fiesta Nacional de España")));
    ui->FesTable->setItem(6,0,new QTableWidgetItem(QString("01/11"))); ui->FesTable->setItem(6,1,new QTableWidgetItem(QString("Todos los Santos")));
    ui->FesTable->setItem(7,0,new QTableWidgetItem(QString("06/12"))); ui->FesTable->setItem(7,1,new QTableWidgetItem(QString("Día de la Constitución")));
    ui->FesTable->setItem(8,0,new QTableWidgetItem(QString("08/12"))); ui->FesTable->setItem(8,1,new QTableWidgetItem(QString("Día de la Concepción")));
    ui->FesTable->setItem(9,0,new QTableWidgetItem(QString("25/12"))); ui->FesTable->setItem(9,1,new QTableWidgetItem(QString("Navidad")));
}

void calendar:: AddFestivo()
{
    if(addWin){
        insertWindow = new InsertDate(ui->FesTable,festivos);
        insertWindow->show();
        addWin = false;
    }else{
        if(!insertWindow->isVisible()){
            insertWindow = new InsertDate(ui->FesTable,festivos);
            insertWindow->show();
        }
    }

}

void calendar:: DeleteFestivo()
{
    int row2delete = ui->FesTable->currentRow();
    int festivosNacionales =10;
    if(row2delete>=0){
        if(row2delete<festivosNacionales){
            QMessageBox::information(this,QString("Aviso"),QString("No se pueden eliminar los Festivos nacionales"
                                                                   "\ndebe de introducir un día Laboral EXTRA en su"
                                                                   "\nlugar."));
        }else{
            ui->FesTable->removeRow(row2delete);
            if(festivos->value(row2delete-festivosNacionales).dayOfWeek()==Qt::Saturday || festivos->value(row2delete-festivosNacionales).dayOfWeek()==Qt::Sunday)
            {
                ui->MyCal->setDateTextFormat(festivos->value(row2delete-festivosNacionales),ui->MyCal->dateTextFormat(QDate(2012,10,26)));
            }else           ui->MyCal->setDateTextFormat(festivos->value(row2delete-festivosNacionales),  ui->MyCal->dateTextFormat(QDate(2012,10,27)));
            festivos->remove(row2delete-festivosNacionales);
            ui->FesTable->setCurrentCell(-1,-1);
        }
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
    }

    //if((ui->FesTable->rowCount())==0)    ui->delFes_Button->setEnabled(false);
}

void calendar:: AddExtra()
{
    if(addWin){
        insertWindow = new InsertDate(ui->LabTable,laborables);
        insertWindow->show();
        addWin = false;
    }else{
        if(!insertWindow->isVisible()){
            insertWindow = new InsertDate(ui->LabTable,laborables);
            insertWindow->show();
        }
    }
}

void calendar:: DeleteExtra()
{
    int row2delete = ui->LabTable->currentRow();
    if(row2delete>=0){
        ui->LabTable->removeRow(row2delete);
        if(festivos->value(row2delete).dayOfWeek()==Qt::Saturday || festivos->value(row2delete).dayOfWeek()==Qt::Sunday)
        {
            ui->MyCal->setDateTextFormat(festivos->value(row2delete),ui->MyCal->dateTextFormat(QDate(2012,10,26)));
        }else           ui->MyCal->setDateTextFormat(festivos->value(row2delete),  ui->MyCal->dateTextFormat(QDate(2012,10,27)));
        laborables->remove(row2delete);
        ui->LabTable->setCurrentCell(-1,-1);
    }else{
        QMessageBox mes;
        mes.information(0,QString(tr("Aviso")),QString(tr("Debe seleccionar una fila\n de la tabla")));
    }

}
