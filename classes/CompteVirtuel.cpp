#include "CompteVirtuel.h"

CompteVirtuel::CompteVirtuel(const string& _identifiant, const string& _nom, const Type _type):
    Compte(_identifiant, _nom, _type), Composite()
{}

CompteVirtuel::CompteVirtuel(const string& _identifiant, const string& _nom, CompteVirtuel* _parent):
    Compte(_identifiant, _nom, _parent), Composite()
{}

void CompteVirtuel::supprimerCompte() {
    if (listeSousComptes.empty()) {
        getParent()->supprimerSousCompte(*this);
        delete this;
    }
    else throw("Ce compte ne peut pas être supprimé car il a un ou plusieurs sous-compte(s).");
}
