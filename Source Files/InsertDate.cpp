#include "InsertDate.h"
#include <stdio.h>

InsertDate::InsertDate(QTableWidget *table, QVector<QDate> *vec_dates, QWidget *parent)
    :QWidget(parent)
{
    this->dates=vec_dates;
    this->tabla=table;
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));

    BuildWindow();
    this->setWindowTitle(tr("Nueva Fecha"));
    connect(Guardar,SIGNAL(clicked()),this,SLOT(SaveDate()));
    connect(Cancelar,SIGNAL(clicked()),this,SLOT(close()));

}
InsertDate::~InsertDate(){
    delete this;
}
bool InsertDate:: CheckRepeated(QDate *date)
{
    for(int i=0;i<dates->size();i++)
        if(dates->value(i)==*date)   return true;
    return false;
}

void InsertDate:: SaveDate(){
    int aux = tabla->rowCount();    
    QMessageBox mes;
    bool valido = QDate::isValid(yyyy->value(),mm->value(),dd->value());
    if(!valido){
        mes.warning(0,QString("Error"),QString("La fecha no es valida"));
    }else{
        QDate *dateis = new QDate(yyyy->value(),mm->value(),dd->value());
        if(!CheckRepeated(dateis)){
            tabla->setRowCount(aux+1);
            dates->append(*dateis);
            tabla->setItem(aux,0,new QTableWidgetItem(dates->last().toString("dd/MM/yyyy")));
            this->close();
        }else   mes.warning(0,QString("Error"),QString("La fecha ya se ha introducido aqui"));
    }
}

void InsertDate:: BuildWindow(){
    Guardar = new QPushButton(QString(tr("Guardar")));
    Cancelar = new QPushButton(QString(tr("Cancelar")));
    HBox = new QHBoxLayout;
        dd = new QSpinBox;
          dd->setMaximum(31);dd->setMinimum(1);
          dd->setMaximumWidth(50);
        mm = new QSpinBox;
           mm->setMaximum(12);mm->setMinimum(1);
           mm->setMaximumWidth(50);
        yyyy = new QSpinBox;
           yyyy->setMinimum(2012); yyyy->setMaximumWidth(110);
           yyyy->setMaximum(2100);
    QLabel *text = new QLabel(QString("Fecha: "));
    HBox->addWidget(text);
    HBox->addWidget(dd);
    HBox->addWidget(mm);
    HBox->addWidget(yyyy);
    QHBoxLayout *butsaux = new QHBoxLayout;
        butsaux->addWidget(Cancelar);
        butsaux->addWidget(Guardar);
    QVBoxLayout *wind = new QVBoxLayout;
        wind->addItem(HBox);
        wind->addItem(butsaux);
    this->setLayout(wind);
}
