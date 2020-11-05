#ifndef POOGNON_COMPTEFACTORY_H
#define POOGNON_COMPTEFACTORY_H

#include <string>
#include <iostream>
#include <QDate>

/*!
 * \file CompteFactory.h
 * \brief Déclaration de la classe CompteFactory.
 */

class Compte;
enum class Type;
class CompteVirtuel;
class CompteFeuille;

//! \class CompteFactory
//! \brief Construction des Compte
class CompteFactory {
    friend class SauvegardeXML;

public:
    //! \brief Constructeur par défaut
    CompteFactory()= default;
    //! \brief Destructeur
    ~CompteFactory()= default;
    //! \brief Constructeur de recopie
    CompteFactory(const CompteFactory&)= default;
    //! \brief Surcharge opérateur =
    CompteFactory& operator=(const CompteFactory&)= default;

    //! \brief Crée un compte sous un parent
    Compte& creerCompte(CompteVirtuel& parent, const std::string& nom,  bool virtuel, float soldeInitial=0, CompteFeuille* passif=nullptr);

    //! \brief Crée un compte sous la racine
    Compte& creerCompte(Type type,  const std::string& nom, bool virtuel,float soldeInitial=0, CompteFeuille* passif=nullptr);

private:
    //! \brief Ajout solde initial
    void setSoldeInitial(CompteFeuille* compte, float montant, CompteFeuille* passif);

    //! \brief Crée un compte sous un parent avec un id. Uniquement appelé lors d'import de XML
    Compte& creerCompte(const std::string& id, CompteVirtuel& parent, const std::string& nom, float solde_rap, QDate date_rap, bool virtuel);

    //! \brief Crée un compte sous la racine avec un id. Uniquement appelé lors d'import de XML
    Compte& creerCompte(const std::string& id, Type type,  const std::string& nom, float solde_rap, QDate date_rap, bool virtuel);
};


#endif //POOGNON_COMPTEFACTORY_H
