#ifndef CREERCOMPTEDIALOG_H
#define CREERCOMPTEDIALOG_H

#include <QDialog>
class CompteFeuille;

/*!
 * \file creercomptedialog.h
 * \brief Déclaration de la classe creerCompteDialog.
 */

namespace Ui {
class creerCompteDialog;
}

/*! \class creerCompteDialog
 *  \brief Pop-up de création d'un  compte
 *
 *  Hérite de la classe QDialog
 * */
class creerCompteDialog : public QDialog
{
    Q_OBJECT

public:
    //! \brief Constructeur
    creerCompteDialog(QWidget *parent);
    //! \brief Destructeur
    ~creerCompteDialog();
private slots:
    //! \brief Slot : Rafraichit les types
    void updateType();
    //! \brief Slot : Ajoute un Compte
    void ajouterCompte();
    //! \brief Slot : Gère l'affichage des champs solde initial
    void affichageSoldeInitial();

private:
    //! \brief Interface graphique
    Ui::creerCompteDialog *ui;
    //! \brief Nouveau Compte passif pour solde initial
    CompteFeuille* creerNouveauPassif();
};

#endif // CREERCOMPTEDIALOG_H
