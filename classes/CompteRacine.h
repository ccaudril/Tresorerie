
#ifndef POOGNON_COMPTERACINE_H
#define POOGNON_COMPTERACINE_H

#include "Composite.h"

/*!
 * \file CompteRacine.h
 * \brief Déclaration de la classe CompteRacine et de son Singleton.
 */

/*! \class CompteRacine
 * \brief Singleton du compte racine
 *
 * Hérite de Composite, donc possède des sous-comptes, mais n'est pas un Compte : ne possède pas type ni solde.
 *
 * Destruction du Singleton sous la responsabilité de classe

*/
class CompteRacine: public Composite {
private:
    //! \brief Chemin d'accès du fichier XML sauvegardant la hériarchie des Compte et des Transaction
    std::string cheminFichier;
    struct Handler{
        CompteRacine* instance;
        Handler():instance(nullptr){}
        ~Handler(){ delete instance; }
    };

    //! \class Handler
    //! \brief Singleton
    static Handler handler;

    //!\brief Constructeur par défaut
    CompteRacine() = default;
    //! \brief Constructeur de recopie
    CompteRacine(const CompteRacine &) = default;
    //! \brief Constructeur par affectation
    CompteRacine &operator=(const CompteRacine &) = default;

public:

    //! \brief Renvoie l'instance du Singleton
    static CompteRacine& getCompteRacine();
    //! \brief Libère l'instance du singleton
    static void libererCompteRacine();
    //! \brief Getter de l'attribut cheminFichier
    const std::string& getCheminFichier() const {return this->cheminFichier;};
    //! \brief Setter de l'attribut cheminFichier
    void setCheminFichier(const std::string& chemin) { this->cheminFichier=chemin;};
    //! \brief Renvoie l'id max de tous les Compte sous le CompteRacine
    int maxId();
};
#endif //POOGNON_COMPTERACINE_H
