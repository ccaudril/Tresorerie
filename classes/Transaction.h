#ifndef TRANSACTION_H_INCLUDED
#define TRANSACTION_H_INCLUDED

#include <list>
#include <string>
#include <QDate>

/*!
 * \file Transaction.h
 * \brief Déclaration des classes TripletTransaction et Transaction et des fonctions estEquilibrée et comptesUniques
 */


using namespace std;
class CompteFeuille;

/*!
 * \class TripletTransaction
 *
 * \brief Triplets compte, débit, crédit
 *
 * Classe permettant la gestion des Triplets compte, débit et crédit associés aux transactions.
 */

class TripletTransaction {
    friend class TransactionManager;
    //! \brief Pointeur sur le CompteFeuille concerné par le TripletTransaction
    CompteFeuille* compte;
    //! \brief Montant du débit du TripletTransaction
    float debit;
    //! \brief Montant du crédit du TripletTransaction
    float credit;
public:
    /*!
     * \brief TripletTransaction
     *
     * Construit un triplet de transactions à partir d'un compte d'un débit et d'un crédit.
     * Equilibre automatiquement débit et crédit si les deux valeurs sont différentes de 0.
     * \param compte : Compte associé à la transaction
     * \param debit
     * \param credit
     */
    TripletTransaction(CompteFeuille* compte, float debit, float credit);

    //! \brief Destructeur par défault
    ~TripletTransaction() = default;

    //! \brief Opérateur d'affectation par défault
    TripletTransaction& operator=(const TripletTransaction&) = default;

    /*!
     * \brief Surcharge de l'opérateur de comparaison ==
     * \return True si les triplets sont égaux sinon False
     */
    bool operator==(const TripletTransaction&) const;

    /*!
     * \brief Surcharge de l'opérateur de comparaison <
     *
     * Compare les adresses des CompteFeuille associés au TripletTransaction.
     * \return True si l'adresse du CompteFeuille associé au TripletTransaction
     * passé en paramètre est supérieure
     */
    bool operator<(const TripletTransaction&) const;

    /*!
     * \brief Accesseur de l'attribut compte
     * \return référence sur le compte associé au triplet
     */
    CompteFeuille& getCompte() const { return *(this->compte); };

    /*!
     * \brief Accesseur de l'attribut debit
     * \return debit associé au triplet
     */
    float getDebit() const { return this->debit; };

    /*!
     * \brief Accesseur de l'attribut credit
     * \return credit associé au triplet
     */
    float getCredit() const { return this->credit; };

};

/*!
 * \class Transaction
 *
 * \brief Transactions de l'application
 *
 * Classe permettant la création des transactions qui seront ajoutés à la liste du TransactionManager.
 */

class Transaction {
    friend class TransactionManager;
    //! \brief date de la Transaction
    QDate date;
    //! \brief nom de la Transaction
    string memo;
    //! \brief reference de la Transaction
    string reference;
    //! \brief liste des triplets associés à la Transaction
    list<TripletTransaction> listeTriplets;
    //! \brief rapprochement ou non de la Transaction
    bool rapproche;

    /*!
     * \brief Constructeur de Transaction
     *
     * Construction un objet Transaction à partir des paramètres passé en arugment
     * \param _date : date de la Transaction
     * \param _memo : nom de la Transaction
     * \param _reference : reference de la Transaction
     * \param _triplets : liste des triplets associés à la Transaction
     * \param _rapproche : true = transaction rapprochée, false = transaction non rapprochée
     */
    Transaction(const QDate& _date, const string& _memo, const string& _reference, list<TripletTransaction> _triplets, bool _rapproche = 0) :
    date(_date), memo(_memo), reference(_reference), listeTriplets(_triplets), rapproche(_rapproche){};

public:
    //! \brief Destructeur par défaut
    ~Transaction() = default;

    //! \brief Constructeur de recopie par défaut
    Transaction(const Transaction &) = default;

    //! \brief Opérateur d'affectation par défaut
    Transaction &operator=(const Transaction &) = default;

    /*!
     * \brief Surcharge de l'opérateur de comparaison ==
     * \return vraie si les Transaction sont identiques
     */
    bool operator==(const Transaction&);

    /*!
     * \brief Accesseur de l'attribut memo
     * \return memo associé à la transaction
     */
    string getMemo() const { return this->memo; };

    /*!
     * \brief  Accesseur de l'attribut reference
     * \return reference associée à la transaction
     */
    string getReference() const { return this->reference; };

    /*!
     * \brief  Accesseur de l'attribut date
     * \return date associée à la transaction
     */
    QDate getDate() const { return this->date; }

    /*!
     * \brief  Accesseur de l'attribut triplets
     * \return référence sur la liste des triplets associés à la transaction
     */
    list<TripletTransaction>& getTriplets();

    /*!
     * \brief CompteFeuille associés à la Transaction
     * \return liste de référence sur les CompteFeuille associés à la transaction
     */
    list<std::reference_wrapper<CompteFeuille>> getComptes();

    /*!
     * \brief Accesseur de l'attribut rapproche
     * \return Renvoie vrai si la transaction est rapprochée
     */
    bool getRapproche() const { return this->rapproche; };

    //! \brief Setter de l'attribut rapproche
    void setRapproche() { this->rapproche=true; };

    //! \brief Setter de l'attribut memo
    void setMemo(const string& m){ this->memo=m; };

    //! \brief Setter de l'attribut reference
    void setReference(const string& ref) { this->reference=ref; };

    /*!
     * \brief Modifie la Transaction
     *
     * Met à jour les attributs de l'objet transaction sur lequel la méthode est appelée.

     * \param date : nouvelle date
     * \param memo : nouveau memo
     * \param reference : nouvelle reference
     * \param triplets : nouvelle liste de triplets
     * \param rapproche : rapproché ou non
     */
    void modifierTransaction(const QDate& date, const string& memo, const string& reference, const list<TripletTransaction>& triplets, bool rapproche = 0);
   
    //! \brief Renseigne sur le fait que la Transaction concerne un CompteFeuille ou non
    //! \param CompteFeuille examiné
    bool possedeCompte(CompteFeuille& c);
};

//! \brief Renseigne sur l'équilibre d'une liste de TripletTransaction
//!
//! Renvoie True si le solde total des débits et crédits vaut 0
//! \param liste : liste à examiner
bool estEquilibree(list<TripletTransaction>);
//! \brief Renseigne sur l'unicité des CompteFeuille d'une liste de TripletTransaction
//!
//! Renvoie True si il n'existe pas 2 fois le même CompteFeuille dans la liste de TripletTransaction
//! \param liste : liste à examiner
bool comptesUniques(list<TripletTransaction>);

#endif //TRANSACTION_H_INCLUDED
