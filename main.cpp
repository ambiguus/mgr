#include <iostream>
#include "Node.hpp"
#include "NodesFactory.hpp"
using namespace std;

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
    nodes->printSample(328924);
//    nodes->printSample(33756); //domator
//    std::cout<<nodes->dijkstra(17285, 33756, 111824, Node::pl)<<std::endl;
    nodes->countSizeMax();
    nodes->setMainComp();
    int i = 10;
    if (argc > 1){
        i = atoi(argv[1]);
    }
    nodes->setMarkersCount(i);
    nodes->setMarkers();

    nodes->countPaths();
    nodes->countCos(328924);
    nodes->clearMarkers();
    delete nodes;
    return 0;
}