#ifndef TRANSFETMANAGER_H_INCLUDED
#define TRANSFETMANAGER_H_INCLUDED

#include <list>
#include <string>
#include <algorithm>
#include <QDate>

#include "Transaction.h"

/*!
 * \file TransactionManager.h
 * \brief Déclaration de la classe TransactionManager, de son Singleton et de ses itérateurs.
 */
class TripletTransaction;
class CompteFeuille;

using namespace std;

/*! \class TransactionManager
   * \brief Classe permettant la gestion des Transaction de l'application.
   *
   * La classe TransactionManager est un singleton qui permet d'indexer et de trier par date les Transaction.
   * Elle possède trois itérateurs : classique, constant et filtré par compte.
   */

class TransactionManager {
    //! \brief Liste des Transaction de l'application
    list<Transaction> listeTransactions;

    //! \class Handler
    //! \brief Singleton
    struct Handler{
        TransactionManager* instance;
        Handler():instance(nullptr){}
        ~Handler(){ delete instance; }
    };
    static Handler handler;
    //!\brief Constructeur par défaut
    TransactionManager()=default;
    //!\brief Destructeur par défaut
    ~TransactionManager()=default;

public :
    /*!
       * \brief Accesseur du singleton TransactionManager
       *
       * \return Singleton TransactionManager
       */
    static TransactionManager& getTransactionManager();
    /*!
       * \brief Ajout d'une transacton
       *
       * Permet l'ajout d'une transaction à la liste du TransactionManager.
       * Utilisée directement lors de la restauration via XML, évite les vérifications de dates.
       *
       * \param date : Date d'ajout de la transaction
       * \param memo : Memo de la transaction
       * \param ref : Reference de la transaction
       * \param triplets : Liste des triplets de comptes associés à la transaction
       * \param rapproche : Booléen à faux par défaut. Peut être vrai lors de la restauration via XML.
       */
    void ajouterTransaction(const QDate& date, const string& memo, const string& ref, list<TripletTransaction>& triplets, bool rapproche=false);
    /*!
       * \brief Création d'une Transaction
       *
       * Vérifie la validité d'une transaction et redirige vers la fonction d'ajout de transaction.
       * Permet de créer une transaction lors de son premier ajout.
       *
       * \param date : Date d'ajout de la transaction
       * \param memo : Memo de la transaction
       * \param ref : Reference de la transaction
       * \param triplets : Triplet des la transaction à ajoutée
       */
    void creerTransaction(const QDate& date, const string& memo, const string& ref, list<TripletTransaction>& triplets);
    /*!
       * \brief Suppression d'une Transaction
       *
       * Cherche et supprime la transaction de la liste. Renvoie une erreur si la transaction n'existe pas.
       *
       * \param transac : Transaction à supprimer
       */
    void supprimerTransaction(Transaction& transac);

    /*! \brief Libération du TransactionManager
     *
     *  Permet la libération de l'instant unique du TransactionManager.
     */
    static void libererTransactionManager();


    //! \brief Iterateur constant sur les transactions
    //!
    //! Itérateur constant qui parcourt toutes les transactions
    class ConstIterator{
        list<Transaction>::const_iterator courant;//! \brief Iterateur constant standard sur la liste des transactions
    public:
        //! \brief Constructeur
        //!
        //! Construit un itérateur en initialisant son itérateur principal avec celui donné
        ConstIterator(list<Transaction>::const_iterator c):courant(c){};
        //! \brief Constructeur de copie par défaut
        ConstIterator(const ConstIterator&)=default;
        //! \brief Constructeur par défaut
        ConstIterator()=default;
        //! \brief Destructeur par défaut
        ~ConstIterator()=default;
        //! \brief surcharge de l'operator++
        ConstIterator& operator++();
        /*!
           * \brief Surcharge de l'opérateur *
           *
           * Renvoie la transaction constante qui est le current item
           *
           * \return Transaction current item
           */
        const Transaction& operator*() const;
        //! \brief surcharge de l'opérateur ==
        bool operator==(const ConstIterator& it) const{ return it.courant==this->courant;};
        //! \brief surcharge de l'opérateur !=
        bool operator!=(const ConstIterator& it) const{ return it.courant!=this->courant;};
    };

    //! \brief Iterateur sur les transactions
    //!
    //! Itérateur qui parcourt toutes les transactions
    class Iterator {
        list<Transaction>::iterator courant;//! \brief Iterateur standard sur la liste des transactions
    public:
        //! \brief Constructeur
        //!
        //! Construit un itérateur en initialisant son itérateur principal avec celui donné
        Iterator(list<Transaction>::iterator d):courant(d){}
        //! \brief Constructeur par défaut
        Iterator()=default;
        //! \brief Constructeur de copie par défaut
        Iterator(const Iterator&)=default;
        //! \brief Destructeur par défaut
        ~Iterator()=default;
        //! \brief Surcharge de l'operator++
        Iterator& operator++();
        /*!
           * \brief Surcharge de l'opérateur *
           *
           * Donne la transaction qui est le current item
           *
           * \return Transaction current item
           */
        Transaction& operator*() const ;
        //! \brief surcharge de l'opérateur ==
        bool operator==(const Iterator& it) const{ return it.courant==this->courant;};
        //! \brief surcharge de l'opérateur !=
        bool operator!=(const Iterator& it) const{ return it.courant!=this->courant;};

    };

    //! \brief Iterateur filtrant les transactions d'un compte en particulier
    //!
    //! Itérateur qui parcourant toutes les transactions d'un compte particulier.
    class FilterIterator {
        CompteFeuille& compte;//! \brief Compte dont on veut parcourir les transactions
        list<Transaction>::iterator courant;//! \brief Iterateur standard sur la liste des transactions
    public:
        //! \brief Constructeur
        //! \param compte : Compte dont on veut filtrer les transactions
        //!
        FilterIterator(CompteFeuille& compte, list<Transaction>::iterator c):compte(compte),courant(c){};
        FilterIterator() = delete;
        //! \brief Constructeur de copie par défaut
        FilterIterator(const FilterIterator&)= default;
        //! \brief Destructeur par défaut
        ~FilterIterator()= default;
        //! \brief surcharge de l'operator++
        FilterIterator& operator++(int);
        /*!
           * \brief Surcharge de l'opérateur *
           *
           * Donne la transaction qui est le current item
           *
           * \return Transaction current item
           */
        Transaction& operator*() const ;
        //! \brief Surcharge de l'opérateur ==
        bool operator==(const FilterIterator& it) const{ return it.courant==this->courant;};
        //! \brief Surcharge de l'opérateur !=
        bool operator!=(const FilterIterator& it) const{ return it.courant!=this->courant;};

    };
    //! \brief Renvoie un itérateur qui pointe sur la premiere transaction
    Iterator begin(){ return Iterator(this->listeTransactions.begin()); }
    //! \brief Renvoie un itérateur qui pointe sur la derniere transaction
    Iterator end(){ return Iterator(this->listeTransactions.end()); }
    //! \brief Renvoie un itérateur constant qui pointe sur la premiere transaction
    ConstIterator cbegin(){ return ConstIterator(this->listeTransactions.cbegin()); }
    //! \brief Renvoie un itérateur constant qui pointe sur la derniere transaction
    ConstIterator cend(){ return ConstIterator(this->listeTransactions.cend()); }
    //! \brief Renvoie un FilterIterator qui pointe sur la premiere transaction
    FilterIterator fbegin(CompteFeuille& compte);
    //! \brief Renvoie un FilterIterator qui pointe sur la derniere transaction
    FilterIterator fend(CompteFeuille& compte){return FilterIterator(compte, this->listeTransactions.end()); }

};

#endif // TRANSFETMANAGER_H_INCLUDED
