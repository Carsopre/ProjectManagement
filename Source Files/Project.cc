#include <sstream>
#include <set>
#include <math.h>
#include "Project.h"
#include <algorithm>
#include "Rules.h"
#include <math.h>
#include <queue>
#include <deque>
#include "Probability.h"
#include "Resource.h"
#include "Activity.h"
#include "Path.h"
#include "Exceptions.h"

//funcion auxiliar para tranformar los floats a string
string num_to_str(float n) {
	stringstream s;
	s << n;
return s.str();
}


/****************************************************************/
/*						CONSTRUCTOR								*/
/****************************************************************/
Project::Project(void)
{
	this->critical_paths= new std::vector<Path*>;
	this->resources= new std::vector<Resource*>;
	this->begin=new Activity("Inicio",0,0,0,0);
	this->end=new Activity("Fin",0,0,0,0);
	this->enabled=false;						//modo debug deshabilitado
	this->report="report";
	
}


/****************************************************************/
/*						DESCONSTRUCTOR							*/
/****************************************************************/
Project::~Project(void)
{
	for(int i=0; i<critical_paths->size();i++)
		delete(critical_paths->at(i));
	delete(critical_paths);
	for(int i=0; i<resources->size();i++)
		delete(resources->at(i));
	delete(this->resources);
	for(int i=0; i<activities.size();i++)
		delete(activities.at(i));

	delete(end); 
	delete(begin);
}


/****************************************************************/
/*							METODOS								*/
/****************************************************************/
void Project::clear()
{
	/*resetear los calculos del proyecto*/
	for(int i=0; i<activities.size();i++)
		activities.at(i)->clear();

	this->critical_paths->clear();
	this->end->clear();

}



/****************************************************************/
/*						ACTIVIDADES								*/
/****************************************************************/

void Project:: modifyActivity(std::string name,int t_normal, int t_tope, float cost_normal, float cost_unit_oportunity)
{
	/*modificar actividad*/
	Activity* act=getActivity(name);

	if(!act)
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name);
	else
		act-> modify(t_normal, t_tope, cost_normal, cost_unit_oportunity);

}



Activity * Project::getActivity(std::string  name)
{
	Activity * act;

	/*Obtener actividad*/
	if( name==begin->getName())
		return begin;

	if( name==end->getName())
		return end;

	 for(int i=0; i<this->activities.size();i++){
		act=(Activity *)this->activities.at(i);
		if(act->getName().compare( name)==0)
			return activities[i];
	 }

	return NULL;

}

int Project::getPositionActivity(std::string  name)
{
	Activity * act;

	/*Posicion del vector actividades donde se encuentra la actividad*/
	 for(int i=0; i<this->activities.size();i++){
		act=(Activity *)this->activities.at(i);
		if(act->getName().compare( name)==0)
			return i;
	 }
	
	return -1; // no existe

}


void Project::deleteActivity(std::string  name){

	int pos;

	Activity* activ=getActivity( name);

	if(!activ)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name);

	else{
		pos=this->getPositionActivity( name);

		/*Borrar relaciones de precedencia*/
		while(activ->getList_Activities_pred()->size()>0){
			this->deleteRelation(activ->getList_Activities_pred()->at(0)->activity->getName(), activ->getName());

		}

		/*Borrar relaciones de succesion*/
		while(activ->getList_Activities_suc()->size()>0){
			this->deleteRelation(activ->getName(), activ->getList_Activities_suc()->at(0)->activity->getName());
			}

		/*Borrar acitvidad del proyecto*/
		this->activities.erase(this->activities.begin()+pos);
		delete(activ);
	}

}

void Project::addActivity(std::string name,int tn, int tp, float cnormal, float cuoport){

	/*Crear y añadir actividad al proyecto*/
	Activity *act;

	if(getActivity(name))
		showErrorInfo(E_ACTIVITY_ALREADY_EXIST, name);

	else{
		act=new Activity(name,tn, tp,  cnormal, cuoport);
		activities.push_back(act);

		/*No tiene ninguna relacion aun, asi que se relaciona con inicio y fin */
		this->addRelation(this->begin->getName(), name);
		this->addRelation(name, this->end->getName());
	}
}

void Project::addActivity(std::string name,int tn, float cnormal)
{

	Activity *act;

	/*Crear y añadir actividad al proyecto*/
	
	if(getActivity(name))
		showErrorInfo(E_ACTIVITY_ALREADY_EXIST, name);

	else{
		act=new Activity(name,tn,  cnormal);
		activities.push_back(act);

		/*No tiene ninguna relacion aun, asi que se relaciona con inicio y fin */
		this->addRelation(this->begin->getName(), name);
		this->addRelation(name, this->end->getName());
	}
}


void Project::updateTMax()
{
	/*limpiar valores anterior de t.max*/
	for(int i=0; i<activities.size();i++)
		activities.at(i)->setTMax(-1);
	end->setTMax(-1);

	/*Calcular tiempos maximos*/
	begin->setTMax(begin->calculMaxTime());

	/*Calcular Hogura Libre*/
	begin->calculHolguraFree(end->getTMax());

}

/****************************************************************/
/*						RELACIONES								*/
/****************************************************************/
void Project::addRelation(std::string  name_orig, std::string  name_dest){

	bool deleteIni=false;
	bool deleteFin=false;
	Activity* orig=getActivity( name_orig);
	Activity* dest=getActivity( name_dest);

	/*Exiten las actividades?*/
	if(!orig)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name_orig);

	else if(!dest)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name_dest);

	/*Se produce un ciclo?*/
	else if(orig->getPositionRelationPredecesor( name_dest)!=-1||dest->getPositionRelationSuccesor( name_orig)!=-1) 
		showErrorInfo(E_CYCLE, "desde "+dest->getName()+" a "+orig->getName());

	/*Ya existia esta relacion?*/
	else if(orig->getPositionRelationSuccesor( name_dest)!=-1||dest->getPositionRelationPredecesor( name_orig)!=-1)
		showErrorInfo(E_RELATION_ALREADY_EXIST, "desde "+orig->getName()+" a "+dest->getName());

	/*Relacion sobre una misma acitivdad?*/
	else if( name_orig.compare( name_dest)==0)
		showErrorInfo(E_CYCLE, "desde "+dest->getName()+" a "+orig->getName());

	else{

		/*Comprobar si antes de añadir la relacion era uno de las actividades de inicio,
		relacionandas con la actividad ficticia Inicio*/
		if(dest->predecesorIsInitial())
			deleteIni=true;
	
		/*Comprobar si antes de añadir la relacion era una de las actividades de fin,
		relacionandas con la actividad ficticia Fin*/
		if(orig->succesorIsEnd())
			deleteFin=true;

		/*Añadir las relaciones*/
		dest->addPredecesor(orig);
		orig->addSuccesor(dest);

		/*Si se relacionaban con Inicio y Fin, eliminar esa relacion*/
		if(deleteFin)
			this->deleteRelation( name_orig,end->getName());
		if(deleteIni)
			this->deleteRelation(begin->getName(), name_dest);

	
	}
}



void Project::deleteRelation(std::string  name_orig, std::string  name_dest){

	int pos_orig;
	int pos_dest;

	Activity* orig=getActivity( name_orig);
	Activity* dest=getActivity( name_dest);

	if(!orig)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name_orig);

	else if(!dest)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name_dest);

	else{
		pos_orig=dest->getPositionRelationPredecesor( name_orig);
		pos_dest=orig->getPositionRelationSuccesor( name_dest);

		/*Existe relacion entre ambas actividades?*/
		if(pos_orig==-1)
			showErrorInfo(E_RELATION_NOT_EXIST, dest->getName()+" no tiene como  predecesor "+ name_orig);

		else if(pos_dest==-1)
			showErrorInfo(E_RELATION_NOT_EXIST, orig->getName()+" no tiene como succesor "+ name_dest);
		
		else{

			/*Eliminar relacion*/
			dest->getList_Activities_pred()->erase(dest->getList_Activities_pred()->begin()+pos_orig);
			orig->getList_Activities_suc()->erase(orig->getList_Activities_suc()->begin()+pos_dest);

			if(orig->getName().compare(begin->getName())!=0&&dest->getName().compare(end->getName())!=0)
			{
				/*Comprobar si despues de borrar la relacion es una de las actividades de inicio*/
				if(dest->getList_Activities_pred()->size()==0)
					this->addRelation(begin->getName(),dest->getName());	// añadir relacion con Inicio

				/*Comprobar si despues de borrar la relacion es una de las actividades de fin*/
				if(orig->getList_Activities_suc()->size()==0)
					this->addRelation(orig->getName(),end->getName());		// añadir relacion con Fin
			}
		}
	}

}


bool Project::validateDependences()
{	
	bool result=true;

	/*Comprobar relaciones de precedencia entre actividades, solo en que una
	relacion no se cumpla, todo el proyecto no cumplira con esta validacion */
	for(int i=0; i<activities.size();i++)
		result*=activities.at(i)->validateDependences(); // AND logica
	return result;
}




float Project::getTotalCost()
{
	float cost=0;

	/*Calcula el coste de todo el proyecto como suma
	del coste de la actividades mas el sobrecoste si existe*/
	for(int i=0; i<activities.size();i++)
		cost+=activities.at(i)->getCostNormal();

	cost+=overrun;
	return cost;

}



/*****************************************************************/
/*							RECURSOS							 */
/*****************************************************************/
Resource * Project ::getResource(string  name_rec)
{
	/*Obtener el recurso*/
	for(int i=0; i<this->resources->size();i++)
		if( name_rec==resources->at(i)->getName())
			return resources->at(i);

	return NULL;	//no existe
}

bool Project::validateLimitationMaxUnitsAllocated()
{
	int tiempo=end->getTMax();
	int n_resources=0;
	Activity *act;
	Activity::resource * rec_asig;
	
	/*Comprueba que se cumple la limitacion para cada recurso*/
	for(int rec=0; rec<this->resources->size(); rec++)
	{
		
		/*Recorremos cada dia*/
		for(int dia=0; dia<tiempo;dia++)
		{
			n_resources=0;

			/*Selecciona las actividades que se estan realizando ese dia*/
			for(int i=0; i<activities.size();i++)
			{
				act=activities.at(i);
				if(act->getTMin()<=dia&&act->getTMin()+act->getTNormal()-1>=dia)
				{
					rec_asig=act->getResource( resources->at(rec)->getName());
					if(rec_asig!=NULL)
						n_resources+=rec_asig->units_asig;
				}
			}
			/*Ese dia se supera el maximo de recursos permitidos?*/
			if(n_resources>resources->at(rec)->getUnitsMax())
				return false;
		}


	}
	return true;

}

std::vector<double> Project :: getAllocationResourcePerDay( string  name_rec)
{
	int tiempo=end->getTMax();
	double n_resources=0;
	Activity *act;
	Activity::resource * rec_asig;
	Resource * resource=getResource( name_rec);
	std::vector<double> asig_resources;
	
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_rec);

	else{
		
		/*Recorremos cada dia*/
		for(int dia=0; dia<tiempo;dia++)
		{
			n_resources=0;

			/*Selecciona las activities que se estan realizando ese dia*/
			for(int i=0; i<activities.size();i++)
			{
				act=activities.at(i);
				if(act->getTMin()<=dia&&act->getTMin()+act->getTNormal()-1>=dia)
				{
					rec_asig=act->getResource( name_rec);
					if(rec_asig!=NULL)
						n_resources+=rec_asig->units_asig;
				}
			}

			/*Guardamos las unidades asignadas ese dia*/
			asig_resources.push_back(n_resources);
		}


	}
	return asig_resources;

}


int Project ::getPositionResource(string  name)
{
	/*Posicion en el vector de recursos donde esta el recurso dado*/
	for(int i=0; i<this->resources->size();i++)
		if( name==resources->at(i)->getName())
			return i;

	return -1;	//no existe


}

void Project ::addResource(string  name_resource, int units_max )
{
	Resource * new_resource;

	/*Crea y añade un recurso al proyecto*/
	if(this->getResource( name_resource)!=NULL)
		showErrorInfo(E_RESOURCE_ALREADY_EXIST,  name_resource);

	else if(units_max<0)	// no puede ser negativo
		showErrorInfo(E_VALUE_RESOURCE, "value incorrecto de units maximas de recurso "+ name_resource);
	

	else{
		new_resource=new Resource( name_resource,units_max);
		resources->push_back(new_resource);
	
	}


}

void Project::deleteResource(string  name_rec){

	
	int pos =getPositionResource( name_rec);
	Resource * rec= getResource( name_rec);
	
	if(pos==-1||rec==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_rec);

	else{
		
		/*Borrar las asignaciones de ese recurso a las actividades*/
		for(int i =0; i<activities.size();i++)
		{
			if(activities.at(i)->getResource(rec->getName())!=NULL)
				activities.at(i)->deleteResource(rec);

		}
		/*Borrar recurso del proyecto*/
		resources->erase(resources->begin()+pos);
		delete(rec);

	
	}

}
			
void Project::allocateResourceActivity(string  name_resource, string  name_activity, int value)
{

	Resource * resource=getResource( name_resource);
	Activity * Activity= getActivity( name_activity);

	/*La actividad y es recurso existen?*/
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_resource);
	else if(Activity==NULL)
		showErrorInfo(E_ACTIVITY_NOT_EXIST,  name_activity);

	/*Valor valido?*/
	else if(value<0)
		showErrorInfo(E_VALUE_RESOURCE, "Se esta intentando aginar un valor no valido al recurso "+ name_resource);
	else if(value>resource->getUnitsMax())
		showErrorInfo(E_VALUE_RESOURCE, "En la asignacion se esta excediendo el maximo del recurso "+ name_resource);

	else	/*Asignar recurso*/
		Activity->allocateResource(resource, value);
	

}

void Project::deleteResourceActivity(string  name_resource, string  name_activity){

	Resource * resource=getResource( name_resource);
	Activity * activity= getActivity( name_activity);

	/*Borrar asignacion del recurso a la actividad*/
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_resource);

	else if(activity==NULL)
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name_activity);

	else
		activity->deleteResource(resource);


}

void Project::  modifyResource(string  name_resource, int units_max)
{
	Resource * resource=getResource( name_resource);
	
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_resource);

	else if(units_max<0)	/*Valor valido?*/
		showErrorInfo(E_VALUE_RESOURCE, "valor incorrecto de unidades maximas de recurso "+ name_resource);
	
	else if(units_max<resource->getUnitsMax())
	{
		/*si el numero de unidades maximas que se quiere asignar es menor al que tenia, 
		solo se podra modificar si el recurso no esta asignado a ninguna actividad*/
		for(int i=0; i< activities.size();i++)
		{
			if(activities.at(i)->getResource(name_resource)!=NULL)
			{
				showErrorInfo(E_VALUE_RESOURCE, "No se puede reducir el numero de unidades si el recurso "+ name_resource+" esta asignado");
				break;
			}
		}
		resource->setUnitsMax(units_max);

	}else
		resource->setUnitsMax(units_max);
	
}



void Project::incrementUnitsResourceActivity(string  name_resource, string  name_activity, int units)
{
	
	Resource * resource=getResource( name_resource);
	Activity * activity= getActivity( name_activity);

	/*Incrementa las unidades asignadas de un recurso a una actividad*/

	/*Existe el recurso y la actividad?*/
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_resource);

	else if(activity==NULL)
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name_activity);

	else if(units<0) /*Valor valido?*/
		showErrorInfo(E_VALUE_RESOURCE, "Se esta intentando asignar un valor no valido al recurso "+ name_resource);
	
	else
		activity->incrementResource(units,  name_resource);


}
void Project::decrementUnitsResourceActivity(string  name_resource, string  name_activity, int units)
{
	Resource * resource=getResource( name_resource);
	Activity * activity= getActivity( name_activity);

	/*Decrementa las unidades asignadas de un recurso a una actividad*/

	/*Existe el recurso y la actividad?*/
	if(resource==NULL)
		showErrorInfo(E_RESOURCE_NOT_EXIST,  name_resource);

	else if(activity==NULL)
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name_activity);

	else if(units<0)	/*valor valido?*/
		showErrorInfo(E_VALUE_RESOURCE, "Se esta intentando disminuir un valor no valido al recurso "+ name_resource);
	else
		activity->decrementResource(units,resource->getName());

}


/************************************************************************/
/*						ALGORITMO CAMINO CRITICO						*/
/************************************************************************/

void Project ::extractCriticalPaths( Path * path, Activity* partida )
{

	Activity * act;

	/*Ultima actividad, se añade el camino critico calculado*/
	if(partida->succesorIsEnd())
	{
		Path * finished_path=new Path(path);
		critical_paths->push_back(finished_path);
		path->clear();		// lo reseteamos para poder usarlo 
	
	}
	else
	{

		for(int i=0; i< partida->getList_Activities_suc()->size(); i++)
		{
			act=partida->getList_Activities_suc()->at(i)->activity;

			/*La actividade es critica?*/
			if(act->getHTotal()==0)
			{
				/*Guardamos el camino antes de añadirle la actividad*/
				Path * path2=new Path(path);		
				path->getPath()->push_back(act);

				/*Se añade y seguimos examninando su succesora*/
				extractCriticalPaths( path, act);

				/*Cuando terminamos de examinar el anterior camino, habiendo añadido la succesora, 
				restauramos el camino que teniamos hasta entonces (sin la succesora) 
				y seguimos buscando entre las otras susccesoras*/
				path->copy(path2);

				delete(path2);
			}
		}
	}
	path->clear();
}





void Project::calculCriticalPath(){

	Activity *act;

	/*Limpiar*/
	clear();

	/*Calcular tiempos minimos*/
	end->setTMin(end->calculMinTime());

	/*Calcular tiempos maximos*/
	begin->setTMax(begin->calculMaxTime());

	/*depurar*/
	mode_debug("\n\n------------------------ CAMINO CRITICO-------------------\n\n");
	for(int i=0; i<this->activities.size();i++)
	{
		act=activities.at(i);
		mode_debug("Actividad:"+act->getName()+"	Tmin:"+num_to_str(act->getTMin())+"	Tmax:"+num_to_str(act->getTMax())+"\n");

	}

	/*Calcular Hogura Libre*/
	begin->calculHolguraFree(end->getTMax());

	/*Calcular caminos criticos*/
	Path * path= new Path();
	extractCriticalPaths(path, this->begin);
}









/****************************************************************/
/*							ANALISIS							*/
/****************************************************************/

float Project::getProbabDurationLessThan(int x, double var)
{
	double media=end->getTMax();
	double var_max=0;
	double var_temp=0;

	/*Valido?*/
	if(x<0)
		return 0;
	if(var<0)
		return 0;

	/*Probabilidad de que el proyecto termine antes de x, segun la varianza*/
	return GetCumulativeDensityNormal( x,media,sqrt(var))*100;
}

float Project::getProbabDurationBetween(int x,int y, double var)
{
	double media=end->getTMax();

	/*Valido?*/
	if(x<0||y<0)
		return 0;
	if(var<0)
		return 0;

	/*Probabilidad de que el proyecto termine entre x e y segun la varianza*/
	float norm_x=GetCumulativeDensityNormal( x,media,sqrt(var));
	float norm_y=GetCumulativeDensityNormal( y,media,sqrt(var));
	return (norm_y-norm_x)*100;	
}

int Project::getDurationByProbability(double x, double var)
{
	double media=end->getTMax();

	/*Valido?*/
	if(x<0||x>1)
		return 0;
	if(var<0)
		return 0;

	/*Duracion del proyecto con una probabilidad x y una varianza dada*/
	return GetCumulativeDensityNormalInv( x, media,sqrt(var));
}

string  Project::analizeDelayTMin(int days, string name_activity)
{
	int hlr;
	int n_resources=0;
	Activity * activity= getActivity( name_activity);
	Activity * act;
	Resource * rec;
	Activity::resource * rec_asig;
	string inform="";
	bool exceded_limitation=false;

	/*ANALISIS FLEXIBILIDAD*/

	if(activity==NULL)
	{
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name_activity);
		return "";
	}

	hlr=activity->getHLR();

	/*Valido?*/
	if(days<0)
		return "Valor de dias no valido\n";

	/*Adelanta la actividad mas de instante de inicio del proyecto?*/
	if(days>activity->getTMin())
		return "No se puede adelantar la actividad mas alla del instante de inicio de proyecto\n";

	/*Factible?*/
	if(days>hlr)
		return "Retraso no factible, augmentaria la duracion del proyecto\n";

	


	/*Como se ve afectada la limitacion?*/

	/*Para los nuevos instantes que se realizaria la actividad*/
	for(int d=activity->getTMin()+days; d<=activity->getTMin()+days+activity->getTNormal();d++)
	{
		/*Comprobar todos los recursos*/
		for(int r=0; r<activity->getResources().size();r++)
		{
			rec=activity->getResources().at(r)->resource_asig;
			n_resources=0;

			/*Selecciona las actividades que se estan realizando ese dia, sin incluir la actividad a analizar*/
			for(int i=0; i<activities.size();i++)
			{
				act=activities.at(i);
				if(act->getTMin()<=d&&act->getTMin()+act->getTNormal()-1>=d && act->getName()!=activity->getName()) 
				{
					rec_asig=act->getResource( rec->getName());
					if(rec_asig!=NULL)
						n_resources+=rec_asig->units_asig;
				}
			}

			/*El consumo de recursos mas el consumo que haria esa actividad si se realizase en ese instante,
			supera el limite de recursos?*/
			if(n_resources+activity->getResource(rec->getName())->units_asig>rec->getUnitsMax())
			{
				/*Guardamos todos los añalisis para cada recurso*/
				if(!exceded_limitation)
					inform.append("La actividad se podria retrasar sin repercutir en la duracion del proyecto, pero:\n");
				inform.append("	Excederia el maximo de recurso "+ rec->getName()+" en "+num_to_str(n_resources+activity->getResource(rec->getName())->units_asig-rec->getUnitsMax())+" unidades en el instante "+ num_to_str(d)+"\n");
				exceded_limitation=true;
			}
		
		}
	}

	if(!exceded_limitation)
		return "Retraso factible, no se modificaria la duracion del proyecto y se seguiria cumpliendo la limitacion de recursos\n";

	else
		return inform;
}


string  Project::analizeAnticipateTMin(int days, string name_activity)
{

	int hla;
	int n_resources=0;
	Activity * activity= getActivity( name_activity);
	Activity * act_pred, *act;
	Resource * rec;
	Activity::resource * rec_asig;
	string pred_problem="";
	string inform="";
	
	bool exceded_limitation=false;

	/*ANALISIS FLEXIBILIDAD*/

	if(activity==NULL)
	{
		showErrorInfo(E_ACTIVITY_NOT_EXIST, name_activity);
		return "";
	}

	hla=activity->getHLA();

	/*Valido?*/
	if(days<0)
		return "Valor de dias no valido\n";

	/*No se puede adelantar mas alla de el inicio del proyecto*/
	if(days>activity->getTMin())
		return "No se puede adelantar la actividad mas alla del instante de inicio del proyecto\n";
	

	/*Es factible?*/
	if(days>hla)
	{
		if(activity->predecesorIsInitial())
			return "Adelanto no faltible, es una actividad de inicio\n";

		/*Que predecesoras condicionan su inicio temprano*/
		for(int i=0; i< activity->getList_Activities_pred()->size();i++)
		{
			act_pred=activity->getList_Activities_pred()->at(i)->activity;
			if(act_pred->getTMin()+act_pred->getTNormal()>activity->getTMin()-days)
			{
				if(pred_problem.size()>0)
					pred_problem.append("-");
				pred_problem.append(act_pred->getName());
			}
		}
		inform.append("Adelanto no factible, no se cumplirian las dependecias con las predecesoras ").append(pred_problem).append("\n");
		return inform;
	}

	

	/*Como se ve afectada la limitacion?*/

	/*Para los nuevos instantes que se realizaria la actividad*/
	for(int d=activity->getTMin()-days; d<=activity->getTMin()-days+activity->getTNormal();d++)
	{
		/*Comprobar todos los recursos*/
		for(int r=0; r<activity->getResources().size();r++)
		{
			rec=activity->getResources().at(r)->resource_asig;
			n_resources=0;

			/*Selecciona las actividades que se estan realizando ese dia, sin incluir la actividad a analizar*/
			for(int i=0; i<activities.size();i++)
			{
				act=activities.at(i);
				if(act->getTMin()<=d&&act->getTMin()+act->getTNormal()-1>=d && act->getName()!=activity->getName()) 
				{
					rec_asig=act->getResource( rec->getName());
					if(rec_asig!=NULL)
						n_resources+=rec_asig->units_asig;
				}
			}

			/*El consumo de recursos mas el consumo que haria esa actividad si se realizase en ese instante,
			supera el limite de recursos?*/
			if(n_resources+activity->getResource(rec->getName())->units_asig>rec->getUnitsMax())
			{
				/*Guardamos todos los añalisis para cada recurso*/
				if(!exceded_limitation)
					inform.append("La actividad se podria retrasar sin repercutir en la duracion del proyecto, pero:\n");
				inform.append("	Excederia el maximo de recurso "+ rec->getName()+" en "+num_to_str(n_resources+activity->getResource(rec->getName())->units_asig-rec->getUnitsMax())+" unidades el instante "+ num_to_str(d)+"\n");
				exceded_limitation=true;
			}
		}
	}

	if(!exceded_limitation)
		return "Adelanto factible, se cumplirian las dependecias del proyecto y se seguiria cumpliendo la limitacion de recursos\n";
	else
		return inform;
	
}




/*********************************************************/
/*					DEBUG								*/
/********************************************************/

bool Project::startDebug(std::string sub_report_chosed)
{
	/*Habilitado el modo?*/
	if(!enabled)
		return false;

	/*Abre el fichero donde guardar la informacion de debug*/
	sub_report=sub_report_chosed;
	
	std::string rep=report+"_"+sub_report+".debug";

	output.open(rep.c_str());

	if(!output.is_open())
	{
		showErrorInfo(E_OPEN_FILE,report+"_"+sub_report+".debug");
		return false;
	}
	return true;
}

/*Metodos para impirmir la informacion de debug*/
void Project::mode_debug(std::string text)
{
	if(!enabled)
		return;
	if(!output.is_open())
		return;
	
	output<<text;	
}
	

void Project::mode_debug(string before,std::vector<Activity*> *paths, string after)
{
	if(!enabled)
		return;
	if(!output.is_open())
		return;
	output<<before;
	for(int j=0; j<paths->size();j++)
		output<<paths->at(j)->getName()<<" ";
	output<<after;

}

void Project::mode_debug(string before,std::deque<Activity*> paths, string after)
{
	if(!enabled)
		return;
	if(!output.is_open())
		return;
	output<<before;
	for(int j=0; j<paths.size();j++)
		output<<paths.at(j)->getName()<<" ";
	output<<after;

}

void Project::mode_debug(string before,std::vector<disp_resource> disp, int t, string after)
{
	if(!enabled)
		return;
	if(!output.is_open())
		return;
	output<<before;
	output<<"disp"<<t<<"{";

	for(int j=0; j<disp.size();j++)
	{
		output<<" "<<disp.at(j).resource->getName()<<"="<<disp.at(j).units_disp;
	}
	output<<"}";
	output<<after;
	
}
void Project::mode_debug(string before,std::set<disp_instant, CompareDisponibilities> *disponibility, string after)
{
	set<disp_instant, CompareDisponibilities>::iterator it=disponibility->begin();

	//output<<before;
	while(disponibility->end()!=it)
	{
		
		mode_debug(before,it->resources,it->instant,after);
		it++;
	}

	output<<after;


}

void Project::finishDebug()
{ 
	/*Cerrar el fichero de debug*/
	if(!enabled)
		return;
	if(output.is_open())
		output.close();
}

void Project::enableDebug()
{
	/*Habilitar el modo*/
	this->enabled=true;
}

void Project::disableDebug()
{
	/*Desabilitar el debug*/
	if(enabled && output.is_open())
			output.close();

	enabled=false;

}

void Project::setReportDebug(string path)
{
	if(output.is_open())
		showErrorInfo(E_UPDATE_PATH,"debug");
	else
		this->report=path.append("report");
}