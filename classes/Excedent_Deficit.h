#ifndef EXCEDENT_DEFICIT_H
#define EXCEDENT_DEFICIT_H

#include "CompteFeuille.h"
#include "Compte.h"

#include <string>


/*!
 * \file Excedent_Deficit.h
 * \brief Déclaration des classes Excedent, Deficit et Resultat
 */


/*
    Les comptes Excedent et Deficit sont des comptes uniques, on utilise donc ici le design pattern Singleton pour 
    les implémenter
*/

class CompteRacine;

//! \class Excedent 
//! \brief Compte unique utilisé pour les clotûres
//!
//! Hérite de CompteFeuille

class Excedent : public CompteFeuille {
private:
    /*! \brief Pointeur sur l'unique instance de la classe*/
    static Excedent* instance;

    /*!
        * \brief Constructeur
        *
        * Constructeur de Excedent
        */
    Excedent();

public:

    /*!
     * \brief Permet de récupérer l'instance unique de Excedent
     *
     * \return compte Excedent
     */
    static Excedent& getExcedent(){
        if(!instance) instance=new Excedent();
        return *instance;
    }
};

//! \class Deficit 
//! \brief Compte unique utilisé pour les clotûres
//!
//! Hérite de CompteFeuille

class Deficit : public CompteFeuille {
private:
    /*! \brief Pointeur sur l'unique instance de la classe*/
    static Deficit* instance;

    /*!
        * \brief Constructeur
        *
        * Constructeur de Deficit
        */
    Deficit();

public:

    /*!
     * \brief Permet de récupérer l'instance unique de Deficit
     *
     * \return Compte Deficit
     */
    static Deficit& getDeficit(){
        if(!instance) instance=new Deficit();
        return *instance;
    }
};

//! \class Resultat
//! \brief Compte unique utilisé pour les clotûres
//!
//! //! Hérite de CompteFeuille

class Resultat : public CompteFeuille {
private:
    /*! \brief Pointeur sur l'unique instance de la classe*/
    static Resultat* instance;

    /*!
        * \brief Constructeur
        *
        * Constructeur de Resultat
        */
    Resultat();

public:

    /*!
     * \brief Permet de récupérer l'instance unique de Resultat
     *
     * \return Compte Resultat
     */
    static Resultat& getResultat(){
        if(!instance) instance=new Resultat();
        return *instance;
    }
};



#endif // EXCEDENT_DEFICIT_H
