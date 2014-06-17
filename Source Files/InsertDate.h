#ifndef INSERTDATE_H
#define INSERTDATE_H

#include <QWidget>
#include <QLabel>
#include <QDate>
#include <QLayout>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>

class InsertDate : public QWidget
{
    Q_OBJECT
public:
    explicit InsertDate(QTableWidget *table, QVector<QDate> *vec_dates, QWidget *parent = 0);
    ~InsertDate();
    void BuildWindow();


public slots:
    void SaveDate();
    bool CheckRepeated(QDate *Date);

private:
    QVector<QDate>  *dates;
    QTableWidget    *tabla;

    QHBoxLayout *HBox;
    QSpinBox *dd;    QSpinBox *mm;    QSpinBox *yyyy;
    QPushButton *Guardar;    QPushButton *Cancelar;
};

#endif // INSERTDATE_H
