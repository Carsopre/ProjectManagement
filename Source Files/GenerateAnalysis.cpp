#include "GenerateAnalysis.h"

GenerateAnalysis::GenerateAnalysis(Project *proyec,QVector<QDate> *workDates, QWidget *parent)
    :QWidget(parent)
{
    //Linking
    this->pro=proyec;
    this->setWindowTitle(QString("Análisis"));
    this->setWindowIcon(QIcon("./icons/management.png"));
    this->setWindowIconText(QString(tr("Project Manager")));

    this->datesVec=workDates;

    buildFlex();
    buildProb();

    hGroups = new QHBoxLayout();
    hGroups->addWidget(groupFlex);
    hGroups->addWidget(groupProb);

    joinGroups();
}
void GenerateAnalysis:: joinGroups(){


    //BUTTONS
    submitAnalysis = new QPushButton(QString("Analizar"));
        submitAnalysis->setFixedSize(120,60);
    cancelAnalysis = new QPushButton(QString("Cerrar"));
        cancelAnalysis->setFixedSize(120,60);
        connect(submitAnalysis,SIGNAL(clicked()),this,SLOT(submit()));
        connect(cancelAnalysis,SIGNAL(clicked()),this,SLOT(close()));
    QHBoxLayout *hbut = new QHBoxLayout;
        hbut->addWidget(cancelAnalysis);
        hbut->addWidget(submitAnalysis);
    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hGroups);
        vbox->addLayout(hbut);
    this->setLayout(vbox);
}

GenerateAnalysis::~GenerateAnalysis()
{
    delete this;
}
//Build WINDOWS
void GenerateAnalysis:: refreshFlex(int index)
{
    act=pro->getActivities().at(index);
    hlrLine->setText(QString::number(act->getHLR()));
    hlaLine->setText(QString::number(act->getHLA()));
}

void GenerateAnalysis:: buildFlex()
{
    groupFlex = new QGroupBox(QString("Flexibilidad Actividad"));
    groupFlex->setCheckable(true); groupFlex->setChecked(false);


    actCombo = new QComboBox();
    for(int i=0;i<pro->sizeActivities();i++)
        actCombo->addItem(QString::fromStdString(pro->getActivities().at(i)->getName()));
    connect(actCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshFlex(int)));
    QHBoxLayout *hCombo = new QHBoxLayout;
        QLabel *actLabel = new QLabel(QString("Actividad: "));
        hCombo->addWidget(actLabel);
        hCombo->addWidget(actCombo);
    act=pro->getActivities().at(0);
    QLabel *hlrLab = new QLabel(QString("HLR: "));
    hlrLine = new QLineEdit;
        hlrLine->setText(QString::number(act->getHLR()));
        hlrLine->setReadOnly(true);        
    QHBoxLayout *hlrBox = new QHBoxLayout;
        hlrBox->addWidget(hlrLab); hlrBox->addWidget(hlrLine);
    QLabel *hlaLab = new QLabel(QString("HLA: "));
    hlaLine = new QLineEdit;
        hlaLine->setText(QString::number(act->getHLA()));
        hlaLine->setReadOnly(true);
    QHBoxLayout *hlaBox = new QHBoxLayout;
        hlaBox->addWidget(hlaLab); hlaBox->addWidget(hlaLine);


    backRadio = new QRadioButton;
        backRadio->setText(QString("Atrasar")); backRadio->setFixedWidth(120);
    forRadio = new QRadioButton;
        forRadio->setText(QString("Adelantar"));forRadio->setFixedWidth(120);
    connect(backRadio,SIGNAL(clicked()),this,SLOT(changeOptions()));
    connect(forRadio,SIGNAL(clicked()),this,SLOT(changeOptions()));
        forRadio->setChecked(true);

    backSpinner = new QSpinBox;
        backSpinner->setFixedWidth(80); backSpinner->setAlignment(Qt::AlignLeft);
        backSpinner->setMinimum(0);     backSpinner->setSingleStep(1);
        backSpinner->setMaximum(999);   backSpinner->setEnabled(false);
    forSpinner = new QSpinBox;
        forSpinner->setFixedWidth(80);  forSpinner->setAlignment(Qt::AlignLeft);
        forSpinner->setMinimum(0);      forSpinner->setSingleStep(1);
        forSpinner->setMaximum(999);    forSpinner->setEnabled(true);

    //TOP
    QVBoxLayout *top = new QVBoxLayout;
        top->addLayout(hCombo);
        top->addLayout(hlrBox);
        top->addLayout(hlaBox);
    //BOTTOM
    QHBoxLayout *hbox1 = new QHBoxLayout;
        hbox1->addWidget(forRadio);
        hbox1->addWidget(forSpinner);
    QHBoxLayout *hbox2 = new QHBoxLayout;
        hbox2->addWidget(backRadio);
        hbox2->addWidget(backSpinner);
    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(top);
        vbox->addLayout(hbox1);
        vbox->addLayout(hbox2);
    groupFlex->setLayout(vbox);
    groupFlex->resize(200,200);
}
void GenerateAnalysis:: buildVar(){
    groupVar = new  QGroupBox(QString("Variación"));
    groupVar->setCheckable(true); groupVar->setChecked(false);
    realDur = new QSpinBox();
        realDur->setMinimum(0); realDur->setSingleStep(1);
        realDur->setMaximum(99999999); realDur->setFixedWidth(80);
    realCost = new QDoubleSpinBox();
        realCost->setMinimum(0.00); realCost->setSingleStep(0.05);
        realCost->setMaximum(99999999); realCost->setFixedWidth(80);

    QHBoxLayout *hdur = new QHBoxLayout;
    QLabel *durLab = new QLabel("Duración Real");
        hdur->addWidget(durLab);
        hdur->addWidget(realDur);
    QHBoxLayout *hcost = new QHBoxLayout;
        QLabel *costLab = new QLabel("Coste Real");
        hcost->addWidget(costLab);
        hcost->addWidget(realCost);

    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hdur);
        vbox->addLayout(hcost);

    groupVar->setLayout(vbox);
}
void GenerateAnalysis:: buildProb(){

    groupProb = new QGroupBox(QString("Probabilidad"));
    groupProb->setCheckable(true);  groupProb->setChecked(false);
    radioProb1 = new QRadioButton();    radioProb1->setText(QString("Probabilidad -> Días"));
    radioProb2 = new QRadioButton();    radioProb2->setText(QString("Días -> Probabilidad"));
    radioProb3 = new QRadioButton();    radioProb3->setText(QString("Intervalo"));
        radioProb1->setChecked(true);

        varian = new QDoubleSpinBox();
        QLabel *varLab = new QLabel(QString("Varianza: "));
            varian->setMaximum(999999); varian->setMinimum(0.00);
            varian->setDecimals(2); varian->setSingleStep(0.5);
            QHBoxLayout *hVar = new QHBoxLayout;
                hVar->addWidget(varLab);
                hVar->addWidget(varian);
        probProb = new QDoubleSpinBox(); probProb->setMaximum(99.5);
            probProb->setMinimum(00.00); probProb->setDecimals(2);
            probProb->setSingleStep(0.5);
            QVBoxLayout *vbox1 = new QVBoxLayout;
                vbox1->addWidget(radioProb1);
                vbox1->addWidget(probProb);
       durProb = new QSpinBox();    durProb->setMaximum(9999);
            durProb->setMinimum(0); durProb->setSingleStep(1);
            QVBoxLayout *vbox2 = new QVBoxLayout;
                vbox2->addWidget(radioProb2);
                vbox2->addWidget(durProb);

        int1Prob = new QSpinBox(); int1Prob->setMaximum(9999); int1Prob->setMinimum(0); int1Prob->setSingleStep(1);
        int2Prob = new QSpinBox(); int2Prob->setMaximum(9999); int2Prob->setMinimum(0); int2Prob->setSingleStep(1);
        //int1Prob->setPrefix(QString("[ "));int2Prob->setPrefix(QString(" , ")); int2Prob->setSuffix(QString(" ]"));
        QHBoxLayout *hbox3 = new QHBoxLayout;
            hbox3->addWidget(int1Prob);
            hbox3->addWidget(int2Prob);
        QVBoxLayout *vbox3 = new QVBoxLayout;
            vbox3->addWidget(radioProb3);
            vbox3->addLayout(hbox3);

    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hVar);
        vbox->addLayout(vbox1);
        vbox->addLayout(vbox2);
        vbox->addLayout(vbox3);
    groupProb->setLayout(vbox);
}

//PRIVATE SLOTS
void GenerateAnalysis::changeOptions(){
    if(forRadio->isChecked()){
        forSpinner->setEnabled(true);
        backSpinner->setEnabled(false);
    }else{
        backSpinner->setEnabled(true);
        forSpinner->setEnabled(false);
    }
}
void GenerateAnalysis::submit(){

    QMessageBox *mes = new QMessageBox();
    mes->setWindowTitle(QString("Análisis"));
    QString analisisMes;
    if(!groupFlex->isChecked() && !groupProb->isChecked()){
        analisisMes.append(QString("No ha sido marcaado ningún tipo de análisis"));
    }

    if(groupFlex->isChecked()){        
        //act = pro->getActivity(actCombo->currentText().toStdString());
        QString flexMes("Flexibilidad Actividad "); flexMes.append(QString::fromStdString(act->getName()));
        flexMes.append(QString(": "));
        if(forRadio->isChecked()){
            flexMes.append(QString::fromStdString(pro->analizeAnticipateTMin(forSpinner->value(),act->getName())));
        }else{
            flexMes.append(QString::fromStdString(pro->analizeDelayTMin(backSpinner->value(),act->getName())));
        }
        analisisMes+=flexMes;
    }
    int num=0;
    if(groupProb->isChecked()){
        
		if(radioProb1->isChecked()){
			QString probMes("\Duracion Proyecto: ");
            num=pro->getDurationByProbability(probProb->value(),varian->value());
            QString proDate("");
            proDate.append(QString::number(num));
			if(pro->getEnd()->getTMax()>=num){
				proDate.append(" Fecha Fin: "); 
				proDate.append(datesVec->at(num-1).toString("dd/MM/yyyy"));
			}
			probMes.append(proDate);
			analisisMes+=probMes;
		}
        if(radioProb2->isChecked())
		{	QString probMes("\nProbabilidad Proyecto: ");
            probMes.append(QString::number(pro->getProbabDurationLessThan(durProb->value(),varian->value())));
			analisisMes+=probMes;
		}
        if(radioProb3->isChecked())
		{	QString probMes("\nProbabilidad Proyecto: ");
            probMes.append(QString::number(pro->getProbabDurationBetween(int1Prob->value(),int2Prob->value(),varian->value()))); 
			analisisMes+=probMes;
		}
        
    }
    mes->information(0,QString("Análisis"),analisisMes);
}
