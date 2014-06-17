#include <string>
#include <QMessageBox>
/*Codigos de errores*/

#define OK 1
#define E_ACTIVITY_NOT_EXIST 2
#define E_RELATION_NOT_EXIST 3
#define E_CYCLE 4
#define E_RELATION_ALREADY_EXIST 5
#define E_ACTIVITY_ALREADY_EXIST 6
#define E_VALUE_RESOURCE 7
#define E_RESOURCE_NOT_ALLOCATED 8
#define A_RESOURCE_ALREADY_ADDED 9
#define E_RESOURCE_NOT_EXIST 10
#define E_RESOURCE_ALREADY_EXIST 11
#define A_UNITS_RESOURCE_0 12
#define E_ALGORITHM_NOT_EXIST_RESOURCES 13
#define E_OPEN_FILE 14
#define E_RESOURCE_MAX_EXCEDED 15
#define E_DEPENDENCES 16
#define E_FORMAT_FILE 17
#define E_UPDATE_PATH 18




void showError(int code);

void showAvisoInfo(int code, std::string information);

void showErrorInfo(int code, std::string infomation);





