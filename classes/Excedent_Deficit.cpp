#include "Excedent_Deficit.h"
#include "CompteRacine.h"
 
Excedent* Excedent::instance=nullptr;
Deficit* Deficit::instance=nullptr;
Resultat* Resultat::instance=nullptr;

Excedent::Excedent():CompteFeuille("A","Excedent",Type::P){
    CompteRacine::getCompteRacine().ajouterSousCompte(*this);
}

Deficit::Deficit():CompteFeuille("B","Deficit",Type::P){
    CompteRacine::getCompteRacine().ajouterSousCompte(*this);
}

Resultat::Resultat():CompteFeuille("C","Resultat",Type::P){
    CompteRacine::getCompteRacine().ajouterSousCompte(*this);
}
