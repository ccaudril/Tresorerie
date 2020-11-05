#ifndef OPERATIONS_H
#define OPERATIONS_H

#include<iostream>
#include<string>

/*!
 * \file Operations.h
 * \brief Déclaration de la classe Operations.
 */

class CompteFeuille;
class CompteVirtuel;

/*! \class Operations
   * \brief Classe contenant les différentes opérations applicables sur les comptes
   */

class Operations
{
public:
    /*!
        * \brief Constructeur de Operations
        */
    Operations()=default;

    /*!
       * \brief Méthode permettant d'effectuer la clôture du livre
       */

    static void cloture_livre();

    /*!
       * \brief Méthode permettant de rapprocher les transactions d'un compte
       *
       * \param pointeur sur un CompteFeuille
       */

    static bool rapprocher(CompteFeuille*);

    /*!
       * \brief Destructeur par défaut
       */

    ~Operations()=default;

    /*!
       * \brief Export du Bilan comptable au format PDF
       *
       * \param filename : nom du fichier à exporter
       */
    void exportBilanToPDF(const std::string& filename);

    /*!
       * \brief Export des comptes de résultat au format PDF
       *
       * \param filename : nom du fichier à exporter
       */
    void exportResultatToPDF(const std::string& filename);

    /*!
       * \brief Export du relevé de comptes au format PDF
       *
       * \param filename : nom du fichier à exporter
       */
    void exportReleveToPDF(const std::string& filename);

};

#endif // OPERATIONS_H
