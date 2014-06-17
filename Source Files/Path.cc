
#include "Activity.h"
#include "Path.h"
/********************************************/
/*				Constructores				*/
/********************************************/
Path::Path(std::vector<Activity*> *path_other)
{
	/*Crear el camino a partir de un conjunto de acitvidades*/
	path= new std::vector <Activity *>;
	for(int i=0; i<path_other->size();i++)
	{
		Activity * act=path_other->at(i);
		path->push_back(act);
	}

}

Path::Path(Path *path_other)
{
	/*Crear el camino a partir de otro dado*/
	path= new std::vector <Activity *>;
	for(int i=0; i<path_other->getPath()->size();i++)
	{
		Activity * act=path_other->getPath()->at(i);
		path->push_back(act);
	}	
}


Path::Path(Activity* act)
{
	path= new std::vector <Activity *>;
	path->push_back(act);
}

Path::Path()
{
	path= new std::vector <Activity *>;
}




/*Destructor*/
Path::~Path()
{
	delete(path);
}

/********************************************/
/*				Metodos						*/
/********************************************/
void Path:: copy(Path* path_other)
{	
	/*copiar las acitivdades de otro camino*/
	for(int i=0; i<path_other->path->size(); i++)
		path->push_back(path_other->path->at(i));

	/*resetear el camino*/
	path_other->getPath()->clear();
}

void Path::calculDuration()
{
	/*Calcular la duracion del camino como suma de la
	duracion de las actividades que lo componen*/
	this->duration=0;

	for(int i=0; i<this->path->size();i++)
		duration+=path->at(i)->getTNormal();
}


bool Path::includesActivity(Activity * act)
{
	/*Comprobar si esa actividad esta dentro del camino*/
	if(act==NULL)
		return false;

	for(int i=0; i<this->path->size();i++)
		if(path->at(i)->getName()==act->getName())
			return true;

	return false;

}



std::string Path::toString()
{
	/*Obtener las actividades que forman el camino en formato string*/
	std::string path_text="";
	for(int i=0; i<this->path->size(); i++)
	{
		if(i!=0)
			path_text.append("-");	
		path_text.append(path->at(i)->getName());

	}
	return path_text;

}