#include <iostream>
#include "Node.hpp"
#include "NodesFactory.hpp"
using namespace std;

int main() {
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
    nodes->printSample(25691);
    nodes->printSample(328924);
    nodes->countSizeMax();
    nodes->setMainComp();
    nodes->setMarkers();
    nodes->countPair(25691);
    delete nodes;
    return 0;
}