#ifndef _nodes_factory_hpp_
#define _nodes_factory_hpp_

#include "Node.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <iterator>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <set>

class NodesFactory{
public:
    NodesFactory();
    void addNodes(std::string nazwa, char lang);
    void addLinksInside(std::string nazwa);
    void addLinksTrans(std::string nazwa);
    void setMarkersCount(int markers_count);
    void printMapSize();
    int* getMarkersPl();
    int* getMarkersEn();
    void setMarkers();
    int getMaxDegree();
    void printDegrees(int max);
    int DFS(int i, int flag); //numer wierzcholka od ktorego liczymy, numer komponentu
    void countComps(char lang);
    void printComps(); //do napisania
    int getMaxComp(char lang);
    void setMainComp();
    void dijkstraFrom(int source, int z, char lang, int j);
    void countPaths();
    void countSizeMax();
    int getSizeMaxPl();
    int getSizeMaxEn();
    double cosine(int* v, int* u, int size);
    double similarity(int* v, int* u, int size);
    int countCos(int i);
    void printSample(int id);
    void clearMarkers();
    double distance(int *v, int *u, int size);
    void compareTopMarkers(int nTop, int source); //nTop - liczba porownywanych najlepszych markerow, source - wezel z ktorym porownujemy wezly drugiej sieci
    void countAllCos(std::string nazwa);
    ~NodesFactory();

protected:
    std::unordered_map<int, Node*> nodes_;
    int markers_count_; //liczba markerow
    int* markers_pl_;
    int* markers_en_;
    std::vector<int> comps_pl_, comps_en_; //wektor: numer komponentu -> liczba wierzcholkow
    int max_comp_pl_, max_comp_en_;
    std::vector<int> keys_pl_, keys_en_;
    int** paths_pl_;
    int** paths_en_;
    int size_max_pl_, size_max_en_;
    int count_pl_, count_en_;
};
#endif