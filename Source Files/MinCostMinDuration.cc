#include "Project.h"
#include <string>
#include <sstream>

/************************************************************/
/*			MIN COST MIN DURACION							*/
/***********************************************************/

void Project :: extractPaths( vector<Path*> *paths,Path * path, Activity* begin ){

	Activity * act;

	/*No hay mas actividades, se añade el camino a la lista*/
	if(begin->succesorIsEnd())
	{
		Path * finished_path=new Path(path); //guarda el camino y lo añade
		paths->push_back(finished_path);
		path->clear();						//limpia el camino para posteriores usos
	}
	else
	{
		/*Tiene succesoras y las examinamos*/
		for(int i=0; i< begin->getList_Activities_suc()->size(); i++)
		{
			act=begin->getList_Activities_suc()->at(i)->activity;
			Path * path2=new Path(path);	// guardamos el camino antes de modificarlo

			/*Se añade una de las succesoras y seguimos examinando su succesores*/
			path->getPath()->push_back(act);
			extractPaths(paths, path, act);

			/*Cuando terminamos de examinar el anterior camino, 
			restauramos el camino que teniamos hasta entonces y seguimos buscando*/
			path->copy(path2);
			delete(path2); 
		}
	}
	path->clear();
}



float calcul_overrun(std:: vector <Path*> paths,Path * activities)
{

	int cost=0;
	int j;

	/*No hay actividades seleccionadas*/
	if(activities->getPath()->size()==0)
		return INF;


	/*Comprobar si todos los caminos almenos tienen una actividad de las elegidas para reducir su t.normal */
	for(int i=0; i<paths.size();i++)
	{
		for(j=0; j<activities->getPath()->size();j++)
			if(paths.at(i)->includesActivity(activities->getPath()->at(j)))
				break;

		/*Hay almenos un camino que no tiene ninguna actividad a reducir*/
		if(j==activities->getPath()->size())
			return INF;   // no es una solucion factible
	}

	/*calcula el coste*/
	for(int i=0; i<activities->getPath()->size();i++)
	{
		for(int j=0;j<paths.size();j++)
		{
			if(paths.at(j)->includesActivity(activities->getPath()->at(i)))
			{
				// solo contamos la acitividad una vez, da igual en que camino este
				cost+=activities->getPath()->at(i)->getOportunityCost();
				break;
			}	
		}
	}

	return cost;
}


float Project:: get_min_cost(std::vector<Path*> paths_to_modif, int n_act,Path * activities_to_modif, Path * solution)
{
	float con_act=INF;
	Activity * act;
	float t_calculated;
	Path * aux=new Path();
	Path * solution_with_act=new Path();
	Path * solution_without_act=new Path();
	float sin_act;

	/*Probar todas las posibles combinaciones de actividades a modificar y elegir la de minimo coste*/

	/*Todas la actividades se han tenido en cuenta*/
	if(n_act==activities.size())
	{
		/*Calcula el sobrecoste de esa eleccion de acividades*/
		t_calculated=calcul_overrun(paths_to_modif,activities_to_modif); // es factible?
		solution->copy(activities_to_modif);
		return t_calculated;
	}
	else /*BACKTRACKING: solucion sin esta actividad, y con esta actividad*/
	{
		aux=new Path(activities_to_modif);	// guardamos las actividades seleccionadas hasta el momento

		/*Solucion sin haber cogido esta actividad*/
		sin_act=get_min_cost(paths_to_modif, n_act+1,activities_to_modif, solution_without_act); 

		activities_to_modif=new Path(aux);	// restaurar la lista a como estaba antes de calcular la solucion sin la actividad
		act=activities.at(n_act);			
		
		/*Se puede reducir los dias de esta acitivdad?*/
		if(act->getDifTNormalTope()>0) 
		{
			/*Solucion habiendo añadido la actividad a la solucion*/
			activities_to_modif->getPath()->push_back(act);
			con_act=get_min_cost(paths_to_modif, n_act+1,activities_to_modif,solution_with_act);
		}
		activities_to_modif=new Path(aux); // restaurar la lista a como estaba antes de calcular la solucion con la actividad

		/* Es mejor solucion, sin la actividad o con la actividad?*/
		if(con_act<sin_act)
		{
			solution->copy(solution_with_act);
			return con_act;
		}
		else
		{
			solution->copy(solution_without_act);
			return sin_act;
		}
	}
}



int max(std::vector<Path*> *paths) //calcula cual camino dura mas
{
	int max=-1;
	for(int i=0; i<paths->size();i++)
		if(max<paths->at(i)->getDuration())
			max=paths->at(i)->getDuration();	
	return max;
}

int calcul_next_instant(std::vector<Path*> *paths,Path* activities)
{
	int j;
	int next_instant=-1;
	std::vector<Path*> *paths_without_activities= new std::vector<Path*>;

	/*Guardar los caminos que no incluyen actividades que se reduciran*/
	for(int i=0; i< paths->size();i++)
	{
		for(j=0; j<activities->getPath()->size();j++)
			if(paths->at(i)->includesActivity(activities->getPath()->at(j)))
				break;

		/*El camino no tiene niguna de las activdades*/
		if(j==activities->getPath()->size())
			paths_without_activities->push_back(paths->at(i));
	}


	/*De los caminos sin actividades a reducir, entre estos obtener el que dura mas tiempo */
	next_instant=max(paths_without_activities);

	delete(paths_without_activities);
	return next_instant;

}

int min_dif_between_tnormal_ttope(std::vector<Activity*> *activities){

	float min=INF;

	/*entre las actividades ,encontrar la minima diferencia entre el t. normal y el tope*/
	for(int i=0; i<activities->size();i++)
	{
		if(min>activities->at(i)->getDifTNormalTope())
			min=activities->at(i)->getDifTNormalTope();
	}

	
	return min;
}

Path * Project::select_activities_min_cost(std::vector<Path*> *paths, float &cost, int& max_decr_days)
{

	int Time=max(paths); // cual es la duracion del camino que tarda mas
	int next_instant=0;
	Path* activities_to_modif_pruebas=new Path();
	Path* activities_to_modif_solution=new Path();
	std:: vector <Path*> paths_to_modif;

	mode_debug("Maxima duracion a reducir = "+num_to_str(Time)+"\n");

	/*Guardar todos los caminos que se quieren reducir porque son los que mas tardan, que seran los caminos criticos*/
	for(int i=0; i< paths->size();i++)
	{
		if(paths->at(i)->getDuration()==Time)
			paths_to_modif.push_back(paths->at(i));
	}

	/*calcula el coste de modificar los caminos criticos*/
	cost=get_min_cost(paths_to_modif, 0, activities_to_modif_pruebas, activities_to_modif_solution);

	/*No es posible reducir todos los caminos criticos*/
	if(cost==INF)
		return NULL;

	/*Se elige, de los caminos que no contienen actividades  a reducir, el de mayor duracion*/
	next_instant=calcul_next_instant(paths,activities_to_modif_solution);

	/*obtener el minimo numero de dias que se pueden decrementar las actividades que forman parte de los caminos a reducir*/
	max_decr_days=min_dif_between_tnormal_ttope(/*paths,*/activities_to_modif_solution->getPath());
	
	/*Si hay algun camino que no contiene actividades a reducir*/
	if(next_instant!=-1)
	{
		/*calcular el minimo entre: combertir el camino no critico, de maxima duracion , en critico o
		los dias maximo que se pueden reducir todas las actividades segun su t.normal-t.tope */
		if(max_decr_days>Time-next_instant)
			max_decr_days=Time-next_instant;
	}	
		
	/*numero de dias que se reduce * coste*/
	cost*=max_decr_days;

	delete(activities_to_modif_pruebas); 
	return activities_to_modif_solution;

}

void Project::decrement_days(std::vector<Path*> *paths,int max_decr_days,Path*act_to_modif){

	int max_decr_days_allowed=max_decr_days;
	Activity * act;
	Path* path;

	mode_debug("\nDecremento de dias de los caminos = "+num_to_str(max_decr_days_allowed)+"\n-----------------------------\n");


	/*Reducir el t.normal de las actividades a modificar, lo maximo que se pueda*/
	for(int i=0; i< act_to_modif->getPath()->size();i++)
	{
		act=act_to_modif->getPath()->at(i);
		act->setDifTNormalTope(act->getDifTNormalTope()-max_decr_days_allowed);
		
		/*Reducir la duracion de los caminos que incluyen la actividad*/
		for(int j=0; j<paths->size();j++)
		{
			path=paths->at(j);
			if(path->includesActivity(act))
				path->setDuration(path->getDuration()-max_decr_days_allowed);
		}
	}

}





Path * Project::select_activities_min_cost(std::vector<Path*> *paths, float &cost, int& max_decr_days, int cost_searched)
{

	int Time=max(paths);  // cual es la duracion del camino que tarda mas
	int next_instant=0;
	Path* activities_to_modif_pruebas=new Path();
	Path* activities_to_modif_solution=new Path();
	std:: vector <Path*> paths_to_modif;
	stringstream cost_searched_s;
	int days=1;

	mode_debug("Maxima duracion a reducir = "+num_to_str(Time)+"\n");

	/*Guardar todos los caminos que se quieren reducir porque son los que mas tardan, que seran los caminos criticos*/
	for(int i=0; i< paths->size();i++)
	{
		if(paths->at(i)->getDuration()==Time)
			paths_to_modif.push_back(paths->at(i));
	}

	/*calcula el coste de modificar los caminos criticos*/
	cost=get_min_cost(paths_to_modif, 0, activities_to_modif_pruebas, activities_to_modif_solution);

	/*No es posible reducir todos los caminos criticos*/
	if(cost==INF)
		return NULL;

	/*Se elige, de los caminos que no contienen actividades  a reducir, el de mayor duracion*/
	next_instant=calcul_next_instant(paths,activities_to_modif_solution);

	/*obtener el minimo numero de dias que se pueden decrementar las actividades que forman parte de los caminos a reducir*/
	max_decr_days=min_dif_between_tnormal_ttope(/*paths,*/activities_to_modif_solution->getPath());
	
	/*Si hay algun camino que no contiene actividades a reducir*/
	if(next_instant!=-1)
	{
		/*calcular el minimo entre: combertir el camino no critico, de maxima duracion , en critico o
		los dias maximo que se pueden reducir todas las actividades segun su t.normal-t.tope */
		if(max_decr_days>Time-next_instant)
			max_decr_days=Time-next_instant;
	}	
		
	
	/*Calculo de coste*/
	if(cost>cost_searched) // superamos el coste por dia establecido, ya no podremos reducir mas la duracion del proyecto
	{
		cost=INF;
		return NULL;
	}
	
	
	delete(activities_to_modif_pruebas); 
	return activities_to_modif_solution;

}

void Project::calculPathMinTimeMinCost(int cost_searched){

	std::vector<Path*> *paths= new vector<Path*>;
	Path * path=new Path();
	float cost;
	int max_decr_days;
	Path *act_to_modif;
	Activity * act;

	/*  INICIALIZACION  */
	mode_debug("*********INICIALIZACION**************\n\nCaminos:\n");

	this->overrun=0; //sobrecoste

	/*obtiene en paths todos los posible caminos del proyecto*/
	extractPaths(paths, path,this->begin);

	/*calcula la duracion en dia de cada uno de los caminos*/
	for(int i=0; i<paths->size();i++)
	{
		paths->at(i)->calculDuration();
		mode_debug("	"+num_to_str(i)+"º: ",paths->at(i)->getPath()," "+num_to_str(paths->at(i)->getDuration())+"\n");
	}

	/*calcula el numero de dias que se puede reducir cada acitvidad , como Tiempo normal - Tiempo tope*/
	for(int i=0; i<activities.size();i++)
	{
		act=activities.at(i);
		act->setDifTNormalTope();
		mode_debug("T.Normal-T.tope "+act->getName()+" = "+num_to_str(act->getDifTNormalTope())+"\n");
	}


	/*		EJECUCION		*/
	mode_debug("\n\n****************** EJECUCION ******************\n\n");

	/*Elegimos las actividades a modificar que tengan min coste, el coste minimo 
	y cuantos days se reducira cada actividad elegida*/
	act_to_modif=select_activities_min_cost(paths,cost,max_decr_days, cost_searched);
	
	/*Se puede reducir la duracion del proyecto?*/
	while(cost!=INF)
	{
		mode_debug("Sobrecoste Minimo = "+num_to_str(cost)+"\n");
		mode_debug("Actividades a modificar",act_to_modif->getPath(),"\n");
		
		/*Sobrecoste*/
		this->overrun+=cost;

		/*Se actualizan los caminos, reducciendo los dias que duran sus actividades*/
		decrement_days(paths,max_decr_days,act_to_modif);

		/* Si el coste es mayor al permitido por dia, terminamos*/
		if(cost>cost_searched)
			break;

		/*Elegimos las activities a modificar a min cost y cuantos days se reducen*/
		act_to_modif=select_activities_min_cost(paths,cost,max_decr_days, cost_searched);
	}

	/*Actualiza los parametros del proyecto segun los resultados*/
	for(int i=0; i<activities.size();i++)
	{
		act=activities.at(i);
		act->setTNormal(act->getTTope()+act->getDifTNormalTope());
	}
	this->calculCriticalPath();

	delete(paths);
	delete(path);
	delete(act_to_modif); 
}

void Project::calculPathMinTimeMinCost(){

	std::vector<Path*> *paths= new vector<Path*>;
	Path * path=new Path();
	float cost;
	int max_decr_days;
	Path *act_to_modif;
	Activity * act;

	/*  INICIALIZACION  */
	mode_debug("*********INICIALIZACION**************\n\nCaminos:\n");
	
	this->overrun=0; //sobrecoste

	/*obtiene en paths todos los posible caminos del proyecto*/
	extractPaths(paths, path,this->begin);
	
	/*calcula la duracion en dia de cada uno de los caminos*/
	for(int i=0; i<paths->size();i++){
		paths->at(i)->calculDuration();	
		mode_debug("	"+num_to_str(i)+"º: ",paths->at(i)->getPath()," "+num_to_str(paths->at(i)->getDuration())+"\n");
	}
	

	/*calcula el numero de dias que se puede reducir cada acitvidad , como Tiempo normal - Tiempo tope*/
	for(int i=0; i<activities.size();i++)
	{
		act=activities.at(i);
		act->setDifTNormalTope();
		mode_debug("\nT.Normal-T.tope "+act->getName()+" = "+num_to_str(act->getDifTNormalTope())+"\n");
	}


	/*		EJECUCION		*/
	mode_debug("\n\n****************** EJECUCION ******************\n\n");

	/*Elegimos las actividades a modificar que tengan min coste, el coste minimo 
	y cuantos days se reducira cada actividad elegida*/
	act_to_modif=select_activities_min_cost(paths,cost,max_decr_days);
	

	/*Se puede reducir la duracion del proyecto?*/
	while(cost!=INF)
	{
		mode_debug("Sobrecoste Minimo = "+num_to_str(cost)+"\n");
		mode_debug("Actividades a modificar: ",act_to_modif->getPath(),"\n");
		
		/*Sobrecoste*/
		this->overrun+=cost;

		/*Se actualizan los caminos, reducciendo los dias que duran sus actividades*/
		decrement_days(paths,max_decr_days,act_to_modif);

		/*Elegimos las activities a modificar a min cost y cuantos days se reducen*/
		act_to_modif=select_activities_min_cost(paths,cost,max_decr_days);
	}

	/*Actualiza los parametros del proyecto segun los resultados*/
	for(int i=0; i<activities.size();i++)
	{
		act=activities.at(i);
		act->setTNormal(act->getTTope()+act->getDifTNormalTope());
	}
	this->calculCriticalPath();

	delete(paths);
	delete(path);
	delete(act_to_modif);
}

