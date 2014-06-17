#ifndef RESOURCES
#define RESOURCES_H

#include <QWidget>
#include <QVector>
#include <QErrorMessage>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include "Project.h"

#include <QMessageBox>
#include "Exceptions.h"

namespace Ui {
class Resources;
}

class resourcesView : public QWidget
{
    Q_OBJECT
    
public:
    explicit resourcesView(Project *proyecto, QWidget *parent = 0);
    ~resourcesView();
	QPushButton *acceptDelete;

private slots:
    void createAddResource();
    void addResource();
    void editResource();
    void deleteResource();
    void cancelNewResource();
    void changeCurrentRow(int row,int col);
signals:
	void resDeleted();
private:
    //Add Resource window
    void createAddResourceWindow();
    void updateTable();
	void setValues();
    void clearAll();
    QWidget *newResource;
    QCheckBox *resUnlimited;
    QLineEdit *resName;
    QSpinBox *resAvailable;	
    QPushButton *save,*cancel;
    bool edit;
    int row2edit;
    //Resources window
    Project *pro;
    Ui::Resources *ui;

protected:
    void closeEvent(QCloseEvent * event){
        newResource->close();		
        this->close();
    }
};

#endif // RESOURCES_H
