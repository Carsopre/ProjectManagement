/*#######################################################################################
Clase Date provee al programador una forma sencilla de operar con fechas y hora.
Copyright (C) Gammexane
Gammexane@Yahoo.com.ar

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
########################################################################################*/

#ifndef _DATE_H
#define _DATE_H

#include <iostream>
#include <string>

using namespace std;

/*******************************************************************************************
*                              Clase para manipulacion de Fechas
*******************************************************************************************/
class Date{
		
	public:	
		Date(){};
		Date(const string&);
		Date(int);
		Date(int, int, int);
		Date(const Date&);
		
		string Format    (string) const;               // Formatea una fecha
		int    Julian    () const {return _Julian;}    // Devuelve un entero Juliano  
        int    Day       () const {return _Day;}       // Devuelve el Dia
        int    Month     () const {return _Month;}     // Devuelve el Mes
        int    Year      () const {return _Year;}      // Devuelve el Año
        int    DayOfWeek () const {return _DayOfWeek;} // Dia de la semana 0= lun 6= dom
		void   AddDay  (int = 1);					   // Incrementa Dias al objeto
        void   AddMonth  (int = 1);                    // Incrementa Meses al objeto 
        void   AddYear   (int = 1);                    // Incrementa Años al objeto 
        int    LastDay   () const;                     // Cantidad de dias del mes
        bool   IsLeapYear() const;                     // Año Bisiesto 
  		
  		Date& operator=(int);                          // Asignacion Juliano
  		Date& operator=(const string&);                // Asignacion String
  		Date& operator=(const Date&);                  // Asignacion Date
  		
  		Date  operator ++ ();                          // Prefijo  Incrementa 1 dia
        Date  operator ++ (int);                       // Sufijo Incrementa 1 dia  
        Date  operator -- ();                          // Prefijo  DeIncrementa 1 dia 
        Date  operator -- (int);                       // Sufijo DeIncrementa 1 dia
        const Date &operator += (int);                 // Incrementa X Dias
        const Date &operator -= (int);                 // DeIncrementa X Dias
        
        Date  operator* (int);                         // Incrementa Meses 
        Date& operator*=(int);
        Date  operator^ (int);                         // Incrementa Años
        Date& operator^=(int); 
              
        int operator == (const Date&);
        int operator != (const Date&); 
        int operator >= (const Date&);
        int operator <= (const Date&); 
        int operator <  (const Date&);
        int operator >  (const Date&);        
  		
  		friend ostream& operator<<(ostream&, const Date&);
  		friend istream& operator>>(istream&, Date&);
  		friend Date     operator+ (const Date&, int);
  		friend Date     operator- (const Date&, int);
  		friend int      operator- (const Date&, const Date&);
  		
  		~Date(){};
  		       
	private:
		void   _Set(int, int, int);
		void   _ConvertToJulian(int, int, int);
		void   _SplitJulian();
		string _ItoS (int) const;
	
    protected:
		int  _Julian;
        int  _Day;
        int  _Month;
        int  _Year;
        int  _DayOfWeek;
		char _Separator;
};

#endif