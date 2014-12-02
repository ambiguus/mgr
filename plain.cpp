#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <set>

using namespace std;
int Nmax = 125906;
vector<int>* dzieci = new vector<int>[Nmax]; //vector z dziecmi
int* ildzieci = new int[Nmax];
int* path = new int[Nmax]; //drogi pomiedzy indeksami
int* id = new int[Nmax]; //indeksy tu => id z bazy danych
map<int, int> index; // id z bazy => indeksy, którymi są numerowane tutaj tablice
int odl;
int aktIndex;
int* comp = new int[Nmax]; //numer komponentu
int maxComp=-2; //numer maksymalnego komponentu
int root=-1;
const clockid_t ZegarID = CLOCK_REALTIME;

//===========================================
//inicjuje zmienne
void init(){
    for (int i=0; i<Nmax;++i){
        ildzieci[i] = 0;
        path[i] = -1;
        comp[i]=-1;
    }
    aktIndex=0;
}
//=================================================
//wczytuje dane z pliku
//wczytuję plik z relacją hiponimii - pierwsza wartość jest indeksem podrzędnego synsetu, druga - nadrzędnego
void readFile(string nazwa){
    ifstream file (nazwa.c_str());
    int child, parent;
    int child_id, parent_id;
    aktIndex=0;
    while (file>>child){
        file>>parent;
        if (index.count(child) > 0){ //czy w index jest juz child
            child_id = index[child]; //jesli tak, przypisujemy child_id
//            }
        }else{
            index.insert(make_pair(child, aktIndex)); //jesli nie, tworzymy nowe child_id
            child_id = aktIndex;
            aktIndex++;
            id[child_id] = child;
        }
        if (index.count(parent) > 0){ //podobnie
            parent_id = index[parent];
        }else{
            index.insert(make_pair(parent,aktIndex));
            parent_id = aktIndex;
            aktIndex++;
            id[parent_id]=parent;
        }
        dzieci[child_id].push_back(parent_id);
        dzieci[parent_id].push_back(child_id); //child jest dzieckiem parent
        ildzieci[parent_id]++; //zwiększyła się liczba dzieci parent
        ildzieci[child_id]++;
    }
    cout<<"index: "<<aktIndex<<endl;
}

//===============================================
//dfs - do liczenia komponentów
//i - numer wierzcholka
//flag - numer komponentu
int DFS(int i, int flag) {
    std::vector<int> stos;
    stos.clear();
    if (comp[i] >= 0){
        return 0;
    }
    stos.push_back(i);
    int a, size = 0, no;
    while(!stos.empty()){
        a = stos.back();
        stos.pop_back();
        if (comp[a] < 0) {
            comp[a]=flag;
            size++;
            if (ildzieci[a] > 0) {
                for (std::vector<int>::iterator it = dzieci[a].begin(); it != dzieci[a].end(); ++it) {
                    no = *it;
                    if (comp[no]<0) {
                        stos.push_back(no);
                    }
                }
            }
        }
    }
    return size;
}
//===============================================
//liczy numery komponentów

void countComps() {
    int size, i=0, max = 0, isize=-1;
    for (int j=0; j<Nmax;++j){
        if (comp[j]<0){
            size = DFS(j, i);
            if (size > max){
                max = size;
                isize=i;
            }
            i++;
        }
    }
    cout<<"max comp: "<<isize<<": "<<max<<endl;
    maxComp=isize;
}
//=====================================================
//liczy czas
inline double interwal (const timespec &poczatek, const timespec &koniec)
{
    return (koniec.tv_sec + koniec.tv_nsec * 1e-9) - (poczatek.tv_sec + poczatek.tv_nsec * 1e-9);
}

//=======================================
//liczy ścieżki i wyrzuca do pliku
void printPaths(int source, ostream &out){
    int inf = 1000;
    int* dist = new int[Nmax];
    for (int i=0;i<Nmax;++i){
        dist[i]=inf;
    }
    set<pair<int, int> > q;
    q.insert(make_pair(0,source));
    dist[source] = 0;
    int juz=0;
    int min,imin, in;
    cout<<source<<endl;
    while (!q.empty()){
        min=inf;
        imin=-1;
        pair<int, int> top = *q.begin();
        min=top.first;
        imin=top.second;
        q.erase(q.begin());
        for (vector<int>::iterator it=dzieci[imin].begin();it != dzieci[imin].end(); ++it){
            in = *it;
                if (dist[imin] + 1 < dist[in]){
                    dist[in] = dist[imin]+1;
                    q.insert(make_pair(dist[in],in));
                }
        }
    }
    for (int i=0;i<Nmax;++i){
//        if (comp[i] == maxComp)
              out<<dist[i]<<"\t";


    }
    out<<endl;
    delete dist;
}
//==========================================
//czyści pamięć
void reset(){
    delete path;
    delete ildzieci;
    delete id;
    delete comp;
}

int main(){
    init();
    readFile("hiponimia_pl.txt");
    cout<<"przeczytal"<<endl;
    countComps();
    cout<<"policzyl komponenty"<<endl;
    ofstream out;
    int n=0;
    for (int i=0;i<Nmax;++i){
        if (i%5000 == 0){
            n = i/5000;
            if (out.is_open()){
                out.close();
            }
            out.open("paths/paths_pl_"+to_string(n));
        }
        if (comp[i] == maxComp) {
            printPaths(i, out);
        }

    }
    out.close();
    reset();
    return 0;
}