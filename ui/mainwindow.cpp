#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../classes/CompteRacine.h"
#include "../classes/Compte.h"
#include "../classes/Composite.h"
#include "../classes/CompteVirtuel.h"
#include "../classes/CompteFeuille.h"
#include "../classes/Operations.h"
#include "../classes/TransactionManager.h"
#include "../classes/Transaction.h"
#include "../classes/Sauvegarde.h"
#include "../classes/Operations.h"
#include "creercomptedialog.h"
#include "fenetrerapprochement.h"

#include "QSpinBox"
#include "QString"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>

Q_DECLARE_METATYPE(Compte*)
Q_DECLARE_METATYPE(Transaction*)

/*!
 * \brief Constructeur
 *
 * Set up l'interface graphique, affiche la liste de comptes.
 *
 * Restaure le contexte précédent.
 *
 * Connecte les signaux des boutons et menus.
 * \param parent : widget parent de la QMainWindow
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    setWindowTitle("POOgnon");

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ChoixCompte* cc1 = new ChoixCompte();
    ChoixCompte* cc2 = new ChoixCompte();
    ui->ajouterTripletTable->setCellWidget(0,0,cc1);
    ui->ajouterTripletTable->setCellWidget(1,0,cc2);
    this->showMaximized();
    this->ui->dateEdit->setCalendarPopup(true);

    // Connexion des slots et des signaux
    QObject::connect(this->ui->actionAjouter, &QAction::triggered, this, &MainWindow::ajouterCompte);
    QObject::connect(this->ui->ajouterCompte, &QPushButton::clicked, this, &MainWindow::ajouterCompte);
    QObject::connect(this->ui->supprCompte, &QPushButton::clicked, this, &MainWindow::supprimerCompte);

    QObject::connect(this->ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, [&](QTreeWidgetItem* item){this->afficherTransaction(&dynamic_cast<CompteItem*>(item)->getCompte());});
    QObject::connect(this->ui->tableWidget, &QTableWidget::itemDoubleClicked, cc1, &ChoixCompte::actualiser);
    QObject::connect(this->ui->tableWidget, &QTableWidget::itemDoubleClicked, cc2, &ChoixCompte::actualiser);
    QObject::connect(this->ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::modifierTransaction);
    QObject::connect(this->ui->suppTransac, &QPushButton::clicked, this, &MainWindow::supprimerTransaction);
    QObject::connect(this->ui->ajouterTransactionButton, &QPushButton::clicked, this, &MainWindow::ajouterTransaction);

    QObject::connect(this->ui->ajouterLigneButton, &QPushButton::clicked, this, &MainWindow::ajouterLigne);
    QObject::connect(this->ui->supprimerLigneButton, &QPushButton::clicked, this, &MainWindow::supprimerLigne);
    QObject::connect(this->ui->clearTransactionButton, &QPushButton::clicked, this, &MainWindow::clearAjoutTransaction);

    QObject::connect(this->ui->actionOuvrir, &QAction::triggered, this, &MainWindow::loadFromXML);
    QObject::connect(this->ui->actionSauvegarder, &QAction::triggered, this, &MainWindow::saveToXML);
    QObject::connect(this->ui->actionNouveau, &QAction::triggered, this, &MainWindow::nouveauFichier);
    QObject::connect(this->ui->actionQuitter, &QAction::triggered, this, &MainWindow::quitterApp);

    QObject::connect(this->ui->actionCloture, &QAction::triggered, this, &MainWindow::cloture);
    QObject::connect(this->ui->actionRapprochement, &QAction::triggered, this, &MainWindow::rapprochement);
    QObject::connect(this->ui->actionBilan, &QAction::triggered, this, &MainWindow::saveBilanToPDF);
    QObject::connect(this->ui->actionResultat, &QAction::triggered, this, &MainWindow::saveResultatToPDF);
    QObject::connect(this->ui->actionReleve, &QAction::triggered, this, &MainWindow::saveReleveToPDF);

    SauvegardeXML sv;
    sv.restaurerContexte();
}

void MainWindow::loadFromXML(){
    TransactionManager::getTransactionManager().libererTransactionManager();
    CompteRacine::getCompteRacine().libererCompteRacine();
    std::string filename = QFileDialog::getOpenFileName(nullptr, "save.xml").toStdString();
    SauvegardeXML sv;
    try {
        sv.loadFromXML(filename);
        CompteRacine::getCompteRacine().setCheminFichier(filename);
        this->afficherListeCompte();
    } catch (const char* e) {
        this->ui->statusbar->showMessage(e, 4000);
    }

}

void MainWindow::saveToXML(){
    std::string filename;
    if(CompteRacine::getCompteRacine().getCheminFichier()==""){
        filename = QFileDialog::getSaveFileName().toStdString();
        if(filename!=""){
            filename+=".xml";
            CompteRacine::getCompteRacine().setCheminFichier(filename);
        }else{
            this->ui->statusbar->showMessage("Action annulée", 4000);
            return;
        }
    } else {
        filename = CompteRacine::getCompteRacine().getCheminFichier();
    }
    SauvegardeXML sv;
    sv.saveToXML(filename);
    this->ui->statusbar->showMessage("Fichier correctement sauvegardé", 4000);
}

void MainWindow::saveBilanToPDF(){
    std::string filename;
    filename = QFileDialog::getSaveFileName().toStdString();
    filename+=".pdf";
    CompteRacine::getCompteRacine().setCheminFichier(filename);
    Operations savePDF;
    savePDF.exportBilanToPDF(filename);
    this->ui->statusbar->showMessage("Bilan comptable exporté au format PDF", 4000);
}

void MainWindow::saveReleveToPDF(){
    std::string filename;
    filename = QFileDialog::getSaveFileName().toStdString();
    filename+=".pdf";
    CompteRacine::getCompteRacine().setCheminFichier(filename);
    Operations savePDF;
    savePDF.exportReleveToPDF(filename);
    this->ui->statusbar->showMessage("Relevé des comptes exporté au format PDF", 4000);
}

void MainWindow::saveResultatToPDF(){
    std::string filename;
    filename = QFileDialog::getSaveFileName().toStdString();
    filename+=".pdf";
    CompteRacine::getCompteRacine().setCheminFichier(filename);
    Operations savePDF;
    savePDF.exportResultatToPDF(filename);
    this->ui->statusbar->showMessage("Résultat exporté au format PDF", 4000);
}

void MainWindow::nouveauFichier(){
    this->saveToXML();
    TransactionManager& tm = TransactionManager::getTransactionManager();
    tm.libererTransactionManager();
    CompteRacine& cm = CompteRacine::getCompteRacine();
    cm.libererCompteRacine();
    this->afficherListeCompte();
    this->ui->tableWidget->setRowCount(0);

}

void MainWindow::quitterApp(){
    SauvegardeXML sv;
    sv.sauvegarderContexte();
    this->close();
}

MainWindow::~MainWindow(){
    for(int i=0; i!=ui->ajouterTripletTable->rowCount();i++) delete ui->ajouterTripletTable->cellWidget(i,0);
    delete ui;
}


//! Affiche la liste des transactions associées au compte actuellement sélectionné.
//! Affiche l'ensemble des triplets pour chaque transaction.
void MainWindow::afficherTransaction(Compte* compte){
    this->ui->tableWidget->setRowCount(0);
    this->ui->tableWidget->setProperty("compte", QVariant::fromValue(compte));
    if(!compte->estVirtuel()){
        TransactionManager& tm = TransactionManager::getTransactionManager();
        bool grey_row=false;
        QColor row_color;
        for(auto it = tm.fbegin(dynamic_cast<CompteFeuille&>(*compte)); it!=tm.fend(dynamic_cast<CompteFeuille&>(*compte)); it++){
            if (grey_row) row_color = this->palette().color(QPalette::AlternateBase);
            else row_color = this->palette().color(QPalette::Base);
            this->ui->tableWidget->insertRow(this->ui->tableWidget->rowCount());
            int nbTriplets = 0; // Value needed for setting spans

            for(auto itt = (*it).getTriplets().begin(); itt!=(*it).getTriplets().end(); itt++, nbTriplets++){
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,0, new TransactionItem((*it).getDate().toString(Qt::ISODate), *it, row_color));
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,1, new TransactionItem(QString::fromStdString((*it).getReference()), *it, row_color));
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,2, new TransactionItem(QString::fromStdString((*it).getMemo()), *it, row_color));
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,3, new TransactionItem(QString::fromStdString((*itt).getCompte().getNom()), *it, row_color));
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,4, new TransactionItem(QString::number((*itt).getDebit()), *it, row_color, Qt::AlignRight));
                this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,5, new TransactionItem(QString::number((*itt).getCredit()), *it, row_color, Qt::AlignRight));
                this->ui->tableWidget->insertRow(this->ui->tableWidget->rowCount());

            }
            this->ui->tableWidget->setRowCount(this->ui->tableWidget->rowCount()-1);
            this->ui->tableWidget->setSpan(this->ui->tableWidget->rowCount()-nbTriplets,0,nbTriplets,1);
            this->ui->tableWidget->setSpan(this->ui->tableWidget->rowCount()-nbTriplets,1,nbTriplets,1);
            this->ui->tableWidget->setSpan(this->ui->tableWidget->rowCount()-nbTriplets,2,nbTriplets,1);
            grey_row = !grey_row;
        }
    }
}

//! Supprime la transaction couramment sélectionnée,
//! sinon actualise la barre de statut.
//! La liste des transactions est automatiquement actualisée.
void MainWindow::supprimerTransaction(){
    if(this->ui->tableWidget->currentItem()!=nullptr && this->ui->tableWidget->currentItem()->isSelected()){
        try{
            Transaction& transac = dynamic_cast<TransactionItem*>(this->ui->tableWidget->currentItem())->getTransaction();
            if(ui->ajouterTransactionButton->text() == "Modifier la transaction"){
                if(ui->ajouterTransactionButton->property("Transaction").value<Transaction*>()==&transac) this->clearAjoutTransaction();
            }
            TransactionManager& tm = TransactionManager::getTransactionManager();
            tm.supprimerTransaction(transac);
            this->afficherTransaction(this->ui->tableWidget->property("compte").value<Compte*>());
            this->afficherListeCompte();
        } catch(const char* e){
            this->ui->statusbar->showMessage(QString(e), 4000);
        }
    }
    else{
        this->ui->statusbar->showMessage("Veuillez sélectionner une transaction à supprimer", 4000);
    }
}

/*!
 * Lance la fenetre de Dialog creerCompteDialog
 * */
void MainWindow::ajouterCompte(){
    creerCompteDialog dialog(this);
    dialog.exec();
    this->afficherListeCompte();
}

/*!
 * Supprime le compte sélectionné dans la hiérarchie.
 * Compte vide (sans transaction ni sous-compte) obligatoire.
 * Génère un QMessageBox de vérification avant suppression.
 * */
void MainWindow::supprimerCompte(){
    if(this->ui->treeWidget->currentItem()==nullptr){
        this->ui->statusbar->showMessage("Aucun compte sélectionné", 4000);
        return;
    }
    Compte& compte = static_cast<CompteItem*>(this->ui->treeWidget->currentItem())->getCompte();
    if(compte.estVirtuel()==true){
        if(dynamic_cast<CompteVirtuel&>(compte).getNbSousComptes()!=0){
            this->ui->statusbar->showMessage("Un  compte qui possède des sous-comptes ne peut pas être supprimé", 4000);
            return;
        }
    }
    else{
        TransactionManager& tm = TransactionManager::getTransactionManager();
        if(tm.fbegin(dynamic_cast<CompteFeuille&>(compte))!= tm.fend(dynamic_cast<CompteFeuille&>(compte))){
            this->ui->statusbar->showMessage("Un  compte qui possède des transactions ne peut pas être supprimé", 4000);
            return;
        }
    }

    auto verification = QMessageBox::question(this, "Suppression d'un compte", QString::fromStdString("Voulez-vous supprimer définitivement le compte "+compte.getNom()+" ?"));
    if(verification==QMessageBox::Yes){
        try {
            if(this->ui->tableWidget->property("compte").value<Compte*>()==&compte) this->ui->tableWidget->setProperty("compte", QVariant::fromValue(nullptr));
            compte.supprimerCompte();
            this->ui->statusbar->showMessage("Compte supprimé", 4000);
            this->afficherListeCompte();
        } catch (const char* e) {
            this->ui->statusbar->showMessage(QString(e), 4000);
        }
    }
    else{
        this->ui->statusbar->showMessage("Suppression de compte annulée", 4000);
    }

}


void MainWindow::modifierTransaction(QTableWidgetItem* item){
    TransactionItem* t = dynamic_cast<TransactionItem*>(item);
    if(t->getTransaction().getRapproche()){
        this->ui->statusbar->showMessage("Une transaction rapprochée ne peut pas être modifiée", 4000);
        return;
    }
    ui->ajouterTransactionButton->setText("Modifier la transaction");
    ui->clearTransactionButton->setText("Annuler");
    ui->ajouterTransactionButton->setProperty("Transaction", QVariant::fromValue(&(t->getTransaction())));
    ui->dateEdit->setDate(t->getTransaction().getDate());
    ui->refEdit->setText(QString::fromStdString(t->getTransaction().getReference()));
    ui->memoEdit->setText(QString::fromStdString(t->getTransaction().getMemo()));
    list<TripletTransaction> liste = t->getTransaction().getTriplets();
    while(liste.size() > (unsigned long)ui->ajouterTripletTable->rowCount()){
        MainWindow::ajouterLigne();
    }

    while(liste.size() < (unsigned long)ui->ajouterTripletTable->rowCount()){
        MainWindow::supprimerLigne();
    }
    int i=0;
    for(auto it = liste.begin(); it!=liste.end(); it++){
        dynamic_cast<ChoixCompte*>(ui->ajouterTripletTable->cellWidget(i,0))->setCurrentText(QString::fromStdString((*it).getCompte().getNom()));
        if(ui->ajouterTripletTable->item(i,1)) ui->ajouterTripletTable->item(i,1)->setText(QString::number((*it).getDebit()));
        else {
            auto* item = new QTableWidgetItem(QString::number((*it).getDebit()));
            ui->ajouterTripletTable->setItem(i,1, item);
        }
        if(ui->ajouterTripletTable->item(i,2)) ui->ajouterTripletTable->item(i,2)->setText(QString::number((*it).getCredit()));
        else {
            auto* item = new QTableWidgetItem(QString::number((*it).getCredit()));
            ui->ajouterTripletTable->setItem(i,2, item);
        }
        i++;
    }
}

//! Affiche la liste des comptes dans le QTreeWidget
//! par appel au constructeur de CompteItem.
void MainWindow::afficherListeCompte(){
    //Compte* previous = &dynamic_cast<CompteItem*>(this->ui->treeWidget->currentItem())->getCompte();
    this->ui->treeWidget->clear();
    CompteRacine& cm = CompteRacine::getCompteRacine();
    for(auto it = cm.begin(); it!=cm.end(); it++){
        CompteItem* compte = new CompteItem(*it);
        this->ui->treeWidget->addTopLevelItem(compte);
        //Dérouller le premier niveau de hiérarchie
        this->ui->treeWidget->expandItem(compte);
    }
    //for
}

//! Instancie un CompteItem à partir d'un Compte.
//!
//! Appelle récursivement le constructeur sur tous les Compte fils si le Compte est virtuel.
//!
//! Sinon, crée juste l'item et set son nom et son solde.
CompteItem::CompteItem(Compte& c):QTreeWidgetItem(),compte(c){
    QString nom = QString::fromStdString(this->compte.getNom());
    if(this->compte.estVirtuel()==true) {
        nom += " [virtuel]";
        QFont police=this->font(0);
        police.setItalic(true);
        this->setFont(0,police);
    }
    this->setText(0, nom);
    this->setText(1, QString::number(this->compte.getSolde()));

    this->setTextAlignment(1, Qt::AlignRight);
    if(this->compte.estVirtuel()){
        for(auto it = dynamic_cast<CompteVirtuel*>(&(this->compte))->begin(); it != dynamic_cast<CompteVirtuel*>(&(this->compte))->end(); it++){
            CompteItem* compte = new CompteItem(*it);
            this->addChild(compte);
        }
    }
}


void MainWindow::ajouterTransaction() {
    bool empty = false;
    for(int i=0; i!=ui->ajouterTripletTable->rowCount();i++){
        if(dynamic_cast<ChoixCompte*>(ui->ajouterTripletTable->cellWidget(i,0))->currentText().isEmpty()) empty = true;
        int nbItem = 0;
        for(int j=1; j!=ui->ajouterTripletTable->columnCount();j++){
            if (ui->ajouterTripletTable->item(i,j)){
                if(!ui->ajouterTripletTable->item(i,j)->text().isEmpty()) nbItem++;
            }
        }
        if(!nbItem) empty = true;
    }
    if(empty || ui->refEdit->text()=="" || ui->memoEdit->text()==""){
        this->ui->statusbar->showMessage("Veuillez remplir tous les champs.", 4000);
        return;
    }
    list<TripletTransaction> liste;
    for(int i=0; i!=ui->ajouterTripletTable->rowCount();i++){
        float debit=0;
        float credit=0;
        if (ui->ajouterTripletTable->item(i,1)) debit = ui->ajouterTripletTable->item(i,1)->text().toFloat();
        if (ui->ajouterTripletTable->item(i,2)) credit = ui->ajouterTripletTable->item(i,2)->text().toFloat();
        if (debit == 0 && credit == 0){
            this->ui->statusbar->showMessage("Veuillez rentrer au moins un montant positif à chaque ligne.", 4000);
            return;
        }
        try {
            liste.push_back(TripletTransaction(dynamic_cast<CompteFeuille*>(dynamic_cast<ChoixCompte*>(ui->ajouterTripletTable->cellWidget(i,0))->currentData().value<Compte*>()), debit, credit));
        }
        catch (const char* e) {
            this->ui->statusbar->showMessage(QString(e), 4000);
            return;
        }
    }
    try {
        if(ui->ajouterTransactionButton->text() == "Modifier la transaction"){
            ui->ajouterTransactionButton->property("Transaction").value<Transaction*>()->modifierTransaction(ui->dateEdit->date(), ui->memoEdit->text().toStdString(), ui->refEdit->text().toStdString(), liste);
            ui->ajouterTransactionButton->setText("Ajouter la transaction");
            ui->clearTransactionButton->setText("Effacer");
            this->ui->statusbar->showMessage("La transaction a bien été modifiée.", 4000);
        }
        else {
            TransactionManager::getTransactionManager().creerTransaction(ui->dateEdit->date(), ui->memoEdit->text().toStdString(), ui->refEdit->text().toStdString(), liste);
            this->ui->statusbar->showMessage("La transaction a bien été ajoutée.", 4000);
        }
        MainWindow::clearAjoutTransaction();
        if(ui->treeWidget->currentItem()){
            this->afficherTransaction(this->ui->tableWidget->property("compte").value<Compte*>());
        }
    }
    catch (const char* e) {
        this->ui->statusbar->showMessage(QString(e), 4000);
        return;
    }
    this->afficherListeCompte();
}


void ChoixCompte::showPopup() {
    actualiser();
    QComboBox::showPopup();
}

ChoixCompte::ChoixCompte():QComboBox(){
    actualiser();
}

void ChoixCompte::actualiser(){
    this->clear();
    CompteRacine& cm = CompteRacine::getCompteRacine();
    for(auto it = cm.dbegin(); it!=cm.dend(); it++){
        if(!(*it).estVirtuel()) this->addItem(QString::fromStdString((*it).getNom()),QVariant::fromValue(&*it));
    }
}

void MainWindow::ajouterLigne(){
    if(ui->ajouterTripletTable->rowCount()<5){
        ui->ajouterTripletTable->insertRow(ui->ajouterTripletTable->rowCount());
        ChoixCompte* cc = new ChoixCompte();
        ui->ajouterTripletTable->setCellWidget(ui->ajouterTripletTable->rowCount()-1,0,cc);
    }
}

void MainWindow::supprimerLigne(){
    if(ui->ajouterTripletTable->rowCount()>2){
        delete ui->ajouterTripletTable->cellWidget(ui->ajouterTripletTable->rowCount(),0);
        ui->ajouterTripletTable->removeRow(ui->ajouterTripletTable->rowCount()-1);
    }
}

void MainWindow::clearAjoutTransaction(){
    ui->refEdit->clear();
    ui->memoEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->ajouterTransactionButton->setText("Ajouter la transaction");
    ui->clearTransactionButton->setText("Effacer");
    for(int i=0; i!=ui->ajouterTripletTable->rowCount(); i++){
        for(int j=1; j!=ui->ajouterTripletTable->columnCount(); j++){
            ui->ajouterTripletTable->takeItem(i,j);
        }
    }
    for(int i=0; i!=ui->ajouterTripletTable->rowCount(); i++){
        dynamic_cast<ChoixCompte*>(ui->ajouterTripletTable->cellWidget(i,0))->actualiser();
    }
}


void MainWindow::cloture(){
    int reponse=QMessageBox::question(this, "Titre de la fenêtre", "Êtes-vous sûre de vouloir effectuer une clôture de vos comptes ?", QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes)
        {
            Operations::cloture_livre();
            this->ui->statusbar->showMessage("Clôture bien effectuée", 4000);
            this->afficherListeCompte();
        }
}

void MainWindow::rapprochement(){
    fenetreRapprochement dialog(this);
    dialog.exec();
    if(this->ui->tableWidget->property("compte").value<Compte*>()){
        this->afficherTransaction(this->ui->tableWidget->property("compte").value<Compte*>());
    }
}

TransactionItem::TransactionItem(QString s, Transaction& t, QColor background, Qt::Alignment al):QTableWidgetItem(s),transaction(t){
    this->setTextAlignment(al|Qt::AlignVCenter);
    this->setBackground(QBrush(background));
    if(this->transaction.getRapproche()){
        QFont f = this->font();
        f.setBold(true);
        f.setItalic(true);
        this->setFont(f);
    }


};
