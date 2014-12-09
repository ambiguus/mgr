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
//    nodes->printSample(328924); //show biznes

    nodes->countComps(Node::en);
    nodes->countComps(Node::pl);
//    nodes->printSample(328924);
    nodes->printSample(33756); //domator
//    nodes->printSample(45);
//    std::cout<<nodes->dijkstra(17285, 33756, 111824, Node::pl)<<std::endl;
    nodes->countSizeMax();
    nodes->setMainComp();
    int i = 360;
    int nTop = 15;
    if (argc > 1){
        i = atoi(argv[1]);
    }
    if (argc > 2){
        nTop = atoi(argv[2]);
    }
    int nLoops = 20;
    nodes->setMarkersCount(i);
    for (int i=0; i<nLoops;++i) {
        std::cout<<"loop = "<<i<<std::endl;
        nodes->setMarkers();
        nodes->countPaths();
        nodes->compareTopMarkers(nTop, 33756);
        nodes->clearMarkers();
    }
    nodes->avgTopMarkers(33756, nLoops);
//    nodes->compareTopMarkers(19, 33756);

    delete nodes;
    return 0;
}