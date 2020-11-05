#include "Sauvegarde.h"
#include "TransactionManager.h"
#include "CompteRacine.h"
#include "CompteFeuille.h"
#include "Compte.h"
#include "CompteFactory.h"
#include "CompteVirtuel.h"

#include <QDomDocument>
#include <QDomElement>
#include <QWidget>
#include <QFile>
#include <QTextStream>


void SauvegardeXML::saveToXML(const std::string &filename){
    QDomDocument doc;
    QDomElement root = doc.createElement("Sauvegarde");
    doc.appendChild(root);
    this->saveCompteToXML(doc, root);
    this->saveTransactionToXML(doc, root);
    QFile outfile(QString::fromStdString(filename));
    if (!outfile.open(QIODevice::ReadWrite))
                return;
    QTextStream stream(&outfile);
    stream << doc.toString();
    outfile.close();
}


void SauvegardeXML::loadFromXML(const std::string filename){
    QFile file(QString::fromStdString(filename));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            throw("Erreur lecture de fichier");
    }

    QDomDocument xmlDoc;
    xmlDoc.setContent(&file);
    this->loadCompteFromXML(xmlDoc);
    this->loadTransactionFromXML(xmlDoc);
    file.close();
}


void SauvegardeXML::saveTransactionToXML(QDomDocument& doc, QDomNode& root){
        QDomElement localroot = doc.createElement("Transactions");
        root.appendChild(localroot);

        TransactionManager& tm = TransactionManager::getTransactionManager();
        for(auto it = tm.begin(); it!= tm.end(); ++it){
            QDomElement transacElement = doc.createElement("Transaction");
            transacElement.setAttribute("date", (*it).getDate().toString("yyyy.MM.dd"));
            transacElement.setAttribute("reference", QString::fromStdString((*it).getReference()));
            transacElement.setAttribute("memo", QString::fromStdString((*it).getMemo()));
            transacElement.setAttribute("rapproche", QString::number((*it).getRapproche()));
            for(auto itt = (*it).getTriplets().begin(); itt!=(*it).getTriplets().end(); itt++){
                QDomElement tripletElement = doc.createElement("Triplet");
                tripletElement.setAttribute("compte", QString::fromStdString((*itt).getCompte().getIdentifiant()));
                tripletElement.setAttribute("credit", QString::number((*itt).getCredit()));
                tripletElement.setAttribute("debit", QString::number((*itt).getDebit()));

                transacElement.appendChild(tripletElement);
            }
            localroot.appendChild(transacElement);
        }
}


void SauvegardeXML::loadTransactionFromXML(QDomDocument& xmlDoc){
    QDomNodeList transactions = xmlDoc.elementsByTagName("Transaction");

    TransactionManager& tm = TransactionManager::getTransactionManager();

    for(int i=0; i<transactions.size(); i++){
        QDomElement transac = transactions.item(i).toElement();
        QDate date = QDate::fromString(transac.attribute("date", ""), "yyyy.MM.dd");
        std::string reference = transac.attribute("reference", "").toStdString();
        std::string memo = transac.attribute("memo", "").toStdString();
        bool rapproche = transac.attribute("rapproche", "").toInt();
        list<TripletTransaction> listeTriplets;
        auto it = transac.firstChild();
        while(!it.isNull()){
            QDomElement triplet = it.toElement();
            CompteFeuille* cf = dynamic_cast<CompteFeuille*>(this->comptes[triplet.attribute("compte", "").toStdString()]);
            listeTriplets.push_back(TripletTransaction(cf, triplet.attribute("debit", "").toFloat(), triplet.attribute("credit", "").toFloat()));
            it=it.nextSibling();
        }
        tm.ajouterTransaction(date, memo, reference, listeTriplets, rapproche);
    }
}


void SauvegardeXML::saveCompteToXML(QDomDocument& doc, QDomNode& root){
    QDomElement localroot = doc.createElement("Comptes");
    root.appendChild(localroot);

    CompteRacine& cr = CompteRacine::getCompteRacine();
    for(auto it = cr.dbegin(); it!= cr.dend(); it++){
        QDomElement compteElement = doc.createElement("Compte");
        compteElement.setAttribute("identifiant", QString::fromStdString((*it).getIdentifiant()));
        compteElement.setAttribute("nom", QString::fromStdString((*it).getNom()));
        compteElement.setAttribute("solde_rap", QString::number((*it).getSoldeRap()));
        compteElement.setAttribute("date_rap", (*it).getDateRap().toString("yyyy.MM.dd"));

        switch ((*it).getType()) {
        case Type::A:
            compteElement.setAttribute("type", QString('A'));
            break;
        case Type::D:
            compteElement.setAttribute("type", QString('D'));
            break;
        case Type::P:
            compteElement.setAttribute("type", QString('P'));
            break;
        case Type::R:
            compteElement.setAttribute("type", QString('R'));
            break;
        }
        if(Compte* c = dynamic_cast<Compte*>((*it).getParent())){
            compteElement.setAttribute("parent", QString::fromStdString(c->getIdentifiant()));
        } else {
            compteElement.setAttribute("parent", QString("racine"));
        }
        if((*it).estVirtuel()){
            compteElement.setAttribute("virtuel", QString("true"));
        }else{
            compteElement.setAttribute("virtuel", QString("false"));
        }
        localroot.appendChild(compteElement);
    }
}


void SauvegardeXML::loadCompteFromXML(QDomDocument& xmlDoc){
    QDomNodeList comptes = xmlDoc.elementsByTagName("Compte");

    CompteFactory factory = CompteFactory();
    for(int i=0; i<comptes.size(); i++){
        QDomElement cpt = comptes.item(i).toElement();
        string nom = cpt.attribute("nom", "").toStdString();
        float solde_rap = cpt.attribute("solde_rap", "").toFloat();
        QDate date_rap = QDate::fromString(cpt.attribute("date_rap", ""), "yyyy.MM.dd");

        Type typeCompte;
        if(cpt.attribute("type", "")==QString('A')){
            typeCompte=Type::A;
        } else if(cpt.attribute("type", "")==QString('P')){
            typeCompte=Type::P;
        } else if(cpt.attribute("type", "")==QString('D')){
            typeCompte=Type::D;
        } else {
            typeCompte=Type::R;
        }
        if(cpt.attribute("parent", "").toStdString()=="racine"){
            Compte& c = factory.creerCompte(cpt.attribute("identifiant", "").toStdString(), typeCompte, nom, solde_rap, date_rap, cpt.attribute("virtuel", "")=="true");
            this->comptes[c.getIdentifiant()] = &c;
        }
        else {
            Compte& c = factory.creerCompte(cpt.attribute("identifiant", "").toStdString(), *(dynamic_cast<CompteVirtuel*>(this->comptes[cpt.attribute("parent", "").toStdString()])) , nom, solde_rap, date_rap, cpt.attribute("virtuel", "")=="true");
            this->comptes[c.getIdentifiant()] = &c;
        }
    }
}


void SauvegardeXML::sauvegarderContexte(){
    QDomDocument doc;
    QDomElement root = doc.createElement("Configuration");
    doc.appendChild(root);
    QDomElement fichierActuel = doc.createElement("Fichier_compte");
    root.appendChild(fichierActuel);
    fichierActuel.setAttribute("chemin", QString::fromStdString(CompteRacine::getCompteRacine().getCheminFichier()));
    QFile outfile(QString::fromStdString(this->cheminFichierConfig));
    if (!outfile.open(QIODevice::ReadWrite))
        return;
    QTextStream stream(&outfile);
    stream << doc.toString();
    outfile.close();
}

void SauvegardeXML::restaurerContexte(){
    QFile file(QString::fromStdString(this->cheminFichierConfig));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::cerr<<"Erreur lecture de fichier";
    }

    QDomDocument xmlDoc;
    xmlDoc.setContent(&file);
    QDomNodeList config = xmlDoc.elementsByTagName("Fichier_compte");
    QDomElement fichier = config.item(0).toElement();
    std::string xmlFile = fichier.attribute("chemin", "").toStdString();
    if(xmlFile!=""){
        try {
            this->loadFromXML(xmlFile);
            CompteRacine::getCompteRacine().setCheminFichier(xmlFile);
        } catch (const char* e) {
            std::cerr<<e;
        }

    }

    file.close();
}
