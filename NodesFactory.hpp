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
#include <list>

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
    int dijkstra(int source, int target, int z, char lang);
    void dijkstraFrom(int source, int z, char lang, int j);
    void countLocs();
    void printSimilarLocs(); // podaje potencjalne tlumaczenia
    void countPair(int i); //use carefully!
    void countPaths();
    void countSizeMax();
    int getSizeMaxPl();
    int getSizeMaxEn();
    double cosine(int* v, int* u, int size);
    void countCos(int i);
    void printSample(int id);
    void clearMarkers();
    void countAvgCos();
    double distance(int *v, int *u, int size);
    int setMarkersBySource(int source, int radius);
    void countPathsLang(int source); // lang - jezyk, w ktorym liczymy sciezki
    int getRankingLang(int source);
    void getRankingAll(std::string nazwa, int radius);
    void countDistances(int radius);
    int getRankingById(int source, std::ostream &output);
    void getMarriages(std::string nazwa);
    void getMarriagesMMDC(std::string nazwa, int length);
    void getPairs(std::string nazwa, int radius, int ranking_radius);
    void getRankingsFromFile(std::string input, std::string output, int ranking_radius);
    ~NodesFactory();

protected:
    std::unordered_map<int, Node*> nodes_;
    int pl_id_; // "pierwszy" z polskich
    int en_id_; // "pierwszy" z angielskich
    int markers_count_; //liczba markerow
    int* markers_pl_;
    int* markers_en_;
    bool markers_pl_exist_, markers_en_exist_;
    std::vector<int> comps_pl_, comps_en_; //wektor: numer komponentu -> liczba wierzcholkow
    int max_comp_pl_, max_comp_en_;
    std::vector<int> keys_pl_, keys_en_;
    int** paths_pl_;
    int** paths_en_;
    int size_max_pl_, size_max_en_;
    int count_pl_, count_en_;
    std::list<int>* ranking_pl_;
    std::list<int>* ranking_en_;
    int ranking_radius_;
//    std::vector<int> source_path_;
};
#endif