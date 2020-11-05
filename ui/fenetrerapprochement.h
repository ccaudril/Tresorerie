#ifndef FENETRERAPPROCHEMENT_H
#define FENETRERAPPROCHEMENT_H

#include <QDialog>

/*!
 * \file fenetrerapprochement.h
 * \brief Déclaration de la classe fenetreRapprochement.
 */

class CompteFeuille;

namespace Ui {
class fenetreRapprochement;
}

/*! \class fenetreRapprochement
 *  \brief Pop-up de rapprochement
 *
 *  Hérite de la classe QDialog
 * */
class fenetreRapprochement : public QDialog
{
    Q_OBJECT

public:
    //! \brief Constructeur
    fenetreRapprochement(QWidget *parent = nullptr);

    //! \brief Destructeur
    ~fenetreRapprochement();

public slots:
    //! \brief Rafraichit les comptes
    void refresh(int);
    //! \brief Appelle la fonction rapprochement
    void lancerRapprochement();

private:
    //! \brief Interface graphique
    Ui::fenetreRapprochement *ui;
};

#endif // FENETRERAPPROCHEMENT_H
