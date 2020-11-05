#include "creercomptedialog.h"
#include "ui_creercomptedialog.h"
#include "../classes/CompteRacine.h"
#include "../classes/Composite.h"
#include "../classes/Compte.h"
#include "../classes/CompteVirtuel.h"
#include "../classes/CompteFactory.h"
#include "../classes/CompteFeuille.h"
#include "../classes/TransactionManager.h"
#include "mainwindow.h"
#include <math.h>

Q_DECLARE_METATYPE(CompteRacine*)
Q_DECLARE_METATYPE(CompteVirtuel*)
Q_DECLARE_METATYPE(Composite*)
Q_DECLARE_METATYPE(Compte*)
Q_DECLARE_METATYPE(const Compte*)
Q_DECLARE_METATYPE(Type)

/*!
 * Remplit les QComboBox pour le choix du compte parent, du type et du solde initial
 *
 * Connecte les signaux
 * * \param parent : Pointeur sur le QWidget parent de la pop-up
*/

creerCompteDialog::creerCompteDialog(QWidget *parent) :QDialog(parent),ui(new Ui::creerCompteDialog){
    ui->setupUi(this);
    this->setWindowTitle("Ajouter un compte");

    //Ajout des comptes dans la ComboBox
    this->ui->parentCombo->clear();
    CompteRacine& cm = CompteRacine::getCompteRacine();
    this->ui->parentCombo->addItem("Racine", QVariant::fromValue(&cm));
    for(auto it = cm.dbegin(); it!=cm.dend(); it++){
        if ((*it).estVirtuel()==true){
            QString nom = QString::fromStdString((*it).getNom());
            for (int i=0; i<(*it).getProfondeur();i++) nom.insert(0,"    ");
            this->ui->parentCombo->addItem(nom, QVariant::fromValue(&*it));
        }
    }

    //Ajout des types dans la ComboBox
    this->ui->typeCombo->clear();
    this->ui->typeCombo->addItem("Actif", QVariant::fromValue(Type::A));
    this->ui->typeCombo->addItem("Passif", QVariant::fromValue(Type::P));
    this->ui->typeCombo->addItem("Recettes", QVariant::fromValue(Type::R));
    this->ui->typeCombo->addItem("Dépenses", QVariant::fromValue(Type::D));

    //Ajout des passifs dans la comboBox de solde initial
    this->ui->soldeCombo->clear();
    for(auto it = cm.dbegin(); it!=cm.dend(); it++){
        if ((*it).estVirtuel()==false && (*it).getType()==Type::P){
            QString nom = QString::fromStdString((*it).getNom());
            this->ui->soldeCombo->addItem(nom, QVariant::fromValue(&*it));
        }
    }
    this->ui->soldeCombo->addItem("++ Créer un nouveau compte de passif");

    //Ajout des comptes dans la ComboBox pour créer un  nouveau compte passif
    this->ui->passifCombo->clear();
    this->ui->passifCombo->addItem("Racine", QVariant::fromValue(&cm));
    for(auto it = cm.dbegin(); it!=cm.dend(); it++){
        if ((*it).estVirtuel()==true && (*it).getType()==Type::P){
            QString nom = QString::fromStdString((*it).getNom());
            for (int i=0; i<(*it).getProfondeur();i++) nom.insert(0,"    ");
            this->ui->passifCombo->addItem(nom, QVariant::fromValue(&*it));
        }
    }


    //Connection des signaux
    QObject::connect(this->ui->parentCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &creerCompteDialog::updateType);
    QObject::connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &creerCompteDialog::ajouterCompte);
    QObject::connect(this->ui->soldeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &creerCompteDialog::affichageSoldeInitial);
    QObject::connect(this->ui->virtuelBox, &QCheckBox::stateChanged, this, &creerCompteDialog::affichageSoldeInitial);
    QObject::connect(this->ui->typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &creerCompteDialog::affichageSoldeInitial);

    this->affichageSoldeInitial();
}

void creerCompteDialog::affichageSoldeInitial(){
    Type t = this->ui->typeCombo->currentData().value<Type>();
    bool cond1 = !this->ui->virtuelBox->isChecked() && (t==Type::A || t==Type::P);
    this->ui->groupSolde->setVisible(cond1);
    this->ui->groupPassif->setVisible(cond1 && this->ui->soldeCombo->currentIndex()==this->ui->soldeCombo->count()-1);

}

/*! \brief Destructeur*/
creerCompteDialog::~creerCompteDialog()
{
    delete ui;
}

/*!
 * Si un compte parent différent de la Racine est sélectionné, la liste Type est figée sur le Type du parent
 */
void creerCompteDialog::updateType(){
    if (this->ui->parentCombo->currentData().value<CompteRacine*>()==&CompteRacine::getCompteRacine()){
        this->ui->typeCombo->setEnabled(true);
    }
    else{
        if(this->ui->parentCombo->currentData().canConvert<Compte*>()){
            Type t = dynamic_cast<CompteVirtuel*>(this->ui->parentCombo->currentData().value<Compte*>())->getType();
            this->ui->typeCombo->setCurrentIndex(this->ui->typeCombo->findData(QVariant::fromValue(t)));
            this->ui->typeCombo->setEnabled(false);
        }

    }
}

/*!
 * Ajoute le compte dans la hiérarchie avec la CompteFactory et ferme la Dialog.
 * Nom du compte obligatoirement non nul, montant inital positif ou nul. Si nouveau compte passif créé, le nom obligatoirement non nul.
 * Rafraichit l'affichage de la hiérarchie sur la fenêtre parente.
*/
void creerCompteDialog::ajouterCompte()
{
    if(this->ui->nomEdit->text()=="") {
        this->ui->lblStatus->setText("Le nom du compte ne doit pas être nul");
        return;
    }
    //Test solde initial
    float montant=0;
    Type t = this->ui->typeCombo->currentData().value<Type>();
    CompteFeuille* passif=nullptr;
    if(!this->ui->virtuelBox->isChecked() && (t==Type::A || t==Type::P) &&this->ui->soldeEdit->text()!=""){
        montant=this->ui->soldeEdit->text().toFloat();
        if(montant <=0 || round(montant*100)/100 != montant){
            this->ui->lblStatus->setText("Le montant initial est invalide");
            return;
        }
        else if(this->ui->soldeCombo->count()-1!=this->ui->soldeCombo->currentIndex()){
            passif=dynamic_cast<CompteFeuille*>(this->ui->soldeCombo->currentData().value<Compte*>());
        }
        else {
            if(this->ui->passifEdit->text()=="") {
                this->ui->lblStatus->setText("Le nom du compte passif ne doit pas être nul");
                return;
            }
            passif = this->creerNouveauPassif();
        }
    }

    CompteFactory fact;
    if (this->ui->parentCombo->currentData().value<CompteRacine*>()==&CompteRacine::getCompteRacine()){
        fact.creerCompte(t, this->ui->nomEdit->text().toStdString(), this->ui->virtuelBox->isChecked(), montant, passif);
    }
    else{
        fact.creerCompte(*dynamic_cast<CompteVirtuel*>(this->ui->parentCombo->currentData().value<Compte*>()), this->ui->nomEdit->text().toStdString(), this->ui->virtuelBox->isChecked(), montant, passif);
    }
    this->accept();

}

/*!
 * \brief Crée un compte passif pour le solde initial
 * \return Pointeur vers ce compte
 */
CompteFeuille* creerCompteDialog::creerNouveauPassif(){
    CompteFactory fact;
    CompteFeuille* newPassif=nullptr;
    if (this->ui->passifCombo->currentData().value<CompteRacine*>()==&CompteRacine::getCompteRacine()){
        newPassif = dynamic_cast<CompteFeuille*>(&fact.creerCompte(Type::P, this->ui->passifEdit->text().toStdString(), false));
    }
    else{
        newPassif = dynamic_cast<CompteFeuille*>(&fact.creerCompte(*dynamic_cast<CompteVirtuel*>(this->ui->passifCombo->currentData().value<Compte*>()), this->ui->passifEdit->text().toStdString(),false));
    }
    return newPassif;
}
