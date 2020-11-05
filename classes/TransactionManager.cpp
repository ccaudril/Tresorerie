#include <iostream>
#include "TransactionManager.h"
#include "Transaction.h"
#include "CompteFeuille.h"

#include <string>
#include <QDate>

TransactionManager::Handler TransactionManager::handler=TransactionManager::Handler();

TransactionManager& TransactionManager::getTransactionManager(){
    if (handler.instance==nullptr) handler.instance=new TransactionManager;
    return *handler.instance;
}

void TransactionManager::libererTransactionManager(){
    delete handler.instance;
    handler.instance=nullptr;
}


TransactionManager::Iterator& TransactionManager::Iterator::operator++() {
    this->courant++;
    return *this;
}

Transaction& TransactionManager::Iterator::operator*() const {
    return *(this->courant);
}


TransactionManager::ConstIterator& TransactionManager::ConstIterator::operator++() {
    this->courant++;
    return *this;
}


const Transaction& TransactionManager::ConstIterator::operator*() const{
    return *(this->courant);
}

TransactionManager::FilterIterator& TransactionManager::FilterIterator::operator++(int){
    // Tant que le compte concerné ne se trouve pas dans un des triplets de la transaction on incrémente
    this->courant++;
    while(*this!=TransactionManager::getTransactionManager().fend(this->compte) && !(this->courant->possedeCompte(this->compte))){
        this->courant++;
    }
    return *this;
}


Transaction& TransactionManager::FilterIterator::operator*() const{
    return *(this->courant);
}


TransactionManager::FilterIterator TransactionManager::fbegin(CompteFeuille& compte) {
    // Return first transaction with compte in parameters
    list<Transaction>::iterator it;
    for (it = this->listeTransactions.begin(); it != this->listeTransactions.end(); it++) {
        if((*it).possedeCompte(compte)){
            break;
        }
    }
    return TransactionManager::FilterIterator(compte, it);
}


bool CompareTransaction(Transaction& t1, Transaction& t2) {
    return t1.getDate() < t2.getDate();
};


void TransactionManager::ajouterTransaction(const QDate& date, const string& memo, const string& ref, list<TripletTransaction>& triplets, bool rapproche){
    for(list<TripletTransaction>::iterator it = triplets.begin(); it != triplets.end(); it++){
        if((*it).getDebit()!=0){
            (*it).getCompte().debiter((*it).getDebit());
        } else {
            (*it).getCompte().crediter((*it).getCredit());
        }
    }
    this->listeTransactions.push_back(Transaction(date, memo, ref, triplets, rapproche));
    //Trie liste par date
    this->listeTransactions.sort(CompareTransaction);
}

void TransactionManager::creerTransaction(const QDate& date, const string& memo, const string& ref, list<TripletTransaction>& triplets) {
    if(!estEquilibree(triplets)) throw("Erreur transaction. Montants non équilibrés.");
    if(!comptesUniques(triplets)) throw("Erreur transaction. Comptes égaux.");
    for(list<TripletTransaction>::iterator it = triplets.begin(); it != triplets.end(); it++){
        if(date<(*it).compte->getDateRap() && (*it).compte->getDateRap()!=QDate::fromString("")) throw("Impossible d'insérer une transaction inférieure à la date de dernier rapprochement d'un de ses comptes");
    }
    this->ajouterTransaction(date, memo, ref, triplets);

}


void TransactionManager::supprimerTransaction(Transaction& transac){
    auto suppression = find(this->listeTransactions.begin(), this->listeTransactions.end(), transac);
    if (suppression != this->listeTransactions.end()) {
        if(suppression->getRapproche()) throw ("Une transaction rapprochée ne peut pas être supprimée");
        for (list<TripletTransaction>::iterator it = transac.listeTriplets.begin(); it != transac.listeTriplets.end(); it++) {
            if ((*it).getDebit() != 0) {
                (*it).getCompte().crediter((*it).getDebit());
            } else {
                (*it).getCompte().debiter((*it).getCredit());
            }
        }
        this->listeTransactions.erase(suppression);
    } else {
        throw ("Erreur, la transaction à supprimer n'existe pas");
    }
}
