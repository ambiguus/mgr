#ifndef _node_hpp_
#define _node_hpp_
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#include "NodesFactory.hpp"

class Node {
public:
    Node();
    Node(int id, char lang);
    Node(int id, char lang, std::string sample);
    char getLang();
    void setLang(char lang);
    int getId();
    void setId(int id);
    void addLinkInside(int id);
    void addLinkTrans(int id);
    std::vector<int> getLinksInside();
    int getLinksTrans();
    void setSample(std::string sample);
    std::string getSample();
    int getDegree();
    int getComp();
    void setComp(int comp);
    void setInMax(bool in_max);
    bool getInMax();
    void setMain(bool main);
    bool getMain();
    void setIndex(int index);
    int getIndex();
    ~Node();
//    int* getLoc(NodesFactory::NodesFactory &factory);
//    int* countLoc(NodesFactory::NodesFactory &factory);
    static const char pl = 'p';
    static const char en = 'e';


protected:
    int id_;
    char lang_;
    std::string sample_;
    std::vector<int> linksInside_; // lista sasiedztwa z wezłami tego samego jezyka
    int linksTrans_; // polaczenie pomiedzy jezykami
    int* loc_; //polozenie wzgledem markerow - tablica o zadanej wielkosci
    int k_;
    bool loc_counted_, k_changed_;
    int comp_; //k - stopien, comp_ - numer komponentu
    bool in_max_, main_;
    int index_;

};
#endif