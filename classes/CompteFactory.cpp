
#include "CompteFactory.h"
#include "Compte.h"
#include "CompteRacine.h"
#include "CompteFeuille.h"
#include "CompteVirtuel.h"
#include "TransactionManager.h"
#include <algorithm>

/*!
 * Crée un  compte et l'ajoute dans les sous-comptes de la racine.
 *
 * Si c'est un CompteFeuille, appelle la création du solde initial
 * \param type : Type du Compte créé
 * \param nom : Nom du Compte Créé
 * \param virtuel : Booléen représentant le caractère virtuel d'un Compte
 * \param montant : Solde initial du Compte
 * \param passif : Compte passif pour le solde initial
 * \return Référence sur le Compte créé
 */
Compte& CompteFactory::creerCompte(Type type, const std::string& nom, bool virtuel, float montant, CompteFeuille* passif) {
    CompteRacine& racine = CompteRacine::getCompteRacine();
    std::string id = std::to_string(racine.maxId()+1);
    Compte* newCompte = nullptr;
    if(virtuel==true) newCompte = new CompteVirtuel(id, nom, type);
    else newCompte = new CompteFeuille(id, nom, type);
    racine.ajouterSousCompte(*newCompte);
    if(!virtuel) this->setSoldeInitial(dynamic_cast<CompteFeuille*>(newCompte), montant, passif);
    return *newCompte;
}

/*!
 * Crée un  compte et l'ajoute dans les sous-comptes du parent. Le type est celui du parent.
 *
 * Si c'est un CompteFeuille, appelle la création du solde initial
 * \param parent : Parent du Compte créé
 * \param nom : Nom du Compte Créé
 * \param virtuel : Booléen représentant le caractère virtuel d'un Compte
 * \param montant : Solde initial du Compte
 * \param passif : Compte passif pour le solde initial
 * \return Référence sur le Compte créé
 */
Compte& CompteFactory::creerCompte(CompteVirtuel &parent, const std::string& nom, bool virtuel, float montant, CompteFeuille* passif) {
    std::string id = std::to_string(CompteRacine::getCompteRacine().maxId()+1);
    Compte* newCompte = nullptr;
    if(virtuel==true)  newCompte = new CompteVirtuel(id, nom, &parent);
    else newCompte = new CompteFeuille(id, nom, &parent);

    parent.ajouterSousCompte(*newCompte);
    if(!virtuel) this->setSoldeInitial(dynamic_cast<CompteFeuille*>(newCompte), montant, passif);
    return *newCompte;
}

Compte& CompteFactory::creerCompte(const std::string& id, CompteVirtuel& parent, const std::string& nom, float solde_rap, QDate date_rap, bool virtuel){
    Compte* newCompte = nullptr;
    if(virtuel==true)  newCompte = new CompteVirtuel(id, nom, &parent);
    else newCompte = new CompteFeuille(id, nom, &parent);
    newCompte->setDateRap(date_rap);
    newCompte->setSoldeRap(solde_rap);
    parent.ajouterSousCompte(*newCompte);
    return *newCompte;
}

Compte& CompteFactory::creerCompte(const std::string& id, Type type,  const std::string& nom, float solde_rap, QDate date_rap, bool virtuel){
    CompteRacine& racine = CompteRacine::getCompteRacine();
    Compte* newCompte = nullptr;
    if(virtuel==true) newCompte = new CompteVirtuel(id, nom, type);
    else newCompte = new CompteFeuille(id, nom, type);
    newCompte->setDateRap(date_rap);
    newCompte->setSoldeRap(solde_rap);
    racine.ajouterSousCompte(*newCompte);
    return *newCompte;
}

/*!
 * Crée une transaction entre ce Compte et et un Compte passif intitulée solde initial.
 * \param compte : Compte à initialiser
 * \param montant : Montant de la transaction
 * \param passif : CompteFeuille de type passif à utiliser pour la transaction
 */
void CompteFactory::setSoldeInitial(CompteFeuille* compte, float montant, CompteFeuille* passif){
    if ((compte->getType()==Type::A || compte->getType()==Type::P) && montant>0 && passif && !passif->estVirtuel() && passif->getType()==Type::P){
        list<TripletTransaction> liste;
        if(compte->getType()==Type::A || compte->getType()==Type::D){
            liste.push_back(TripletTransaction(compte, montant, 0));
            liste.push_back(TripletTransaction(passif, 0, montant));
        }
        else{
            liste.push_back(TripletTransaction(dynamic_cast<CompteFeuille*>(compte), 0, montant));
            liste.push_back(TripletTransaction(passif, montant, 0));
        }
        TransactionManager::getTransactionManager().creerTransaction(QDate::currentDate(), "Solde initial", "INIT",liste );
    }

}
