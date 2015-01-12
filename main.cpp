#include <iostream>
#include "Node.hpp"
#include "NodesFactory.hpp"
using namespace std;

inline double interwal (const timespec &poczatek, const timespec &koniec)
{
    return (koniec.tv_sec + koniec.tv_nsec * 1e-9) - (poczatek.tv_sec + poczatek.tv_nsec * 1e-9);
}
int main(int argc, char* argv[]) {
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
//    nodes->printSample(328924);
    nodes->printSample(33756); //domator
    nodes->countSizeMax();
    nodes->setMainComp();
    int i = 15;
    if (argc > 1){
        i = atoi(argv[1]);
    }
    nodes->setMarkersCount(i);
    nodes->setMarkers();
    nodes->countPaths();
    cout<<"paths"<<endl;
    nodes->countCos(33756);
    cout<<"cos"<<endl;
    nodes->clearMarkers();
    delete nodes;
    return 0;
}