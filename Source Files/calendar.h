#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QDate>
#include <QVector>
#include <qevent.h>
#include <InsertDate.h>

namespace Ui {
class calendar;
}

class calendar : public QWidget
{
    Q_OBJECT
    
public:
    explicit calendar(QVector<QDate> *fest, QVector<QDate> *labo,QString *weekEnd, QWidget *parent = 0);
    ~calendar();
    void getLabo();
    void getFest();

private slots:
    void LoadCal();
    void UpdateCal();
    void AddExtra();
    void DeleteExtra();
    void AddFestivo();
    void DeleteFestivo();
    void ChangeFesRow(int row, int col);
    void ChangeLabRow(int row, int col);
    void PaintCal(QDate day,bool LabFes);
    void aboutWeekends();

protected:
    void closeEvent(QCloseEvent * event){
        if(!addWin)  insertWindow->close();
        this->close();
    }

private:

    QVector<QDate> *festivos, *laborables;
    int tam_festivos, tam_laborables;
    QString *weekEndC;
    QTextCharFormat *original1, *original2;

    bool addWin;
    InsertDate  *insertWindow;
    QDate ex;
    QTextCharFormat format;
    QDate m_currentDate;
    QPen m_outlinePen;
    QBrush m_transparentBrush;
    QBrush brus;
    QPainter painter;
    QRect rect;

private:
    Ui::calendar *ui;

};

#endif // CALENDAR_H
