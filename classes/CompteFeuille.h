#ifndef COMPTEFEUILLE_H
#define COMPTEFEUILLE_H

#include "Compte.h"
#include <string>

/*!
 * \file CompteFeuille.h
 * \brief Déclaration de la classe CompteFeuille
 */


class CompteStrategy;
class CompteVirtuel;

//! \class CompteFeuille
//! \brief Classe instanciable représentant les Comptes sur lesquels sont effectuées des Transaction

class CompteFeuille : public Compte {

    //!Pointeur vers la classe CompteStrategy qui permet l'implémentation des méthodes de CompteFeuille
    CompteStrategy* strategy = nullptr;
public:
    /*!
        * \brief Constructeur 1
        *
        * Constructeur de CompteFeuille situé sous un CompteVirtuel
        *
        * \param identifiant : Identifiant du Compte créé
        * \param nom : Nom du Compte créé
        * \param parent : Pointeur vers le Compte parent
        */
    CompteFeuille(const std::string& identifiant,const std::string& nom, CompteVirtuel* parent);

    //!Suppression du constructeur de recopie
    CompteFeuille(const CompteFeuille& c) = delete;


     /*!
        * \brief Constructeur 2
        *
        * Constructeur de CompteFeuille situé sous le CompteRacine
        *
        * \param identifiant : Identifiant du Compte créé
        * \param nom : Nom du Compte créé
        * \param type : Type du Compte créé (classe Type)
        */
    CompteFeuille(const std::string& identifiant,const std::string& nom, const Type type);


    /*!
     * \brief Destructeur par défaut
     */
    virtual ~CompteFeuille();


    /*!
     * \brief Renvoie la virtualité ou non du CompteFeuille
     *
     * \return boolean false
     */
    bool estVirtuel() const override {return false;}


    /*!
     * \brief Permet de débiter le compte
     *
     * \param montant : montant de la Transaction
     */
    void debiter(float montant);

    /*!
     * \brief Permet de créditer le compte
     *
     * \param montant : montant de la Transaction
     */

    virtual void crediter(float montant);

    /*!
     * \brief Suppression du CompteFeuille
     */
    virtual void supprimerCompte() override;


    /*!
     * \brief Surcharge de operator== pour comparer les CompteFeuille
     */

    bool operator==(const CompteFeuille& c) const { return c.getIdentifiant()==this->getIdentifiant();}

};


#endif // COMPTEFEUILLE_H
