#include "CompteRacine.h"
#include <algorithm>
#include <stdexcept>

CompteRacine::Handler CompteRacine::handler=CompteRacine::Handler();

CompteRacine& CompteRacine::getCompteRacine() {
    if (handler.instance==nullptr) handler.instance=new CompteRacine;
    return *handler.instance;
}

void CompteRacine::libererCompteRacine(){
    delete handler.instance;
    handler.instance=nullptr;
}

int CompteRacine::maxId(){
    int idMax=0;
    for (auto it= this->dbegin(); it!=this->dend(); ++it){
        try {
            int id = std::stoi(it->getIdentifiant());
            if(id>idMax)idMax=id;
        } catch (invalid_argument) {}
    }
    return idMax;
}
