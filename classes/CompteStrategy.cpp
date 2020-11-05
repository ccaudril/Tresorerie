#include "CompteStrategy.h"
#include "CompteFeuille.h"
#include "Compte.h"
#include "CompteVirtuel.h"
#include "CompteRacine.h"

    /* StrategyAD */

//débit d'un compte d'actif ou de dépenses : augmente le solde
void StrategyAD::debiter(float montant) {
    float solde = compte->getSolde();
    compte->setSolde(solde+montant);
    if(compte->getParent()!=nullptr && compte->getParent()!=&CompteRacine::getCompteRacine())
    {
        CompteVirtuel* papa=static_cast<CompteVirtuel*>(compte->getParent());
        papa->setSolde(papa->getSolde()+montant);
        while(papa->getParent()!=nullptr && papa->getParent()!=&CompteRacine::getCompteRacine())
        {
            papa=static_cast<CompteVirtuel*>(papa->getParent());
            papa->setSolde(papa->getSolde()+montant);
        }
    }
}

//crédit d'un compte d'actif ou de dépenses : diminue le solde
void StrategyAD::crediter(float montant) {
    float solde = compte->getSolde();
    compte->setSolde(solde-montant);
    if(compte->getParent()!=nullptr && compte->getParent()!=&CompteRacine::getCompteRacine())
    {
        CompteVirtuel* papa=static_cast<CompteVirtuel*>(compte->getParent());
        papa->setSolde(papa->getSolde()-montant);
        while(papa->getParent()!=nullptr && papa->getParent()!=&CompteRacine::getCompteRacine())
        {
            papa=static_cast<CompteVirtuel*>(papa->getParent());
            papa->setSolde(papa->getSolde()-montant);
        }
    }
}


    /* StrategyPR */

//débit d'un compte de passif ou de revenus : diminue le solde
void StrategyPR::debiter(float montant) {
    float solde = compte->getSolde();
    compte->setSolde(solde-montant);
    if(compte->getParent()!=nullptr && compte->getParent()!=&CompteRacine::getCompteRacine())
    {
        CompteVirtuel* papa=static_cast<CompteVirtuel*>(compte->getParent());
        papa->setSolde(papa->getSolde()-montant);
        while(papa->getParent()!=nullptr && papa->getParent()!=&CompteRacine::getCompteRacine())
        {
            papa=static_cast<CompteVirtuel*>(papa->getParent());
            papa->setSolde(papa->getSolde()-montant);
        }
    }
}

//crédit d'un compte de passif ou de revenus : augmente le solde
void StrategyPR::crediter(float montant) {
    float solde = compte->getSolde();
    compte->setSolde(solde+montant);
    if(compte->getParent()!=nullptr && compte->getParent()!=&CompteRacine::getCompteRacine())
    {
        CompteVirtuel* papa=static_cast<CompteVirtuel*>(compte->getParent());
        papa->setSolde(papa->getSolde()+montant);
        while(papa->getParent()!=nullptr && papa->getParent()!=&CompteRacine::getCompteRacine())
        {
            papa=static_cast<CompteVirtuel*>(papa->getParent());
            papa->setSolde(papa->getSolde()+montant);
        }
    }
}
