#ifndef COMPTEVIRTUEL_H_INCLUDED
#define COMPTEVIRTUEL_H_INCLUDED

#include "Compte.h"
#include "Composite.h"

/*!
 * \file CompteVirtuel.h
 * \brief Déclaration de la classe CompteVirtuel.
 */

//! \class CompteVirtuel
//! \brief Classe représentant un compte virtuel
//!
//! Hérite de Compte et Composite
class CompteVirtuel : public Compte, public Composite {
private:
    friend class CompteFactory;
    //! \brief Constructeur 1.
    //!
    //! Appelle le Constructeur 1 de Compte et le constructeur par défaut de Composite
    CompteVirtuel(const string& _identifiant, const string& _nom, const Type _type);
    //! \brief Constructeur 2
    //!
    //! Appelle le Constructeur 2 de Compte et le constructeur par défaut de Composite
    CompteVirtuel(const string& _identifiant, const string& _nom, CompteVirtuel* _parent);
    //! \brief Constructeur de recopie par défaut
    CompteVirtuel(const CompteVirtuel&) = default;
    //! \brief Destructeur virtuel par défaut
    virtual ~CompteVirtuel() = default;
    //! \brief Surcharge de l'opérateur d'affectation par défaut
    CompteVirtuel& operator=(const CompteVirtuel&) = default;

public:
    //! \brief Définition de la méthode virtuelle pure estVirtuel() déclarée dans Compte
    //!
    //! Renseigne sur le fait qu'un Compte soit virtuel ou non.
    bool estVirtuel() const override {return true;}
    //! \brief Définition de la méthode virtuelle pure supprimerCompte() déclarée dans Compte
    //!
    //! Supprime le CompteVirtuel
    virtual void supprimerCompte() override;
};

#endif
