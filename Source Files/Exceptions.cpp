#include <iostream>
#include "Exceptions.h"

using namespace std;



void showAvisoInfo(int code, std::string information){
    QMessageBox mes;
	switch(code){
	
		case A_RESOURCE_ALREADY_ADDED:
            mes.warning(0,QString("Aviso"),QString("La actividad ").append(QString::fromStdString(information).append(QString(" por lo que se modifican las unidades asginadas"))));
            //std::cout<<"La actividad "+ information+ " por lo que se modifica las unidades asignadas"<<std::endl;
			break;
		case A_UNITS_RESOURCE_0:
            mes.warning(0,QString("Aviso"),QString("La actividad ").append(QString::fromStdString(information).append(QString(" por lo que se elimina la asignacion del recurso"))));
            //std::cout<<"La actividad "+ information+ " por lo que se le desasigna automaticamente ese recurso"<<std::endl;
			break;

		default:
			std::cout<<information<<endl;
			break;
	}
    mes.show();

}


void showErrorInfo(int code, std::string information){
    QMessageBox mes;
	switch(code){
	
		case E_ACTIVITY_NOT_EXIST:
            mes.information(0,QString("Aviso"),QString("La actividad ").append(QString::fromStdString(information).append(QString(" no existe"))));
            //std::cout<<"La actividad "+ information+ " no existe"<<std::endl;
			break;

		case E_RELATION_NOT_EXIST:
            mes.information(0,QString("Error"),QString::fromStdString(information));
            //std::cout<<"Error: "+ information<<std::endl;
			break;

		case E_CYCLE:
            mes.information(0,QString("Aviso"),QString("Se produce un ciclo, ya existe la relacion\n").append(QString::fromStdString(information)));
            //std::cout<<"Se produce un ciclo porque ya existe relacion "+information<<std::endl;
			break;

		case E_RELATION_ALREADY_EXIST:
            mes.information(0,QString("Aviso"),QString("Ya existe la relacion\n").append(QString::fromStdString(information)));
            //std::cout<<"Ya existe relacion "+information<<std::endl;
			break;

		case E_ACTIVITY_ALREADY_EXIST:
            mes.information(0,QString("Aviso"),QString("La actividad ").append(QString::fromStdString(information).append(QString(" ya existe"))));
            //std::cout<<"La actividad "+information+" ya existe"<<std::endl;
			break;

		case E_VALUE_RESOURCE:
            mes.information(0,QString("Aviso"),QString::fromStdString(information));
            //std::cout<<information<<std::endl;
			break;
		case  E_RESOURCE_NOT_ALLOCATED:
            mes.information(0,QString("Aviso"),QString("El recurso ").append(QString::fromStdString(information)));
            //std::cout<<"El recurso "+information<<endl;
			break;

		case E_RESOURCE_NOT_EXIST:
            mes.information(0,QString("Aviso"),QString("El recurso ").append(QString::fromStdString(information).append(QString(" no existe"))));
            //std::cout<<"El recurso "+information+" no existe"<<endl;
			break;

        case E_RESOURCE_ALREADY_EXIST:
            mes.information(0,QString("Aviso"),QString("Ya existe el recurso: \n").append(QString::fromStdString(information)));
            //std::cout<<"Ya existe el recurso "+information<<std::endl;
			break;

		case E_OPEN_FILE:
            mes.information(0,QString("Aviso"),QString("No se pude abrir el fichero \n").append(QString::fromStdString(information)));
            //std::cout<<"No se puede abrir el fichero "+information<<std::endl;
			break;

		case E_RESOURCE_MAX_EXCEDED:
            mes.information(0,QString("Aviso"),QString("Se excede el max de unidades del recurso: \n").append(QString::fromStdString(information)));
            //std::cout<<"Se ha excedido el maximo de unidades del recurso "+information<<std::endl;
			break;

		case E_ALGORITHM_NOT_EXIST_RESOURCES:
            mes.information(0,QString("Aviso"),QString("El algoritmo: ").append(QString::fromStdString(information).append(QString("\nnecesita que el proyecto tenga recursos"))));
            //std::cout<<"El algoritmo "+ information+ " necesita un que el proyecto tengo algun recurso"<<std::endl;
			break;

		case E_DEPENDENCES:
            mes.information(0,QString("Aviso"),QString("No se cumple la dependencia entre: \n").append(QString::fromStdString(information)));
            //std::cout<<"No se cumple la dependencia entre "+ information<<std::endl;
			break;

		case E_FORMAT_FILE:
            mes.information(0,QString("Aviso"),QString("El fichero: \n").append(QString::fromStdString(information).append(QString("\nNo termina con .gpi"))));
            //std::cout<<"El fichero "+ information+" no termina con .gpi"<<std::endl;
			break;
		case E_UPDATE_PATH:
			 mes.information(0,QString("Aviso"),QString("El path del fichero \n").append(QString::fromStdString(information).append(QString(" no se puede modificar mientras esta abierto\n"))));
           break;

		default:
			cout<<information<<endl;
			break;
	
	
	
	}





}
