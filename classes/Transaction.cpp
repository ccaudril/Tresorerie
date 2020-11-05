#include <list>
#include <functional>
#include <math.h>
#include "Transaction.h"
#include "CompteFeuille.h"

TripletTransaction::TripletTransaction(CompteFeuille* _compte, float _debit, float _credit): compte(_compte) {
    // Les montants de débit et crédit doivent être strictement positifs
    if(_debit<0){
        _credit-=_debit;
        _debit=0;
    }
    if(_credit<0){
        _debit-=_credit;
        _credit=0;
    }
    if(_debit!=0 && _credit!=0){
        int _min = min(_debit, _credit);
        _debit-=_min;
        _credit-=_min;
    }
    if((round(_debit*100)/100 != _debit) || (round(_credit*100)/100 != _credit)) throw("Les débits et crédits doivent être arrondis au centième.");
    this->debit=_debit;
    this->credit=_credit;

}

bool TripletTransaction::operator==(const TripletTransaction& triplet) const {
    return (this->compte==triplet.compte && this->debit==triplet.debit && this->credit==triplet.credit);
}

bool TripletTransaction::operator<(const TripletTransaction& triplet) const {
    return &(this->getCompte())<&(triplet.getCompte());
}

list<TripletTransaction>& Transaction::getTriplets(){
    return this->listeTriplets;
}

list<std::reference_wrapper<CompteFeuille>> Transaction::getComptes() {
    list<std::reference_wrapper<CompteFeuille>> listeCompte;
    for(list<TripletTransaction>::iterator it=this->listeTriplets.begin(); it!=this->listeTriplets.end(); it++){
        listeCompte.push_back((*it).getCompte());
    }
    return listeCompte;

}
void Transaction::modifierTransaction(const QDate& d, const string& m, const string& ref, const list<TripletTransaction>& triplets,  bool rapproche){
    if(!estEquilibree(triplets)) throw("Erreur transaction. Montants non équilibrés.");
    if(!comptesUniques(triplets)) throw("Erreur transaction. Comptes égaux.");
    for(list<TripletTransaction>::iterator it = this->getTriplets().begin(); it != this->getTriplets().end(); it++){
        if((*it).getDebit()!=0){
            (*it).getCompte().crediter((*it).getDebit());
        } else {
            (*it).getCompte().debiter((*it).getCredit());
        }
    }
    this->date=d;
    this->memo=m;
    this->reference=ref;
    this->listeTriplets=triplets;
    this->rapproche=rapproche;

    for(list<TripletTransaction>::iterator it = this->getTriplets().begin(); it != this->getTriplets().end(); it++){
        if((*it).getDebit()!=0){
            (*it).getCompte().debiter((*it).getDebit());
        } else {
            (*it).getCompte().crediter((*it).getCredit());
        }
    }
}

bool Transaction::operator==(const Transaction& transac){
    return (this->reference==transac.reference && this->memo==transac.memo && this->date==transac.date && this->listeTriplets==transac.listeTriplets);
}

bool Transaction::possedeCompte(CompteFeuille& c) {
    for(list<TripletTransaction>::iterator it = this->listeTriplets.begin(); it != this->listeTriplets.end(); it++){
        if ((*it).getCompte()==c) return true;
    }
    return false;

}

bool estEquilibree(list<TripletTransaction> triplets) {
    float debitTotal = 0;
    float creditTotal = 0;
    for(auto it = triplets.begin(); it != triplets.end(); it++){
        debitTotal+=(*it).getDebit();
        creditTotal+=(*it).getCredit();
    }
    if(debitTotal==creditTotal) return 1;
    return 0;
}

bool comptesUniques(list<TripletTransaction> triplets) {
    triplets.sort();
    TripletTransaction* tripletPrecedent = nullptr;
    for(auto it=triplets.begin(); it!=triplets.end(); it++){
        if(tripletPrecedent) if(&tripletPrecedent->getCompte()==&((*it).getCompte())) return 0;
        tripletPrecedent = &(*it);
    }
    return 1;
}
