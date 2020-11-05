#include <algorithm>
#include "Composite.h"
#include "CompteVirtuel.h"
#include "Compte.h"

/*!
 * Incrémente en suivant le parcourt en profondeur de l'arbre.
 * Parcourt récursivement tous les sous-comptes d'un Compte avant de passer à son frère.
 * \return Le deepIterator concerné
 */
Composite::deepIterator& Composite::deepIterator::operator++() {
	//Si l'itérateur principal a la main
	if (this->itActif==0){
		//Si ce compte est virtuel
		if ((*itPrincipal)->estVirtuel()==true && (dynamic_cast<CompteVirtuel*>(*itPrincipal))->getNbSousComptes()>0){
			//Création d'un itérateur sur ce compte

            this->itSousComptes = new Composite::deepIterator((dynamic_cast<CompteVirtuel*>(*itPrincipal))->begin());
			this->itActif = 1; //On donne la main à cet itérateur
		}
		else{ //Si le sous-compte est une feuille
			this->itPrincipal++;
		}
	}
	else{ //Si un iterateur d'un sous-compte a la main 
        ++(*itSousComptes);
		//Si cet iterateur arrive a sa fin
		if(*itSousComptes==dynamic_cast<CompteVirtuel*>(*itPrincipal)->end()){
		    delete(itSousComptes);
		    itSousComptes= nullptr;
			this->itActif=0; //On donne la main a l'iterateur principal
			this->itPrincipal++; //Sous-compte suivant
		}
	}
	return *this;
}

/*!
 * \return Référence du Compte pointé
 */
Compte& Composite::deepIterator::operator*() const {
	if (this->itActif==0) return **(this->itPrincipal);
	else return **(this->itSousComptes);
}

/*!
 * \return Adresse du Compte pointé
 */
Compte* Composite::deepIterator::operator->() const {
    if (this->itActif==0) return *(this->itPrincipal);
    else return &**(this->itSousComptes);
}

/*!
 * Appelle l'incrément préfixé
 * \return Le deepIterator avant l'incrément
 */
Composite::deepIterator Composite::deepIterator::operator++(int) {
	auto old = *this;
	++(*this);
	return old;
}

/*!
 * \param _it : deepIterator à comparer
 */
bool Composite::deepIterator::operator!=(const Composite::deepIterator& _it) const{
	if (this->itActif==0) return _it.itPrincipal!= this->itPrincipal;
	else return _it.itSousComptes != this->itSousComptes;
}

/*!
 * \param _it : deepIterator à comparer
 */
bool Composite::deepIterator::operator==(const Composite::deepIterator& _it) const{
    if (this->itActif==0) return _it.itPrincipal == this->itPrincipal;
    else return _it.itSousComptes == this->itSousComptes;
}
/*!
 * \return Le deepIterator
 */
Composite::deepIterator Composite::dbegin(){
    return Composite::deepIterator(this->listeSousComptes.begin());
}
/*!
 * \return Le deepIterator
 */
Composite::deepIterator Composite::dend(){
    return Composite::deepIterator(this->listeSousComptes.end());
}

/*! \brief Ajoute un sous-compte à sa liste
 */
void Composite::ajouterSousCompte(Compte& c) {
    listeSousComptes.push_back(&c);
}

/*!
 *  Ne détruit pas le sous-compte en question.
 */
void Composite::supprimerSousCompte(Compte& c) {

    list<Compte*>::iterator it = std::find(this->listeSousComptes.begin(), this->listeSousComptes.end(), &c);
    if(it ==  this->listeSousComptes.end()) throw("Ce compte n'existe pas.");
    else this->listeSousComptes.erase(it);
}

/*!
 * \brief Renvoie le nombre de sous-comptes directs
 */
int Composite::getNbSousComptes() const {
    return listeSousComptes.size();
}

/*!
 * Détruit tous les sous-comptes directs de sa liste.
 */
Composite::~Composite() {
    for (auto it = listeSousComptes.begin(); it != listeSousComptes.end(); it++){
        delete *it;
    }
}
