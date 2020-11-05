#ifndef COMPOSITE_H_INCLUDED
#define COMPOSITE_H_INCLUDED

/*!
 * \file Composite.h
 * \brief Déclaration de la classe Composite et de ses iterateurs.
 */

#include <list>
#include <iostream>
#include "Compte.h"
class CompteVirtuel;

//! \class Composite
//! \brief Classe non instanciable mère de CompteRacine et CompteVirtuel.
//!
//! La classe Composite regroupe des attributs et méthodes nécessaires
//! au parcours des fils d'un Compte dans la hiérarchie.

class Composite {
  friend Compte::Compte(const Compte&);
  friend Compte& Compte::operator=(const Compte&);

protected:
  //! \brief Liste de ses sous-comptes directs
  std::list<Compte*> listeSousComptes;
  //! \brief Constructeur par défaut
  Composite() = default;
  //! \brief Constructeur de recopie par défaut
  Composite(const Composite&) = default;
  //! \brief Destructeur
  virtual ~Composite();
  //! \brief Operateur d'affectation par défaut
  Composite& operator=(const Composite&) = default;

public:
  //! \brief Ajoute un sous-compte à sa liste
  void ajouterSousCompte(Compte& c);
  //! \brief Supprime un sous-compte de sa liste
  void supprimerSousCompte(Compte& c);
  //! \brief Renvoie le nombre de sous-comptes directs
  int getNbSousComptes() const;

  //! \brief Itérateur en profondeur
  //!
  //! Itérateur qui parcourt en profondeur l'arbre de tous les comptes descendants du Composite
  class deepIterator {

        short itActif = 0; //! \brief L'iterateur principal (0) ou iterateur de sous-compte (1) a la main
        std::list<Compte*>::iterator  itPrincipal; //! \brief Iterator standard sur la liste des sous-comptes
        Composite::deepIterator* itSousComptes = nullptr; //! \brief Iterator récursif, successivement sur chacun des sous-comptes virtuels
        friend class Composite;
        //! \brief Constructeur
        //! Construit un itérateur en initialisant son itérateur principal avec celui donné
        deepIterator(std::list<Compte*>::iterator _it): itPrincipal(_it) {}

    public:
        //! \brief Contructeur par défaut
        deepIterator() = default;
        //! \brief Constructeur de recopie
        deepIterator(const deepIterator & i):itActif(i.itActif), itPrincipal(i.itPrincipal){
            if(i.itSousComptes!= nullptr) itSousComptes = new Composite::deepIterator((*i.itSousComptes));
        }
        //! \brief Destructeur
        ~deepIterator() {if (itSousComptes!= nullptr) delete(itSousComptes);}
        //! \brief Incrémente (préfixé) l'itérateur à son descendant suivant
        deepIterator& operator++();
          //! \brief Incrémente (postfixé) l'itérateur à son descendant suivant
        deepIterator operator++(int);

        //! \brief Teste l'égalité de 2 itérateurs
        bool operator==(const deepIterator& _it) const;
        //! \brief Teste l'inégalité de 2 itérateurs
        bool operator!=(const deepIterator& _it) const;
        //! \brief Accède au compte pointé par l'itérateur
        Compte &operator*() const ;
        //! \brief Accède à l'adresse du compte pointé par l'itérateur
        Compte* operator->() const;
    };

    //! \brief Renvoie un deepIterator qui pointe sur le premier sous-compte
    deepIterator dbegin();
    //! \brief Renvoie un deepIterator qui pointe sur la fin de la liste de sous-comptes
    deepIterator dend();

    //! \class iterator
    //! \brief Iterateur sur les sous-comptes direct
    //!
    //! Hérite de l'iterator sur la liste des sous-comptes direct du Composite
    class  iterator : public std::list<Compte*>::iterator{
        friend class Composite;
        //! \brief Constructeur
        iterator(std::list<Compte*>::iterator it):std::list<Compte*>::iterator(it){};
    public:
       //! \brief Redéfinition de l'operateur *
        Compte& operator*() const{
            return *std::list<Compte*>::iterator::operator*();
        }
        //! \brief Redéfinition de l'opérateur ->
        Compte* operator->() const{
            return std::list<Compte*>::iterator::operator*();
        }
    };
    //! \brief Renvoie un iterator qui pointe sur le premier sous-compte
    iterator begin(){return iterator(this->listeSousComptes.begin());}

    //! \brief Renvoie un iterator qui pointe sur la fin de la liste des sous-comptes
    iterator end(){return iterator(this->listeSousComptes.end());}


};

#endif //COMPOSITE_H_INCLUDED
