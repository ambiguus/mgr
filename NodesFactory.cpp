#include "Node.hpp"
#include "NodesFactory.hpp"

//construct
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
//    std::cout<<"markery:"<<std::endl;
    while (set < markers_count_){
        std::unordered_map<int, Node*>::iterator it = nodes_.begin();
        std::advance(it, rand() % size);
        int trans = it->second->getLinksTrans();
        if (it->second->getMain() && it->second->getDegree() == 1){ //tu było jeszcze sprawdzanie czy to liść, ale teraz nie robi
                if (it->second->getLang() == Node::pl) {
                    markers_pl_[set] = it->first;
                    markers_en_[set] = trans;
                } else if (it->second->getLang() == Node::en) {
                    markers_en_[set] = it->first;
                    markers_pl_[set] = trans;
                }
                it->second->setIsMarker(true);
                nodes_[trans]->setIsMarker(true);
//                std::cout<<set<<": id - "<<it->first<<", "<<it->second->getSample()<<", tlumaczenie: id - "<<trans<<", "<<nodes_[trans]->getSample()<<std::endl;
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
        imin=-1;
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
//            std::cout<<"it"<<*it<<std::endl;
//            std::cout<<nodes_[*it]->getSample()<<std::endl;
            in = nodes_[*it]->getIndex();
//            std::cout<<"in"<<in<<std::endl;
            if (dist[imin] + 1 < dist[in]){
                dist[in] = dist[imin]+1;
                q.insert(std::make_pair(dist[in],in));
            }
        }
    }
//    std::cout<<"after while"<<std::endl;
    if (lang == Node::pl){
        for (int i=0;i<z;++i){
            paths_pl_[i][j] = dist[i];
        }
    }else{
        for (int i=0;i<z;++i){
            paths_en_[i][j] = dist[i];
        }
    }
    delete dist;
}

//countPaths - liczy drogi od markerów do pozostałych węzłów danej sieci
void NodesFactory::countPaths(){
    paths_pl_ = new int*[count_pl_];
    paths_en_ = new int*[count_en_];
    for (int i=0;i<count_en_;++i){
        paths_en_[i] = new int[markers_count_];
        for (int j=0;j<markers_count_;++j){
            paths_en_[i][j] = -1;
        }
    }
    for (int i=0;i<count_pl_;++i){
        paths_pl_[i] = new int[markers_count_];
        for (int j=0;j<markers_count_;++j){
            paths_pl_[i][j] = -1;
        }
    }
    for (int j=0;j<markers_count_;++j){
        dijkstraFrom(markers_pl_[j], count_pl_, Node::pl, j);
        dijkstraFrom(markers_en_[j],count_en_, Node::en,j);
    }
}

double NodesFactory::similarity(int* v, int* u, int size){
    double sv, su,top=0,sqv=0,squ=0;
    for (int i=0; i<size; ++i){
        if (v[i] == 1000 || u[i] == 1000){
            std::cout<<"marker "<<i<<std::endl;
            return 2;
        }
        sv = 1.0/float(v[i]);
        su = 1.0/float(u[i]);
        top += (sv*su);
        sqv += (sv*sv);
        squ += (su*su);
    }
    return top/(sqrt(sqv*squ));
}

///////za długa !!!!!!!!!!!!!!!!!!!!!!!!!111
int NodesFactory::countCos(int i) {
//    std::cout<<nodes_[i]->getSample()<<std::endl;
    char lang = nodes_[i]->getLang();
    int trans = nodes_[i]->getLinksTrans();
    int* v = new int[markers_count_];
    int index = nodes_[i]->getIndex();
    if (lang == Node::pl){
        for (int j=0;j<markers_count_;++j){
            v[j] = paths_pl_[index][j];
        }
    }else{
        for (int j=0;j<markers_count_;++j){
            v[j] = paths_en_[index][j];
        }
    }
    double cos, tcos, mcos=-10.0;
    int trans_pos = 0, mkey=1;
    int trans_in = nodes_[trans]->getIndex();
    if (lang == Node::pl){
        tcos = similarity(v, paths_en_[trans_in], markers_count_);
    }else{
        tcos = similarity(v, paths_pl_[trans_in], markers_count_);
    }
    if (tcos == 2){
        std::cout<<"straange"<<std::endl;
        printSample(trans);
    }
    if (isnan(tcos)){
        std::cout<<"nan: "<<trans_in<<", "<<trans<<std::endl;
        std::cout<<"slowo\ttrans\tmarkers"<<std::endl;
        if (lang == Node::pl){
            for (int i=0;i<markers_count_;++i){
                std::cout<<v[i]<<"\t"<<paths_en_[trans_in][i]<<"\t"<<markers_en_[i]<<std::endl;
            }
        }else{
            for (int i=0;i<markers_count_;++i){
                std::cout<<v[i]<<"\t"<<paths_pl_[trans_in][i]<<"\t"<<markers_pl_[i]<<std::endl;
            }
        }
    }
    if (lang == Node::pl){
        for (int j=0;j<count_en_;++j){
            if (!nodes_[keys_en_[j]]->getInMax()){
                continue;
            }
            cos = similarity(v, paths_en_[j], markers_count_);
            if (cos > tcos){
                trans_pos++;
            }
            if (cos > mcos){
                mcos = cos;
                mkey = j;
            }
            if (cos == 2){
                std::cout<<"------------------------"<<nodes_[keys_en_[j]]->getSample()<<", "<<nodes_[keys_en_[j]]->getInMax()<<", "<<nodes_[keys_en_[j]]->getMain()<<std::endl;
            }
        }
    }else{
        for (int j=0;j<count_pl_;++j){
            if (!nodes_[keys_pl_[j]]->getInMax()){
                continue;
            }
            cos = similarity(v, paths_pl_[j], markers_count_);
            if (cos > tcos){
                trans_pos++;
            }
            if (cos > mcos){
                mcos = cos;
                mkey = j;
            }
            if (cos == 2){
                std::cout<<"------------------------"<<nodes_[keys_pl_[j]]->getSample()<<", "<<nodes_[keys_pl_[j]]->getInMax()<<", "<<nodes_[keys_pl_[j]]->getMain()<<std::endl;
            }
        }
        
    }
    //drukowanie informacji o miejscu w rankingu
    /*
    std::cout<<markers_count_<<" markerow, "<<nodes_[i]->getSample()<<std::endl;
    std::cout<<"pozycja: \twartosc\tmax_cos\tnajlepszy\tprawdziwy"<<std::endl;
    if (lang == Node::pl){
        std::cout<<trans_pos<<"\t"<<tcos<<"\t"<<mcos<<"\t"<<nodes_[keys_en_[mkey]]->getSample()<<"\t"<<nodes_[trans]->getSample()<<std::endl;
    }else{
        std::cout<<trans_pos<<"\t"<<tcos<<"\t"<<mcos<<"\t"<<nodes_[keys_pl_[mkey]]->getSample()<<"\t"<<nodes_[trans]->getSample()<<std::endl;
    }
    std::cout<<"wektory: "<<std::endl;
    std::cout<<"szukany - "<<i<<": "<<nodes_[i]->getSample()<<std::endl;
    if (lang == Node::pl){
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_pl_[index][j]<<" ";
        }
    }else{
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_en_[index][j]<<" ";
        }
    }
    std::cout<<std::endl;
    if (lang == Node::pl){
        std::cout<<"najlepszy - "<<keys_en_[mkey]<<": "<<nodes_[keys_en_[mkey]]->getSample()<<std::endl;
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_en_[mkey][j]<<" ";
        }
    }else{
        std::cout<<"najlepszy - "<<keys_pl_[mkey]<<": "<<nodes_[keys_pl_[mkey]]->getSample()<<std::endl;
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_pl_[mkey][j]<<" ";
        }
    }
    std::cout<<std::endl;
    std::cout<<"prawdziwy - "<<trans<<": "<<nodes_[trans]->getSample()<<std::endl;
    if (lang == Node::pl){
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_en_[trans_in][j]<<" ";
        }
    }else{
        for (int j=0;j<markers_count_; ++j){
            std::cout<<paths_pl_[trans_in][j]<<" ";
        }
    }
    std::cout<<std::endl;
    std::cout<<similarity(paths_en_[mkey], paths_pl_[index], markers_count_)<<std::endl;
     */
    return trans_pos;
}

//countSizeMax - wyznacza rozmiary maksymalnych komponentów
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

//////////////////////////za długa funkcja!!!!!!!!!
//void NodesFactory::compareTopMarkers(int nTop, int source) {
//    int max, iMax, tRate, tEqual=0, tNear, count, trans, tt; //max - najlepsza zbieżność, imax - indeks tego najbardziej zbieżnego, tRate - pozycja tłumaczenia, tNear - liczba zbieżnych
//    //paths - indexy od 0
//    std::cout<<nodes_[source]->getSample()<<std::endl;
//    trans = nodes_[source]->getLinksTrans();
//    std::cout<<"tlumaczenie: "<<nodes_[trans]->getSample()<<std::endl;
//    char lang = nodes_[source]->getLang();
//    source = nodes_[source]->getIndex();
//    trans = nodes_[trans]->getIndex();
//    int* near = new int[nTop];
//    std::pair<int, int> top;
//
//    for (int i=0;i<nTop;++i){
//        if (lang == Node::pl) {
//            top = *paths_pl_[source].begin();
//            paths_pl_[source].erase(paths_pl_[source].begin());
//        }
//        else {
//            top = *paths_en_[source].begin();
//            paths_en_[source].erase(paths_en_[source].begin());
//        }
//        if (lang == Node::pl){
//            tt = keys_pl_[top.second];
//        }else{
//            tt = keys_en_[top.second];
//        }
//        tt = nodes_[tt]->getLinksTrans();
//        near[i] = nodes_[tt]->getIndex();
//    } //wypełniłam tablicę najbliższych markerów - tlumaczen
//
//    count=0;
//    for (int i=0;i < nTop; ++i){
//        if (lang == Node::en) { //tlumaczenie
//            top = *paths_pl_[trans].begin();
//            paths_pl_[trans].erase(paths_pl_[trans].begin());
//        }
//        else {
//            top = *paths_en_[trans].begin();
//            paths_en_[trans].erase(paths_en_[trans].begin());
//        }
//        if (std::find(std::begin(near), std::end(near), top.second) != std::end(near)){ //istnieje
//            count++;
//        }
//    }
//    tNear = count;
//    tRate = 0;
//    int lCount;
//    if (lang == Node::pl) {
//        lCount = count_en_;
//    }else {
//        lCount = count_pl_;
//    }
//    for (int j=0; j < lCount; ++j){
//        count=0;
//        for (int i=0;i < nTop; ++i){
//            if (lang == Node::en) {
//                top = *paths_pl_[trans].begin();
//                paths_pl_[trans].erase(paths_pl_[trans].begin());
//            }else {
//                top = *paths_en_[trans].begin();
//                paths_en_[trans].erase(paths_en_[trans].begin());
//            }
//            if (std::find(std::begin(near), std::end(near), top.second) != std::end(near)){ //istnieje
//                count++;
//            }
//        }
//        if (count > max){
//            max = count;
//            iMax = j;
//        }
//        if (count > tNear){
//            tRate++;
//        }
//        if (count == tNear){
//            tEqual++;
//        }
//    }
//    std::cout<<"zbieznosc"<<nTop<<"/"<<markers_count_<<" markerow"<<std::endl;
//    int kk;
//    if (lang == Node::pl){
//        kk = keys_en_[iMax];
//    }else{
//        kk = keys_pl_[iMax];
//    }
//    std::cout<<"Najlepsze: "<<kk<<", "<<nodes_[kk]->getSample()<<", zbieznosc"<<max<<std::endl;
//    std::cout<<"Poprawne: miejsce: "<<tRate<<", rownowaznych: "<<tEqual<<std::endl;
//}

void NodesFactory::countAllCos(std::string nazwa){
    std::ofstream file (nazwa.c_str());
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        if (it->second->getMain() && !it->second->getIsMarker()){
            file<<countCos(it->first)<<std::endl;
        }
    }
}

NodesFactory::~NodesFactory() {
    for (std::unordered_map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        delete it->second;
    }
    nodes_.clear();
}