#include "Node.hpp"
#include "NodesFactory.hpp"
inline double interwal (const timespec &poczatek, const timespec &koniec)
{
    return (koniec.tv_sec + koniec.tv_nsec * 1e-9) - (poczatek.tv_sec + poczatek.tv_nsec * 1e-9);
}

NodesFactory::NodesFactory():markers_count_(10), max_comp_pl_(-1), max_comp_en_(-1){
        count_en_ = 82192; //liczba angielskich
        count_pl_ = 111824; //liczba polskich
}

//addNodes - dodaje węzły do sieci - czyta z pliku w formacie: id (tab) przykladowe_slowo
void NodesFactory::addNodes(std::string nazwa, char lang){
    int id, cPl=0, cEn=0;
    std::string sample;
    std::ifstream file (nazwa.c_str());
    while (file>>id){
        getline(file, sample);
        nodes_[id] = new Node(id,lang,sample);
        if (lang == Node::pl){
            keys_pl_.push_back(id);
            nodes_[id]->setIndex(cPl);
            cPl++;
        }else if (lang == Node::en){
            keys_en_.push_back(id);
            nodes_[id]->setIndex(cEn);
            cEn++;
        }
    }
    //liczba wezlów w sieci
    if (lang == Node::en) {
    count_en_ = cEn;
    }else {
    count_pl_ = cPl;
    }
    file.close();
}

//addLinksInside - dodaje linki pomiędzy węzłami wewnątrz sieci jednego języka - czyta z pliku w formacie: id1 (\t) id2
void NodesFactory::addLinksInside(std::string nazwa){
    int id1, id2;
    std::ifstream file (nazwa.c_str());
    //sprawdzac czy nodes_[id] istnieja
    int i=0;
    while (file>>id1) {
        i++;
        file >> id2;
        if (nodes_.count(id1) > 0 && nodes_.count(id2) > 0 && nodes_[id1]->getLang() == nodes_[id2]->getLang()) { //jesli istnieja wezly
            nodes_[id1]->addLinkInside(id2);
            nodes_[id2]->addLinkInside(id1);
        }
    }
    file.close();
}

//addLinksTrans - dodaje linki tłumaczeń pomiędzy językami, format jw
void NodesFactory::addLinksTrans(std::string nazwa){
    int id1, id2;
    std::ifstream file (nazwa.c_str());
    while (file>>id1){
        file>>id2;
        //sprawdzac czy obiekt istnieje
        if (nodes_.count(id1) > 0 && nodes_.count(id2) > 0) {
            nodes_[id1]->addLinkTrans(id2);
            nodes_[id2]->addLinkTrans(id1);
        }
    }
    file.close();
}

//setMarkersCount - ustawia liczbę markerów
void NodesFactory::setMarkersCount(int markers_count){
    markers_count_=markers_count;
}

//printMapSize - drukuje liczbę markerów
void NodesFactory::printMapSize() {
    std::cout<<nodes_.size()<<std::endl;
}

//getMarkersPl - zwraca markery w sieci języka polskiego
int* NodesFactory::getMarkersPl() {
    return markers_pl_;
}

//getMarkersEn - jw, ale w sieci języka angielskiego
int* NodesFactory::getMarkersEn() {
    return markers_en_;
}

//setMarkers - ustala markery w obu sieciach
void NodesFactory::setMarkers() {
    markers_en_ = new int[markers_count_];
    markers_pl_ = new int[markers_count_];
    int set=0, size = nodes_.size();
    srand(time(NULL));
    while (set < markers_count_){
        std::unordered_map<int, Node*>::iterator it = nodes_.begin();
        std::advance(it, rand() % size);
        int trans = it->second->getLinksTrans();
        if (it->second->getMain()){ //tu było jeszcze sprawdzanie czy to liść, ale teraz nie robi
                if (it->second->getLang() == Node::pl) {
                    markers_pl_[set] = it->first;
                    markers_en_[set] = trans;
                } else if (it->second->getLang() == Node::en) {
                    markers_en_[set] = it->first;
                    markers_pl_[set] = trans;
                }
                set++;
        }
    }
}

//getMaxDegree - zwraca maksymalny stopień węzła w obu sieciach
int NodesFactory::getMaxDegree() {
    int max = 0, m;
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it!=nodes_.end(); ++it){
        m=it->second->getDegree();
        if (max < m){
            max = m;
        }
    }
    return max;
}

//printDegrees - drukuje rozklad stopni wierzchołków obu sieci dla stopni mniejszych niż max
void NodesFactory::printDegrees(int max) {
    int* hist = new int[max];
    for (int i=0; i< max; ++i){
        hist[i]=0;
    }
    int m;
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it!=nodes_.end(); ++it){
            m=it->second->getDegree();
            if (m < max){
                hist[m]++;
            }
    }
    for (int i=0; i<max;++i){
        std::cout<<i<<"\t"<<hist[i]<<std::endl;
    }
}

//dfs - algorytm dfs
int NodesFactory::DFS(int i, int flag) {
    std::vector<int> nn;
    nn.clear();
    std::vector<int> stos;
    stos.clear();
    if (nodes_[i]->getComp() >= 0){
        return 0;
    }
    stos.push_back(i);
    char lang = nodes_[i]->getLang();
    int a, size = 0, no;
    while(!stos.empty()){

        a = stos.back();
        stos.pop_back();
        if (nodes_[a]->getComp() < 0 && nodes_[a]->getLang() == lang) {
            nodes_[a]->setComp(flag);
            size++;
            if (nodes_[a]->getDegree() > 0) {
                nn.clear();
                nn = nodes_[a]->getLinksInside();
                for (std::vector<int>::iterator it = nn.begin(); it != nn.end(); ++it) {
                    no = *it;
                    if (nodes_[no]->getComp() < 0 && nodes_[no]->getLang() == lang) {
                        stos.push_back(no);
                    }
                }
            }
        }
    }
    return size;
}


//countcomps - nadaje węzłom sieci lang numery ich komponentów i znajduje maksymalny komponent
void NodesFactory::countComps(char lang) {
    if (lang == Node::pl) {
        comps_pl_.clear();
    }else if (lang == Node::en){
        comps_en_.clear();
    }
    int size, i=0, max = 0, isize=-1;

    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end();++it){
        if (it->second->getComp() < 0 && it->second->getLang() == lang){
            size = DFS(it->first, i);
            if (size > max){
                max = size;
                isize=i;
            }
            i++;
            if (lang == Node::pl) {
                comps_pl_.push_back(size);
            }else if (lang== Node::en){
                comps_en_.push_back(size);
            }
        }
    }
    if (lang == Node::pl) {
        max_comp_pl_ = isize;
        size_max_pl_ = max;
    }else if (lang == Node::en){
        max_comp_en_ = isize;
        size_max_en_ = max;
    }
//    std::cout<<lang<<": "<<max<<std::endl;
}

//getMaxComp - zwraca numer maksymalnego komponentu danej sieci
int NodesFactory::getMaxComp(char lang){
    if (lang == Node::pl)
        return max_comp_pl_;
    else return max_comp_en_;
}

//setMainComp - nadaje węzłom sieci wartość true zmiennej main, jeśli węzeł może być markerem (ma odpowiednik w drugiej sieci)
//i wartość true zmiennej inMax, jeśli węzeł należy do maksymalnego komponentu
void NodesFactory::setMainComp() {
    int pl=0, en=0, trans;
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end();++it) {
        if ((it->second->getComp() == max_comp_en_ && it->second->getLang() == Node::en)) {
            en++;
            trans = it->second->getLinksTrans();
            if (trans > 0){
                if (nodes_[trans]->getComp() == max_comp_pl_ && nodes_[trans]->getLinksTrans() == it->first){
                    it->second->setMain(true);
                    nodes_[trans]->setMain(true);
                }
            }
            it->second->setInMax(true);
        }
        if ((it->second->getComp() == max_comp_pl_ && it->second->getLang() == Node::pl)) {
            pl++;
            trans = it->second->getLinksTrans();
            if (trans > 0){
                if (nodes_[trans]->getComp() == max_comp_en_ && nodes_[trans]->getLinksTrans() == it->first){
                    it->second->setMain(true);
                    nodes_[trans]->setMain(true);
                }
            }
            it->second->setInMax(true);
        }
    }
}

//dijkstraFrom - liczy drogi z węzła source do wszystkich węzłów danej sieci lang, source jest j-tym markerem sieci lang, z oznacza liczbę węzłów w tej sieci
void NodesFactory::dijkstraFrom(int source, int z, char lang, int j) {
    int inf = 1000;
    int* dist = new int[z];
    for (int i=0;i<z;++i){
        dist[i]=inf;
    }
    source = nodes_[source]->getIndex();
    std::set<std::pair<int, int> > q;
    std::vector<int> nn;
    q.insert(std::make_pair(0,source));
    dist[source] = 0;
    int imin, in, key;
    while (!q.empty()){
        std::pair<int, int> top = *q.begin();
        imin=top.second;
        q.erase(q.begin());
        if (lang == Node::pl){
            key = keys_pl_[imin];
        }else{
            key = keys_en_[imin];
        }
        nn.clear();
        nn = nodes_[key]->getLinksInside();
        for (std::vector<int>::iterator it=nn.begin();it != nn.end(); ++it){
            in = nodes_[*it]->getIndex();
            if (dist[imin] + 1 < dist[in]){
                dist[in] = dist[imin]+1;
                q.insert(std::make_pair(dist[in],in));
            }
        }
    }
    if (lang == Node::pl){
        for (int i=0;i<z;++i){
            paths_pl_[i].insert(std::make_pair(dist[i], source));
        }
    }else{
        for (int i=0;i<z;++i){
            paths_en_[i].insert(std::make_pair(dist[i], source));
        }
    }

    q.clear();
    nn.clear();
    delete dist;
}

//countPaths - liczy drogi od markerów do pozostałych węzłów danej sieci
void NodesFactory::countPaths(){
    paths_pl_ = new std::set<std::pair<short int, int> >[count_pl_];
    paths_en_ = new std::set<std::pair<short int, int> >[count_en_];
        for (int j=0;j<markers_count_;++j){
        dijkstraFrom(markers_pl_[j], count_pl_, Node::pl, j);
        dijkstraFrom(markers_en_[j],count_en_, Node::en,j);
    }
    delete[] markers_pl_; //robic to?
    delete[] markers_en_;
}

void NodesFactory::countSizeMax() {
    int comp;
    char lang;
    size_max_en_ = 0;
    size_max_pl_ = 0;
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        comp = it->second->getComp();
        lang = it->second->getLang();
        if (comp == max_comp_pl_ && lang == Node::pl){
            size_max_pl_++;
        }
        if (comp == max_comp_en_ && lang == Node::en){
            size_max_en_++;
        }
    }
}

//getSizeMaxEn - zwraca rozmiar maksymalnego komponentu sieci angielskiej
int NodesFactory::getSizeMaxEn() {
    return size_max_en_;
}

//getSizeMaxPl - jw, sieć polska
int NodesFactory::getSizeMaxPl() {
    return size_max_pl_;
}

//cosine - liczy podobieństwo cosinusowe dwóch wektorów
double NodesFactory::cosine(int *v, int *u, int size) {
    double sum=0.0;
    double qv=0.0, qu=0.0, q;
    for (int i=0;i<size;++i){
        sum += (v[i]*u[i]);
        qv += (v[i]*v[i]);
        qu += (u[i]*u[i]);
    }
    q = sqrt(qv)*sqrt(qu);
    return double(sum)/q;
}

//distance - liczy odległość Euklidesową między dwoma wektorami
double NodesFactory::distance(int *v, int *u, int size) {
    double sum=0.0;
    for (int i=0;i<size;++i){
        sum += ((v[i]-u[i])*(v[i]-u[i]));
    }
    sum = double(sum);
    return sqrt(sum)/size;
}

//printSample - drukuje przykładowe słowo i numer komponentu danego synsetu (węzła)
void NodesFactory::printSample(int id){
    std::cout<<nodes_[id]->getSample()<<std::endl;
    std::cout<<nodes_[id]->getMain()<<std::endl;
    int a = nodes_[id]->getLinksTrans();
    if (a > 0){
        std::cout<<nodes_[a]->getSample()<<std::endl;
        std::cout<<nodes_[a]->getMain()<<std::endl;
    }
}

//clearMarkers - czyści tablice markerów i dróg
void NodesFactory::clearMarkers() {
    delete markers_en_;
    delete markers_pl_;
    delete paths_en_;
    delete paths_pl_;
}

void NodesFactory::pathsToArray(int nTop){
    topPathsPl_ = new int*[count_pl_];
    topPathsEn_ = new int*[count_en_];
    std::pair<int, int> top;
    int key;
    for (int i=0; i<count_pl_; ++i){
        key = keys_pl_[i];
        if (nodes_[key]->getInMax()){
            topPathsPl_[i] = new int[nTop];
            for (int j=0; j<nTop;++j){
                top = *paths_pl_[i].begin();
                paths_pl_[i].erase(paths_pl_[i].begin());
                topPathsPl_[i][j] = top.second;
                if (top.second == i){
                    j--;
                }
            }
        }
        paths_pl_[i].clear();
        
    }
    for (int i=0; i<count_en_; ++i){
        key = keys_en_[i];
        if (nodes_[key]->getInMax()){
            topPathsEn_[i] = new int[nTop];
            for (int j=0; j<nTop;++j){
                top = *paths_en_[i].begin();
                paths_en_[i].erase(paths_en_[i].begin());
                topPathsEn_[i][j] = top.second;
                if (top.second == i){
                    j--;
                }
            }
        }
        paths_en_[i].clear();
    }
    delete[] paths_en_;
    delete[] paths_pl_;
}
int NodesFactory::compareTopMarkers(int nTop, int source) {
    int tRate, tNear, count, trans, tt, key; //max - najlepsza zbieżność, imax - indeks tego najbardziej zbieżnego, tRate - pozycja tłumaczenia, tNear - liczba zbieżnych
    trans = nodes_[source]->getLinksTrans();
//    std::cout<<"tlumaczenie: id="<<trans<<", "<<nodes_[trans]->getSample()<<std::endl;
    char lang = nodes_[source]->getLang();
    source = nodes_[source]->getIndex();
    trans = nodes_[trans]->getIndex();
    std::vector<int> near;
    int itop;
    for (int i=0;i<nTop;++i){
        if (lang == Node::pl) {
            itop = topPathsPl_[source][i];
            tt = keys_pl_[itop];
        }
        else {
            itop = topPathsEn_[source][i];
            tt = keys_en_[itop];
        }
        tt = nodes_[tt]->getLinksTrans();
        near.push_back(nodes_[tt]->getIndex());
    } //wypełniłam tablicę najbliższych markerów -> dodałam ich tłumaczenia
    count=0;
    for (int i=0;i < nTop; ++i){ // sprawdza ile z najblizszych markerow tlumaczenia jest tez tlumaczeniami markerow najblizej badanego wezla (source)
        if (lang == Node::en) { //tlumaczenie
            itop = topPathsPl_[trans][i];
        }
        else {
            itop = topPathsEn_[trans][i];
        }
        for (std::vector<int>::iterator it=near.begin(); it != near.end(); ++it){
            if (*it == itop){
                count++;
                break;
            }
        }
    }
    tNear = count;
    tRate = 0;
    int lCount;
    if (lang == Node::pl) {
        lCount = count_en_;
    }else {
        lCount = count_pl_;
    }
    for (int j=0; j < lCount; ++j){
        if (trans == j){
            continue;
        }
        if (lang == Node::pl){
            key = keys_en_[j];
        }else{
            key = keys_pl_[j];
        }
        if (!nodes_[key]->getInMax()){
            continue;
        }
        count=0;
        for (int i=0;i < nTop; ++i){
            if (lang == Node::en) {
                itop = topPathsPl_[j][i];
            }else {
                itop = topPathsEn_[j][i];
            }
            for (std::vector<int>::iterator it=near.begin(); it != near.end(); ++it){
                if (*it == itop){
                    count++;
                    break;
                }
            }
        }
        if (count >= tNear){
            tRate++;
        }
    }
    return tRate;
}
void NodesFactory::topMarkersAll(std::string nazwa, int nTop){
    std::ofstream file(nazwa.c_str());
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        if (it->second->getMain()){
            file<<compareTopMarkers(nTop, it->first)<<std::endl;
        }
    }
}
NodesFactory::~NodesFactory() {
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        delete it->second;
    }
    nodes_.clear();
}