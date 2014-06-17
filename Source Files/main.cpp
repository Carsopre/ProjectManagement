#include <QtGui/QApplication>
#include "Rules.h"
#include "VentanaPrincipal.h"

/*
  RECORDATORIS PAULA:

  TO-DO:
    - IMPORTANT: LÍNEA 115/116 LIMITATION RESOURCE.
    - Fer que se tanquen els informes al tancar el programa.
    - Gen. Report // Report:        
        - Histograma.
        - Pintar de color blau els laborables extra, i roig els festius.
        Gener: 1, 6
        Març: 19 (València), 29
        Maig: 1
        Agost: 15;
        Octubre: 9 (València), 12
        Novembre: 1
        Decembre: 6, 8, 25
 */

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
