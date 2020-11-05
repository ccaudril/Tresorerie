#include "Compte.h"
#include "CompteVirtuel.h"
#include "Composite.h"
#include "CompteRacine.h"


Compte::Compte(const string& _identifiant, const string& _nom, const Type _type) :
  identifiant(_identifiant),
  nom(_nom),
  solde(0),
  soldeRap(0),
  parent(&CompteRacine::getCompteRacine()),
  type(_type)
{}

Compte::Compte(const string& _identifiant, const string& _nom, CompteVirtuel* _parent) :
  identifiant(_identifiant),
  nom(_nom),
  solde(0),
  soldeRap(0),
  parent(_parent),
  type(_parent->type)
{}

Compte::Compte(const Compte& c):
  identifiant(c.identifiant),
  nom(c.nom),
  solde(c.solde),
  type(c.type)
{
  if (c.parent) parent = new Composite(*(c.parent));
}

Compte& Compte::operator=(const Compte& c) {
  if(&c!=this) {
    identifiant = c.identifiant;
    nom = c.nom;
    solde = c.solde;
    type = c.type;
    delete parent;
    parent = nullptr;
    if(c.parent) parent = new Composite(*(c.parent));
  }
  return *this;
}

int Compte::getProfondeur() const{
    Composite* pere = this->parent;
    int profondeur=1;
    while(pere!= &CompteRacine::getCompteRacine()){
        profondeur++;
        pere = dynamic_cast<CompteVirtuel*>(pere)->parent;
    }
    return profondeur;

}
