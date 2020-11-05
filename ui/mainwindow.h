#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDate>
#include <QVariant>

/*!
 * \file mainwindow.h
 * \brief Déclaration des classes MainWindow, CompteItem, TransactionItem et ChoixCompte
 */

class CompteRacine;
class Compte;
class Composite;
class CompteVirtuel;
class CompteFeuille;
class Transaction;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TransactionItem;
class fenetreRapprochement;

//! \class MainWindow
//! \brief Fenêtre principale de l'application
//!
//! La fenêtre est séparée en 3 parties :
//!
//!     - A gauche : La hiérarchie des Comptes
//!     - En haut à droite : La liste des transactions d'un Compte
//!     - En bas à droite : Une série de champs à remplir pour ajouter/modifier des transactions
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class fenetreRapprochement;
    MainWindow(QWidget *parent = nullptr);
    //! Destructeur
    ~MainWindow();
    //! Actualise la hiérarchie des Compte et leur solde
    void afficherListeCompte();

public slots:
    //! \brief Slot : Affiche les Transaction d'un Compte
    //!
    //! Connecté au signal : Double clic sur un Compte
    void afficherTransaction(Compte*);

    //! \brief Slot : Affiche la Transaction pour modification
    //!
    //! Connecté au signal : Double clic sur une Transaction
    void modifierTransaction(QTableWidgetItem*);

    //! \brief Slot : Supprime la Transaction sélectionnée
    //!
    //! Connecté au signal : Bouton Supprimer Transaction
    void supprimerTransaction();

    //! \brief Slot : Ouvre la fenêtre CreerCompte
    //!
    //! Connecté au signal : Bouton Ajouter Compte
    void ajouterCompte();

    //! \brief Slot : Supprime un Compte
    //!
    //! Connecté au signal : Bouton Supprimer Compte
    void supprimerCompte();

    //! \brief Slot : Ajoute la transaction renseignée
    //!
    //! Connecté au signal : Bouton Ajouter Transaction
    void ajouterTransaction();

    //! \brief Slot : Ajoute une ligne pour un TripletTransaction
    //!
    //! Connecté au signal : Bouton +
    void ajouterLigne();

    //! \brief Slot : Retire une ligne de TripletTransaction
    //!
    //! Connecté au signal : Bouton -
    void supprimerLigne();

    //! \brief Slot : Réinitialise les champs d'AjoutTransaction
    //!
    //! Connecté au signal : Bouton Effacer/Annuler
    void clearAjoutTransaction();

    //! \brief Slot : Ouvrir depuis un fichier XML
    //!
    //! Connecté au signal : Fichier->Ouvrir
    void loadFromXML();

    //! \brief Slot : Enregistrer l'ensemble des comptes et des transactions en XML
    //!
    //! Connecté au signal : Fichier->Sauvegarder
    void saveToXML();

    //! \brief Slot : Enregistrer l'ensemble des comptes et des transactions en PDF
    //!
    //! Connecté au signal : Rapports->Bilan comptable
    void saveBilanToPDF();

    //! \brief Slot : Enregistrer l'ensemble des comptes et des transactions en PDF
    //!
    //! Connecté au signal : Rapports->Résultat
    void saveResultatToPDF();

    //! \brief Slot : Enregistrer l'ensemble des comptes et des transactions en PDF
    //!
    //! Connecté au signal : Rapports->Relevé Recettes/Dépenses
    void saveReleveToPDF();

    //! \brief Slot : Permet d'ouvrir une nouvelle hiérarchie de compte en sauvegardant l'ancienne
    //!
    //! Connecté au signal : Fichier->Nouveau
    void nouveauFichier();

    //! \brief Slot : Quitte l'application en sauvegardant le contexte
    //!
    //! Connecté au signal : Fichier->Quitter
    void quitterApp();

    //! \brief Slot : Lance la cloture du livre
    //!
    //! Connecté au signal : Opérations->Clôture du livre
    void cloture();

    //! \brief Slot : Lance le rapprochement d'un compte
    //!
    //! Connecté au signal : Opérations->Rapprochement
    void rapprochement();

private:
    Ui::MainWindow *ui;
};

//! \class CompteItem
//! \brief Items contenus dans le QTreeWidget pour l'affichage des Compte
//!
//! Hérite de QTreeWidgetItem et y ajoute une référence sur le Compte associé.
class CompteItem : public QTreeWidgetItem{
    friend class MainWindow;
    //!Référence sur le Compte sélectionné
    Compte& compte;
    //! \brief Constructeur
    CompteItem(Compte& c);
    //! \brief Destructeur par défaut
    ~CompteItem()=default;
public:
    //! \brief Renvoie une référence sur le Compte sélectionné
    Compte& getCompte(){
        return this->compte;
    }
};

//! \class TransactionItem
//! \brief Items contenus dans le QTableWidget.
//!
//! Hérite de QTableWidgetItem et y ajoute une référence sur la Transaction associée.
class TransactionItem : public QTableWidgetItem {
    friend class MainWindow;
    //! \brief Référence sur la Transaction associée.
    Transaction& transaction;
    //! \brief Constructeur à partir d'une QString, d'une référence sur transaction et d'un éventuel alignement des cellules.
    TransactionItem(QString s, Transaction& t, QColor background, Qt::Alignment al=Qt::AlignLeft);

public:
    //! \brief Accesseur de l'attibut transaction
    Transaction& getTransaction(){
        return this->transaction;
    }
};

//! \class ChoixCompte
//! \brief Listes déroulantes de tous les CompteFeuille
//!
//! Hérite de QComboBox
class ChoixCompte : public QComboBox {
    Q_OBJECT
    friend class MainWindow;
    //! \brief Constructeur initialisant la liste de CompteFeuille
    ChoixCompte();
public slots:
    //! \brief Slot : Surcharge de showPopup() de QComboBox
    //!
    //! Actualise la liste de Comptes avant de l'afficher
    virtual void showPopup() override;
    //! \brief Slot : Actualise la liste de CompteFeuille
    void actualiser();
};

#endif // MAINWINDOW_H
