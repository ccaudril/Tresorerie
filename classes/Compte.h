#ifndef COMPTE_H_INCLUDED
#define COMPTE_H_INCLUDED

/*!
 * \file Compte.h
 * \brief Déclaration de la classe Compte.
 */

#include <string>
#include <QDate>
#include <algorithm>
#include "Operations.h"

class Composite;
class CompteVirtuel;
class CompteRacine;
class StrategyAD;
class StrategyPR;
class CompteFeuille;

using namespace std;

/**
 * \enum Type
 * \brief Types de comptes.
 *
 * Type est une série de constantes prédéfinies représentant
 * les différents types de comptes.
 */

enum class Type {
  A, /*!< Compte d'actifs */
  P, /*!< Compte de passifs */
  D, /*!< Compte de dépenses */
  R /*!< Compte de recettes */
};

/*! \class Compte
   * \brief Classe abstraite représentant un Compte.
   *
   * La classe Compte regroupe les attributs et méthodes communs
   * à tous les Compte, excepté le CompteRacine.
   */

class Compte {
  std::string identifiant; /*!< Identifiant du Compte*/
  string nom; /*!< Nom du Compte*/
  float solde; /*!< Solde du Compte*/
  float soldeRap; /*!< Solde du Compte lors du dernier rapprochement*/
  Composite* parent; /*!< Pointeur vers le compte Composite parent dans la hiérarchie*/
  Type type; /*!< Type du Compte (A, P, D ou R)*/
  QDate dateRap; /*!< Date du dernier rapprochement*/

  friend class StrategyAD;
  friend class StrategyPR;
  friend class Operations;
  friend void rapprocher(CompteFeuille*);
  friend class CompteFactory;


public:
  /*!
     * \brief Constructeur 1
     *
     * Constructeur de la classe Compte permettant de creér un Compte directement
     * sous le CompteRacine dans la hiérarchie. Son solde est initialisé à 0
     * et son parent est un pointeur vers le CompteRacine.
     *
     * \param _identifiant : Identifiant du compte créé
     * \param _nom : Nom du Compte créé
     * \param _type : Type du Compte créé
     */
  Compte(const string& _identifiant, const string& _nom, const Type _type);

  /*!
     * \brief Constructeur 2
     *
     * Constructeur de la classe Compte permettant de créer un Compte sous un
     * CompteVirtuel. Son solde est initialisé à 0 et il prend le type de son parent.
     *
     * \param _identifiant : Identifiant du Compte créé
     * \param _nom : Nom du Compte créé
     * \param _parent : Pointeur vers le CompteVirtuel parent du Compte créé
     */

  Compte(const string& _identifiant, const string& _nom, CompteVirtuel* _parent);

  /*!
     * \brief Destructeur par défaut
     */

  virtual ~Compte() = default;

  /*!
     * \brief Constructeur de recopie
     *
     * Constructeur de recopie par défaut avec allocation dynamique.
     *
     * \param c : Référence sur le Compte recopié
     */

  Compte(const Compte& c);

  /*!
     * \brief Opérateur d'affectation
     *
     * Operateur d'affectation par défaut avec allocation dynamique et gestion
     * de l'auto-affectation.
     *
     * \param c : Référence sur le Compte recopie
     * \return Référence sur le Compte ré-affecté
     */

  Compte& operator=(const Compte& c);

  /*!
     * \brief Accesseur de l'attribut nom
     *
     * \return Attribut nom
     */

  string getNom() const {return nom;}

  /*!
     * \brief Accesseur de l'attribut identifiant
     *
     * \return Attribut identifiant
     */

  std::string getIdentifiant() const {return identifiant;}

  /*!
     * \brief Accesseur de l'attribut solde
     *
     * \return Attribut solde
     */

  float getSolde() const {return solde;}

  /*!
     * \brief Accesseur de l'attribut soldeRap
     *
     * \return Attribut soldeRap
     */

  float getSoldeRap() const {return soldeRap;}


  /*!
     * \brief Accesseur de l'attribut parent
     *
     * \return Attribut parent
     */

  Composite* getParent() const {return parent;}

  /*!
     * \brief Accesseur de l'attribut type
     *
     * \return Attribut type
     */

  Type getType() const {return type;}

  /*!
     * \brief Renseigne sur le fait qu'un Compte soit virtuel ou non.
     *
     * Méthode virtuelle pure.
     *
     */

  virtual bool estVirtuel() const = 0;

  /*!
     * \brief Suppression du Compte
     *
     * Methode virtuelle pure.
     *
     */

  virtual void supprimerCompte() = 0;

  /*!
     * \brief Accesseur de l'attribut dateRap
     *
     * \return Attribut dateRap
     */

  QDate getDateRap() const {return dateRap;}

  /*!
     * \brief Profondeur du Compte dans la hiérarchie des Compte (CompteRacine = 0)
     *
     * \return Profondeur du Compte
     */

  int getProfondeur() const;


private:

  /*!
     * \brief Mutateur privé de l'attribut solde
     *
     * \param s : Nouveau solde de l'objet
     */

  void setSolde(const float& s) {solde=s;}

  /*!
     * \brief Mutateur privé de l'attribut soldeRap
     *
     * \param s : Nouveau soldeRap de l'objet
     */

  void setSoldeRap(const float& s) {soldeRap=s;}

  /*!
     * \brief Mutateur privé de l'attribut dateRap
     *
     * \param s : Nouveau dateRap de l'objet
     */

  void setDateRap(QDate date) {dateRap=date;}

};

#endif //COMPTE_H_INCLUDED
