#ifndef COMPTESTRATEGY_H_INCLUDED
#define COMPTESTRATEGY_H_INCLUDED

    /*!
    * \file CompteStrategy.h
    * \brief Déclaration des classes CompteStrategy, StrategyAD, StrategyPR
    */

#include "CompteFeuille.h"
#include "Compte.h"
#include <string>

using namespace std;

    /*! \class CompteStrategy
    * \brief Classe abstraite, mère des classes StrategyAD et StrategyPR.
    *
    * La classe CompteStrategy permet de diviser les opérations de crédit
    * et de débit en deux classes : sur les comptes d'actifs/dépenses et 
    * sur les comptes de passifs/revenus.*/ 

class CompteStrategy {
protected :
    /*! \brief Pointeur sur un CompteFeuille (qui peut subir des opérations)*/
    CompteFeuille* compte= nullptr;
public:  
        /*! \brief Constructeur par défaut */
    CompteStrategy()= default;

        /*! \brief Constructeur de la classe CompteStrategy à partir d'un objet CompteFeuille.
        * \param c : adresse d'un objet CompteFeuille */
    CompteStrategy(CompteFeuille& c) : compte(&c) {}

        /*! \brief Destructeur par défaut */
    virtual ~CompteStrategy()= default;

        /*! \brief Méthode virtuelle pure : debiter
         *
        * Cette méthode sera redéfinie dans les classes filles de la classe CompteStrategy.
        * \param montant : montant de la transaction à débiter
        */
    virtual void debiter(float montant) = 0;

        /*! \brief Méthode virtuelle pure : crediter
         *
        * Cette méthode sera redéfinie dans les classes filles de la classe CompteStrategy.
        * \param montant : montant de la transaction à créditer
        */
    virtual void crediter(float montant) = 0;
};

    /*! \class StrategyAD
    * \brief Classe fille héritant de CompteStrategy.
    *
    * La classe StrategyAD permet de définir les méthodes debiter() et
    * crediter() pour des comptes d'actifs ou de dépenses.
    */ 
class StrategyAD : public CompteStrategy {
public:
    //! \brief Constructeur
    StrategyAD(CompteFeuille& c):CompteStrategy(c){}

        /*! \brief Méthode debiter
         *
        * Cette méthode permet de débiter un compte d'actif ou de dépenses.
        * \param montant : montant de la transaction à débiter
        */
    void debiter(float montant) override;

        /*! \brief Méthode crediter
         *
        * Cette méthode permet de créditer un compte d'actif ou de dépenses.
        * \param montant : montant de la transaction à créditer
        */
    void crediter(float montant) override;
};

    /*! \class StrategyPR
    * \brief Classe fille héritant de CompteStrategy.
    *
    * La classe StrategyPR permet de définir les méthodes debiter() et
    * crediter() pour des comptes de passifs ou de revenus.
    */ 
class StrategyPR : public CompteStrategy {
public:
    //! \brief Constructeur
    StrategyPR(CompteFeuille& c):CompteStrategy(c){}
        /*! \brief Méthode debiter
         *
        * Cette méthode permet de débiter un compte de passifs ou de revenus.
        * \param montant : montant de la transaction à débiter
        */
    void debiter(float montant) override;

        /*! \brief Méthode crediter
         *
        * Cette méthode permet de créditer un compte de passifs ou de revenus.
        * \param montant : montant de la transaction à créditer
        */
    void crediter(float montant) override;
};

#endif //COMPTESTRATEGY_H_INCLUDED
