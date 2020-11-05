#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <map>
#include <QString>
#include <functional>
#include <QDomDocument>
#include <QFile>


/*!
* \file Sauvegarde.h
* \brief Déclaration de la classe SauvegardeXML
*/

class Compte;

/*! \class SauvegardeXML
 *
* \brief Sauvegarde et restauration de l'application
*
* La classe SauvegardeXML implémente les fonctions de sauvegarde et de récupération
* des comptes et des transactions au format XML.
*
*/

class SauvegardeXML{
private:
    /*!
     * \brief Mapping comptes - identifiant
     */
    std::map<std::string, Compte*> comptes;

    //! \brief Chemin du fichier de configuration de l'application
    //!
    //! Permet de sauvegarder le contexte
    std::string cheminFichierConfig = "./config_poognon.xml";
public:
    //! Constructeur par défaut
    SauvegardeXML() = default;
    //! Destructeur par défaut
    ~SauvegardeXML() = default;
    //!Supression de la surcharge de l'opérateur d'affectation
    SauvegardeXML& operator=(const SauvegardeXML&) = delete ;

    /*!
     * \brief Sauvegarde dans un fichier
     *
     * Exporte l'ensemble des comptes et des transactions de l'application au format XML dans le fichier passé en paramètre
     * \param filename : chemin d'accès au fichier de sauvegarde
     */
    void saveToXML(const std::string& filename);

    /*!
     * \brief Charge un fichier
     *
     * Crée la hiérarchie des comptes et des transactions depuis un fichier XML.
     * \param filename : chemin d'accès au fichier de sauvegarde
     */
    void loadFromXML(const std::string filename);

    /*!
     * \brief Sauvegarde les Transaction
     *
     * Ajoute au noeud passé en paramètre l'ensemble des transactions.
     *
     * Sauvegarde la date, la référence, le memo et le rappochement,
     * ainsi que les Triplets associés en noeuds fils.
     *
     * \param doc : document XML
     * \param root : noeud XML auquel ajouter les transactions
     */
    void saveTransactionToXML(QDomDocument& doc, QDomNode& root);

    /*!
     * \brief Sauvegarde les Compte
     *
     * Ajoute au noeud passé en paramètre l'ensemble des comptes.
     *
     * Sauvegarde l'identifiant, le nom, le parent, le type, le solde au dernier rapprochement et la date,
     * ainsi que la virtualité du compte.
     *
     * \param doc : document XML
     * \param root : noeud XML auquel ajouter les comptes
     */
    void saveCompteToXML(QDomDocument& doc, QDomNode& root);

    /*!
     * \brief Charge les Transaction
     *
     * Lit le fichier XML et ajoute les transactions au TransactionManager
     * \param xmlDoc : document XML à lire
     */
    void loadTransactionFromXML(QDomDocument& xmlDoc);

    /*!
     * \brief Charge les Compte
     *
     * Lit le fichier XML et créer l'arborescence des comptes par appel au CompteFactory.
     * \param xmlDoc : document XML à lire
     */
    void loadCompteFromXML(QDomDocument& xmlDoc);

    /*!
     * \brief Sauvegarde le contexte
     *
     * Sauvegarde le contexte de l'application, notamment le chemin d'accès au fichier en cours de lecture
     * dans le fichier défini en attribut de classe.
     */
    void sauvegarderContexte();

    /*!
     * \brief Restaure le contexte
     *
     * Récupère le chemin d'accès du dernier fichier en lecture dans le fichier défini en attribut de classe
     */
    void restaurerContexte();
};

#endif // SAUVEGARDE_H
