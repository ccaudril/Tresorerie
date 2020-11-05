#include "CompteFeuille.h"
#include "CompteStrategy.h"
#include "CompteVirtuel.h"
#include "TransactionManager.h"

CompteFeuille::~CompteFeuille() {
    delete strategy;
}

CompteFeuille::CompteFeuille(const std::string& identifiant, const std::string& nom, CompteVirtuel* parent)
    :Compte(identifiant,nom,parent) //Appel au constructeur Compte 2
    {
        /*
            Dans le cas où le compte feuille est de type Actif ou Dépenses, son constructeur appelle directement
            le constructeur de StrategyAD, ce qui permet d'obtenir les bonnes versions des méthodes débiter()
            et créditer()
            Même chose si le compte feuille est de type Passif ou Recettes

            Un CompteFeuille appartient forcément à l'un des comptes Actif, Passif, Recettes ou Dépenses, et son type
            est défini selon celui de son parent
        */
        if (this->getType()==Type::A||this->getType()==Type::D)
            //strategy(StrategyAD(*this));
            this->strategy = new StrategyAD(*this);
        else if (this->getType()==Type::P||this->getType()==Type::R)
            this->strategy = new StrategyPR(*this);
}

CompteFeuille::CompteFeuille(const std::string& identifiant,const std::string& nom, const Type type):Compte(identifiant,nom,type) //Appel au constructeur Compte 1
{
    if (type==Type::A||type==Type::D){
        this->strategy = new StrategyAD(*this);
    }
    else if (type==Type::P||type==Type::R)
        this->strategy = new StrategyPR(*this);
}

void CompteFeuille::debiter(float montant){
    this->strategy->debiter(montant);
}

void CompteFeuille::crediter(float montant){
    this->strategy->crediter(montant);
}

void CompteFeuille::supprimerCompte() {
    TransactionManager& tm = TransactionManager::getTransactionManager();
    if(tm.fbegin(*this)== tm.fend(*this)){
        getParent()->supprimerSousCompte(*this);
        delete this;
    }
    else throw("Ce compte ne peut pas être supprimé car il a un ou plusieurs sous-compte(s).");
}
