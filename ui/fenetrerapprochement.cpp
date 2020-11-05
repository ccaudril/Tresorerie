#include "fenetrerapprochement.h"
#include "ui_fenetrerapprochement.h"
#include "../classes/CompteRacine.h"
#include "../classes/Composite.h"
#include "../classes/Compte.h"
#include "../classes/CompteVirtuel.h"
#include "../classes/CompteFactory.h"
#include "../classes/CompteFeuille.h"
#include "../classes/Operations.h"
#include "mainwindow.h"

#include <QString>
#include <QMessageBox>

Q_DECLARE_METATYPE(CompteRacine*)
Q_DECLARE_METATYPE(CompteVirtuel*)
Q_DECLARE_METATYPE(Composite*)
Q_DECLARE_METATYPE(Compte*)
Q_DECLARE_METATYPE(const Compte*)
Q_DECLARE_METATYPE(Type)
Q_DECLARE_METATYPE(CompteFeuille*)



fenetreRapprochement::fenetreRapprochement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fenetreRapprochement)
{
    ui->setupUi(this);
    this->setWindowTitle("Il y a du rapprochement dans l'air");


    this->ui->comptes->clear();
    this->ui->comptes->addItem("Sélectionner un compte");
    CompteRacine& cm = CompteRacine::getCompteRacine();
    for(auto it = cm.dbegin(); it!=cm.dend(); it++){
        if ((*it).estVirtuel()==false){
            QString nom = QString::fromStdString((*it).getNom());
            this->ui->comptes->addItem(nom, QVariant::fromValue(&*it));
        }
    }
    //Mettre les QLine en non éditables
    this->ui->solde->setReadOnly(true);
    this->ui->soldeRap->setReadOnly(true);

    QObject::connect(this->ui->comptes, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh(int)));
    QObject::connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &fenetreRapprochement::lancerRapprochement);

}

fenetreRapprochement::~fenetreRapprochement()
{
    delete ui;
}

void fenetreRapprochement::refresh(int i)
{
    CompteFeuille* cf=nullptr;
    cf=dynamic_cast<CompteFeuille*>(this->ui->comptes->currentData().value<Compte*>());
    if(cf!=nullptr && i==this->ui->comptes->currentIndex())
    {
        QString soldeVal;
        soldeVal.setNum(cf->getSolde());
        this->ui->solde->setText(soldeVal);

        QString soldeRapVal;
        soldeRapVal.setNum(cf->getSoldeRap());
        this->ui->soldeRap->setText(soldeRapVal);
    }

}

void fenetreRapprochement::lancerRapprochement()
{
    CompteFeuille* cf=nullptr;
    bool besoinRap;
    cf=dynamic_cast<CompteFeuille*>(this->ui->comptes->currentData().value<Compte*>());
    if(cf!=nullptr)
    {
        besoinRap = Operations::rapprocher(cf);
        if(besoinRap==true)
        {
            QString nom = QString::fromStdString(cf->getNom());
            QMessageBox::information(this, "Toujours plus de rapprochement", "Le rapprochement du compte "+nom+" a bien été effectué");
        }
        else
        {
            QMessageBox::information(this, "Pas de rapprochement", "Il n'y a pas de transactions à rapprocher sur ce compte");
        }

     }
    else QMessageBox::information(this,"Pas de rapprochement","Vous n'avez pas selectionné de compte");
}

