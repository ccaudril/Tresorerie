#include "Operations.h"
#include "Composite.h"
#include "Compte.h"
#include "CompteFactory.h"
#include "CompteRacine.h"
#include "CompteVirtuel.h"
#include "Transaction.h"
#include "TransactionManager.h"
#include "CompteFeuille.h"
#include "Excedent_Deficit.h"

#include<iostream>
#include<string>
#include<QTextDocument>
#include<QPrinter>
#include<QPainter>


void Operations::cloture_livre(){
    list<TripletTransaction>* listD = new list<TripletTransaction>;
    list<TripletTransaction>* listR = new list<TripletTransaction>;
    CompteRacine& racine = CompteRacine::getCompteRacine();
    float totalD=0, totalR=0;
    TransactionManager& tm = TransactionManager::getTransactionManager();

    for (auto it=racine.dbegin(); it!=racine.dend(); it++){
        if((*it).getType()==Type::D && (*it).estVirtuel()==false)
        {
            listD->push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>((*it)),0,(*it).getSolde()));
            totalD+=(*it).getSolde();
        }
        else if((*it).getType()==Type::R && (*it).estVirtuel()==false)
        {
            listR->push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>((*it)),(*it).getSolde(),0));
            totalR+=(*it).getSolde();
        }
    }
    Resultat& res = Resultat::getResultat();
    listD->push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(res),totalD,0));
    listR->push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(res),0,totalR));
    tm.ajouterTransaction(QDate(2021,01,01),"Cloture 2020","CL2020D",*listD);
    tm.ajouterTransaction(QDate(2021,01,01),"Cloture 2020","CL2020R",*listR);
    if(res.getSolde()>0)
    {
        list<TripletTransaction> listE;
        Excedent& exc = Excedent::getExcedent();
        listE.push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(res),res.getSolde(),0));
        listE.push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(exc),0,res.getSolde()));
        tm.ajouterTransaction(QDate(2021,01,01),"Cloture 2020","CL2020E",listE);
    }
    else if(res.getSolde()<0)
    {
        list<TripletTransaction> listE;
        Deficit& def = Deficit::getDeficit();
        listE.push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(res),0,-res.getSolde()));
        listE.push_back(TripletTransaction(&dynamic_cast<CompteFeuille&>(def),-res.getSolde(),0));
        tm.ajouterTransaction(QDate(2021,01,01),"Cloture 2020","CL2020E",listE);
    }


    //return;

}

bool Operations::rapprocher(CompteFeuille* c)
{
    TransactionManager& tm = TransactionManager::getTransactionManager();
    bool besoinRap = false;
    for (auto it=tm.begin(); it!=tm.end(); ++it)
    {
        if((*it).getRapproche()==false && (*it).possedeCompte(*c)==true)
        {
            (*it).setRapproche();
            if(besoinRap==false) besoinRap=true;
        }
    }
    if(static_cast<int>(c->getSoldeRap()*100)!=static_cast<int>(c->getSolde()*100)){
        besoinRap=true;
     }
    if(besoinRap==true)
    {
        c->setSoldeRap(c->getSolde());
        c->setDateRap(QDate::currentDate());
    }
    return besoinRap;

}

void Operations::exportBilanToPDF(const std::string& filename){

    // Script de l'affichage des comptes
    QString s00=" ",s01=" ",s10=" ",s11=" ";
    float totalA=0, totalP=0;
    CompteRacine& racine = CompteRacine::getCompteRacine();
    for (auto it=racine.dbegin() ; it!=racine.dend(); it++){
        if((*it).getType()==Type::A){
            // indentation
            int i;
            s00 += "<pre style=\"font-family: sans-serif\">";
            s01 += "<pre style=\"font-family: sans-serif\">";
            for(i=0;i<(*it).getProfondeur();i++){
                s00 += "    ";
            }

            // case "noms des comptes"
            s00 += "A:";
            s00 += QString::fromStdString((*it).getNom());
            s00 += "</pre>";

            // case "soldes des comptes"
            s01 += QString::number( (*it).getSolde(),'f', 2 );
            s01 += "€ </pre>";

            // ajout au solde total pour les comptes feuilles
            if((*it).estVirtuel()==false){
                totalA+=(*it).getSolde();
            }
        }
        if((*it).getType()==Type::P){
            // indentation
            int i;
            s10 += "<pre style=\"font-family: sans-serif\">";
            s11 += "<pre style=\"font-family: sans-serif\">";
            for(i=0;i<(*it).getProfondeur();i++){
                s10 += "    ";
            }

            // case "noms des comptes"
            s10 += "P:";
            s10 += QString::fromStdString((*it).getNom());
            s10 += "</pre>";

            // case "soldes des comptes"
            s11 += QString::number( (*it).getSolde() ,'f', 2);
            s11 += "€ </pre>";

            // ajout au solde total pour les comptes feuilles
            if((*it).estVirtuel()==false){
                totalP+=(*it).getSolde();
            }
        }
    }

    s00+="Total Actifs";
    s01+=QString::number( totalA );
    s01+="€";
    s10+="Total Passifs";
    s11+=QString::number( totalP );
    s11+="€";
    QString s20="Bilan : Actifs - Passifs",s21=" ";
    s21 += QString::number( totalA-totalP, 'f', 2 ) + " €";

    // Construction du HTML
    QString html = "<h1 style=\"text-align: center\">Bilan actif/passif</h1> <br>";
    html+="<style type='text/css'> table{border-collapse: collapse;} td{border: 1px solid black;} </style>";
    html+="<table width='100%'> <tr><td>";
    html+=s00;
    html+="</td><td>";
    html+=s01;
    html+="</td></tr> <tr><td>";
    html+=s10;
    html+="</td><td>";
    html+=s11;
    html+="</td></tr> <tr style=\"font-weight: bold\"><td>";
    html+=s20;
    html+="</td><td>";
    html+=s21;
    html+="</td></tr> </table> ";

    // Mise en page du PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageSize(QPrinter::A4);
    printer.setPageMargins(15,15,15,15,QPrinter::Millimeter);
    printer.setFullPage(false);

    // Construction du PDF
    QTextDocument doc;
    doc.setHtml(html);
    doc.adjustSize();
    doc.print(&printer);
}

void Operations::exportReleveToPDF(const std::string& filename){

    // Script de l'affichage des comptes
    QString t00=" ",t01=" ",t10=" ",t11=" ";
    float totalR=0, totalD=0;
    CompteRacine& racine = CompteRacine::getCompteRacine();
    for (auto it=racine.dbegin(); it!=racine.dend(); it++){
        if((*it).getType()==Type::R){
            // indentation
            int i;
            t00 += "<pre style=\"font-family: sans-serif\">";
            t01 += "<pre style=\"font-family: sans-serif\">";
            for(i=0;i<(*it).getProfondeur();i++){
                t00 += "    ";
            }

            // case "noms des comptes"
            t00 += "R:";
            t00 += QString::fromStdString((*it).getNom());
            t00 += "</pre>";

            // case "soldes des comptes"
            t01 += QString::number( (*it).getSolde(), 'f', 2 );
            t01 += "€ </pre>";

            // ajout au solde total pour les comptes feuilles
            if((*it).estVirtuel()==false){
                totalR+=(*it).getSolde();
            }
        }
        if((*it).getType()==Type::D){
            // indentation
            int i;
            t10 += "<pre style=\"font-family: sans-serif\">";
            t11 += "<pre style=\"font-family: sans-serif\">";
            for(i=0;i<(*it).getProfondeur();i++){
                t10 += "    ";
            }

            // case "noms des comptes"
            t10 += "D:";
            t10 += QString::fromStdString((*it).getNom());
            t10 += "</pre>";

            // case "soldes des comptes"
            t11 += QString::number( (*it).getSolde(), 'f', 2 );
            t11 += "€ </pre>";

            // ajout au solde total pour les comptes feuilles
            if((*it).estVirtuel()==false){
                totalD+=(*it).getSolde();
            }
        }
    }
    t00+="Total Recettes";
    t01+=QString::number( totalR );
    t01+="€";
    t10+="Total Depenses";
    t11+=QString::number( totalD );
    t11+="€";
    QString t20="Résultat : Recettes - Depenses",t21=" ";
    t21 += QString::number( totalR-totalD, 'f', 2)+" €";


    // Construction du HTML
    QString html = "<h1 style=\"text-align: center\">Compte de Résultat</h1> <br>";
    html+="<style type='text/css'> table{border-collapse: collapse;} td{border: 1px solid black;} </style>";
    html+="<table width='100%'> <tr><td>";
    html+=t00;
    html+="</td><td>";
    html+=t01;
    html+="</td></tr> <tr><td>";
    html+=t10;
    html+="</td><td>";
    html+=t11;
    html+="</td></tr> <tr style=\"font-weight: bold\"><td>";
    html+=t20;
    html+="</td><td>";
    html+=t21;
    html+="</td></tr> </table> ";

    // Mise en page du PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageSize(QPrinter::A4);
    printer.setPageMargins(15,15,15,15,QPrinter::Millimeter);
    printer.setFullPage(false);

    // Construction du PDF
    QTextDocument doc;
    doc.setHtml(html);
    doc.adjustSize();
    doc.print(&printer);
}

void Operations::exportResultatToPDF(const std::string& filename){

    // Script de l'affichage des comptes
    float totalR=0, totalD=0;
    CompteRacine& racine = CompteRacine::getCompteRacine();
    for (auto it=racine.dbegin(); it!=racine.dend(); it++){
        if((*it).getType()==Type::R){
            if((*it).estVirtuel()==false){
                totalR+=(*it).getSolde();
            }
        }
        if((*it).getType()==Type::D){
            if((*it).estVirtuel()==false){
                totalD+=(*it).getSolde();
            }
        }
    }
    QString resultat = QString::number( totalR-totalD, 'f', 2 )+ " €";


    // Construction du HTML
    QString html = "<h1 style=\"text-align: center\">Résultat</h1> <br>";
    html+="<style type='text/css'> table{border-collapse: collapse;} td{border: 1px solid black;} </style>";
    html+="<table width='100%'> <tr><td>";
    html+="Résultat : Recettes - Dépenses";
    html+="</td><td>";
    html+=resultat;
    html+="</td></tr> </table> ";

    // Mise en page du PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageSize(QPrinter::A4);
    printer.setPageMargins(15,15,15,15,QPrinter::Millimeter);
    printer.setFullPage(false);

    // Construction du PDF
    QTextDocument doc;
    doc.setHtml(html);
    doc.adjustSize();
    doc.print(&printer);

}
