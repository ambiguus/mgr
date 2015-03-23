#include <iostream>
#include "Node.hpp"
#include "NodesFactory.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3){
        cout<<"za malo parametrow: [1] - plik wynikowy, [2] - zasieg"<<endl;
        return 0;
    }
    NodesFactory* nodes = new NodesFactory();
    nodes->addNodes("rzeczowniki_pl2.txt", Node::pl);
    nodes->addNodes("rzeczowniki_en.txt", Node::en);
    nodes->addLinksInside("hiponimia_en.txt");
    nodes->addLinksInside("hiponimia_pl.txt");
    nodes->addLinksTrans("syn_miedzy.txt");
    nodes->setMarkersCount(10);
//    nodes->printSample(328924); //show biznes
    nodes->countComps(Node::en);
    nodes->countComps(Node::pl);
//    int id=33756;
//    nodes->printSample(id); //domator
    nodes->countSizeMax();
    nodes->setMainComp();
//    nodes->setMarkersBySource(id,3);
//    nodes->countPathsLang(id);
//    cout<<"paths"<<endl;
//    nodes->getRankingLang(id);
//    nodes->clearMarkers();
//    nodes->getRankingAll(argv[1], atoi(argv[2]));
//    nodes->getPairs(argv[1], atoi(argv[2]), 1000);
    nodes->getRankingsFromFile("rankingi_makreryzrodla1000", "pary_1000_from_file", 1000);
    delete nodes;
    return 0;
}