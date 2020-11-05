#include "ui/mainwindow.h"
#include "classes/Composite.h"
#include "classes/Compte.h"
#include "classes/CompteFactory.h"
#include "classes/CompteRacine.h"
#include "classes/CompteVirtuel.h"
#include "classes/Transaction.h"
#include "classes/TransactionManager.h"
#include "classes/CompteFeuille.h"
#include "classes/Excedent_Deficit.h"
#include "classes/Sauvegarde.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
using namespace std;


int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.afficherListeCompte();
    w.show();
    return a.exec();
}
