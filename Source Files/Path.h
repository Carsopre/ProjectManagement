
#ifndef Path_H_
#define Path_H_

class Path{

		int duration;					// duracion del camino
		std::vector<Activity*>* path;	// actividades que forman parte del camino

	public:
	
		Path(std::vector<Activity*> *path);
		Path(Path *path);
		Path(Activity* act);
		Path();
		~Path();
		void copy(Path* path);								//copia la informacion del camino que se le pasa
		void clear(){ this->path->clear();}					// resetea el camino
		std::vector<Activity*> * getPath(){ return path;};
		void calculDuration();								// calcula la duracion del camino segun las duraciones de las actividades que forman parte
		int getDuration(){ return duration;};
		void setDuration(int duration){ this->duration=duration;};
		bool includesActivity(Activity * act);				// comprueba si la actividad forma parte del camino
		std::string toString();								// devuelve en string el camino con los nombres de las actividades que forman
};

#endif;