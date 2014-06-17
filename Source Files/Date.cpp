/*#######################################################################################
# Clase Date provee al programador una forma sencilla de operar con fechas.
# Copyright (C) Gammexane
# Gammexane@Yahoo.com.ar
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
########################################################################################*/

#include "Date.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////
// Constructores                                          //
////////////////////////////////////////////////////////////
Date::Date(const string &SDateIn){
	*this = SDateIn;
}

Date::Date(int IDateIn){
	*this = IDateIn;
}

Date::Date(int DayIn, int MonthIn, int YearIn){
	_Set(DayIn, MonthIn, YearIn);
}

Date::Date(const Date &DDateIn){
	*this = DDateIn;
}

////////////////////////////////////////////////////////////
// Operadores                                             //
////////////////////////////////////////////////////////////

/************************ Igualacion ***********************/
Date& Date::operator=(int IDateIn){
	_Julian = IDateIn;
	_SplitJulian();
	return *this;
}

Date& Date::operator=(const string &SDateIn){
	int Day, Month, Year; 
	sscanf(SDateIn.c_str(), "%d%c%d%c%d", &Day, &_Separator, &Month, &_Separator, &Year);
	_Set(Day, Month, Year);
	return *this;
}

Date& Date::operator=(const Date &DDateIn){
	return *this = DDateIn.Julian();  
}

/**********************  Entrada/Salida  *******************/
ostream& operator <<(ostream &ostm, const Date &DateIn){
	ostm << DateIn.Format("%n/%N/%A");
	return ostm;
}

istream& operator >>(istream &ist, Date &DateIn){
	string str;
	ist >> str;
	DateIn = str;
	return ist;
}

/**********************   Aritmeticos    *******************/
Date operator +(const Date &DDateIn, int AddDays){
	Date result;
	int Sum;
	Sum = DDateIn.Julian() + AddDays;
	result = Sum;

	return result;
}

Date operator -(const Date &DDateIn, int RestDays){
	Date Result;
	int Rest;
	Rest = DDateIn.Julian() - RestDays;
	Result = Rest;
	return Result;
}

int operator -(const Date &DDateIn1, const Date &DDateIn2){
	if (DDateIn1.Julian() > DDateIn2.Julian())
		return DDateIn1.Julian() - DDateIn2.Julian();
	else
		return DDateIn2.Julian() - DDateIn1.Julian();
} 

Date Date::operator ++(){
    _Julian++;
	_SplitJulian();
	return *this;
}

Date Date::operator ++(int){
    Date temp=*this;                   
    _Julian++;                           
	_SplitJulian();
    return temp;
}

Date Date::operator --(){
    _Julian--;
	_SplitJulian();
    return *this;
}

Date Date::operator --(int){
    Date temp=*this;                    
    _Julian--;                           
	_SplitJulian();
    return temp;
}

const Date &Date::operator += (int i){
	_Julian += i;
	_SplitJulian();
	return *this;
}

const Date &Date::operator -= (int i){
	_Julian -= i;
	_SplitJulian();
	return *this;
}

Date Date::operator*(int addMonth){
	Date Result(*this);
	Result.AddMonth(addMonth);
	return Result;
}

Date& Date::operator*=(int addMonth){
	AddMonth(addMonth);
	return *this;
}

Date Date::operator^(int addYear){
	Date Result(*this);
	Result.AddYear(addYear);
	return Result;
}

Date& Date::operator^=(int addYear){
	AddYear(addYear);
	return *this;
}

/**********************   Comparacion    *******************/
int Date::operator == (const Date &DDateIn){
    return ( _Julian == DDateIn._Julian );
}

int Date::operator != (const Date &DDateIn){
    return ( _Julian != DDateIn._Julian );
}

int Date::operator >= (const Date &DDateIn){
    return ( (_Julian == DDateIn._Julian) || (_Julian > DDateIn._Julian) );
}

int Date::operator <= (const Date &DDateIn){
    return ( (_Julian == DDateIn._Julian) || (_Julian < DDateIn._Julian) );
}

int Date::operator <  (const Date &DDateIn){
	return (_Julian < DDateIn._Julian);
}

int Date::operator >  (const Date &DDateIn){
	return (_Julian > DDateIn._Julian);
}

////////////////////////////////////////////////////////////
// Funciones Publicas
////////////////////////////////////////////////////////////

//---------------------------------------------------------
// Funcion Format 
// recibe un string y reemplaza por su respectivo valor: 
// %n = dia en nro
// %d = dia en letra formato corto
// %D = dia en letra formato largo
// %l = dia en letra solo inicial
// %N = Mes en nro
// %m = mes en letra formato corto
// %M = mes en letra formato largo
// %a = Año en nro Dos digitos
// %A = Año en nro cuatro digitos
//---------------------------------------------------------

string Date::Format(string DateFormat) const{
	static const string Month_Long[12]  = {"Enero", "Febrero", "Marzo", "Abril", 
	                                       "Mayo", "Junio", "Julio", "Agosto", 
	                                       "Septiembre", "Octubre", "Noviembre", "Diciembre"};
	                                
	static const string Month_Short[12] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", 
	                                       "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
	                               
    static const string Day_Long[7]  = {"Lunes", "Martes", "Miercoles", "Jueves", 
                                         "Viernes", "Sabado", "Domingo"};
                                   
    static const string Day_Short[7] = {"Lun", "Mar", "Mie", "Jue", "Vie", "Sab", "Dom"};
    
    static const string Day_Letter[7] = {"L", "M", "M", "J", "V", "S", "D"};
                                  	                               
	while(true){
		string::size_type pos = DateFormat.find('%');
        if (pos <= 1/*strlen(DateFormat.c_str())*/){
			switch (DateFormat[pos+1]){ 
				case 'n':
					DateFormat.replace(pos, 2, _ItoS(_Day));
					break;
				case 'D':
					DateFormat.replace(pos, 2, Day_Long[_DayOfWeek]);
					break; 
				case 'd':
					DateFormat.replace(pos, 2, Day_Short[_DayOfWeek]);	
					break; 
				case 'l':
					DateFormat.replace(pos, 2, Day_Letter[_DayOfWeek]);	
					break;
				case 'N':
					DateFormat.replace(pos, 2, _ItoS(_Month));
					break;
				case 'M':
					DateFormat.replace(pos, 2, Month_Long[_Month-1]);	
					break;
				case 'm':
					DateFormat.replace(pos, 2, Month_Short[_Month-1]);
					break;
				case 'A':
					DateFormat.replace(pos, 2, _ItoS(_Year));	
					break; 
				case 'a':
					DateFormat.replace(pos, 2, _ItoS(_Year).substr(2,2));	
					break;
			}
		}else break;
	}
  	return DateFormat;
}
//---------------------------------------------------------
// Funcion AddMonth
// Agrega Meses ( tambien ver los operadores * y *= ) 
//---------------------------------------------------------
void Date::AddMonth (int AddMonth){
	int Month = _Month;
	int Year = _Year;
	
	Month += AddMonth;
	while (Month > 12){
		Month -= 12;
		Year++;
	}	
	_Set(_Day, Month, Year);
}

//---------------------------------------------------------
// Funcion AddYear
// Agrega Años ( tambien ver los operadores ^ y ^= )
//---------------------------------------------------------
void Date::AddYear  (int AddYear){
	_Year += AddYear;
    _Set(_Day, _Month, _Year);
}

//---------------------------------------------------------
// Funcion IsLeapYear
// Devuelve True si es un año bisiesto, False si no lo es
//---------------------------------------------------------
bool Date::IsLeapYear() const{
	return  ( (_Year >= 1582) ?
		  (_Year % 4 == 0  &&  _Year % 100 != 0  ||  _Year % 400 == 0 ):
		  (_Year % 4 == 0) );
}

//---------------------------------------------------------
// Funcion LastDay
// Devuelve la cantidad de dias del mes
//---------------------------------------------------------
int  Date::LastDay  () const{
	static int eom[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (IsLeapYear() == true) eom[1] = 29; 
	return eom[_Month - 1];
}

////////////////////////////////////////////////////////////
// Funciones Privadas
////////////////////////////////////////////////////////////
void  Date::_Set(int Day, int Month, int Year){
	_ConvertToJulian(Day, Month, Year);
	_SplitJulian();
}

void Date::_ConvertToJulian(int Day, int Month, int Year){
	_Julian = 
	         ( 1461 * ( Year + 4800 + ( Month - 14 ) / 12 ) ) / 4 +
		     ( 367  * ( Month - 2 - 12 * ( ( Month - 14 ) / 12 ) ) ) / 12 - 
		     ( 3    * ( ( Year + 4900 + ( Month - 14 ) / 12 ) / 100 ) ) / 4 +                    
	          Day - 32075;
}

void Date::_SplitJulian(){
    int L, n, i, j; 
    
	L = _Julian + 68569;
    n = ( 4 * L) / 146097;
    L = L - ( 146097 * n + 3 ) / 4;
    i = ( 4000 * ( L + 1 ) ) / 1461001;
    L = L - ( 1461 * i ) / 4 + 31;
    j = ( 80 * L) / 2447;
    _Day   = L - ( 2447 * j ) / 80;
    L      = j / 11;
    _Month = j + 2 - ( 12 * L );
    _Year  = 100 * ( n - 49 ) + i + L;
    
    _DayOfWeek = _Julian%7;
}

string Date::_ItoS(int Integer) const{
	char Character[17];
	string String;
	sprintf(Character, "%2d", Integer);
	if (Character[0] == ' ') Character[0] = '0';
	String = Character;
	return String;
}
