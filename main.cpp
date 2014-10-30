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
//    nodes->printSample(328924);

    nodes->countComps(Node::en);
    nodes->countComps(Node::pl);
//    nodes->printSample(342854);
//    nodes->printSample(328924);
//    nodes->printSample(284799);
//    nodes->printSample(17285);
    nodes->printSample(33756);
//    std::cout<<nodes->dijkstra(17285, 33756, 111824, Node::pl)<<std::endl;
    nodes->countSizeMax();
    nodes->setMainComp();
    int i = 25;
    if (argc > 1){
        i = atoi(argv[1]);
    }
    nodes->setMarkersCount(i);
    nodes->setMarkers();
    nodes->countPaths();
    nodes->countCos(33756);
    nodes->clearMarkers();
    delete nodes;
    return 0;
}