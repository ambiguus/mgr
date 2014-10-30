#include "Node.hpp"
#include "NodesFactory.hpp"

NodesFactory::NodesFactory():pl_id_(0),en_id_(0),markers_count_(10), markers_pl_exist_(false), markers_en_exist_(false), max_comp_pl_(-1), max_comp_en_(-1){
        count_en_ = 82192;
        count_pl_ = 111824;
}
void NodesFactory::addNodes(std::string nazwa, char lang){
    int id, cPl=0, cEn=0;
    std::string sample;
    std::ifstream file (nazwa.c_str());

    if (file>>id) {
        if (lang == Node::en) {
            en_id_ = id;
        } else if (lang == Node::pl){
            pl_id_ = id;
        }
        getline(file, sample);
        nodes_[id] = new Node(id, lang, sample);
        if (lang == Node::pl){
            keys_pl_.push_back(id);
            nodes_[id]->setIndex(cPl);
            cPl++;
        }else if (lang==Node::en){
            keys_en_.push_back(id);
            nodes_[id]->setIndex(cEn);
            cEn++;
        }
    }
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
    std::cout<<"cPl: "<<cPl<<std::endl;
    std::cout<<"cEn: "<<cEn<<std::endl;
    file.close();
}
void NodesFactory::addLinksInside(std::string nazwa){
    int id1, id2;
    std::ifstream file (nazwa.c_str());
    //sprawdzac czy nodes_[id] istnieja
    int i=0;
    while (file>>id1){
        i++;
        file>>id2;
        if (nodes_.count(id1) > 0 && nodes_.count(id2) > 0 && nodes_[id1]->getLang() == nodes_[id2]->getLang()) { //jesli istnieja wezly
            nodes_[id1]->addLinkInside(id2);
            nodes_[id2]->addLinkInside(id1);
        }
    }
    std::cout<<nazwa<<": "<<i<<std::endl;
    file.close();
}
void NodesFactory::addLinksTrans(std::string nazwa){
    int id1, id2;
    //sprawdzac czy obiekt istnieje
    std::ifstream file (nazwa.c_str());
    while (file>>id1){
        file>>id2;
        if (nodes_.count(id1) > 0 && nodes_.count(id2) > 0) {
            nodes_[id1]->addLinkTrans(id2);
            nodes_[id2]->addLinkTrans(id1);
        }
    }
    file.close();
}
//int* getMarkers();
void NodesFactory::setMarkersCount(int markers_count){
    markers_count_=markers_count;
}

void NodesFactory::printMapSize() {
    std::cout<<nodes_.size()<<std::endl;
}

int* NodesFactory::getMarkersPl() {
    return markers_pl_;
}
int* NodesFactory::getMarkersEn() {
    return markers_en_;
}
void NodesFactory::setMarkers() {
    markers_en_ = new int[markers_count_];
    markers_pl_ = new int[markers_count_];
    //dorobic
    int set=0, size = nodes_.size();
    std::cout<<"markery: "<<std::endl;
    while (set < markers_count_){
        std::map<int, Node*>::iterator it = nodes_.begin();
        std::advance(it, rand() % size);
        int trans = it->second->getLinksTrans();
        if (it->second->getMain()){
                if (it->second->getLang() == Node::pl) {
                    markers_pl_[set] = it->first;
                    markers_en_[set] = trans;
                } else if (it->second->getLang() == Node::en) {
                    markers_en_[set] = it->first;
                    markers_pl_[set] = trans;
                }
                // std::cout<<markers_en_[set]<<std::endl;
                set++;
                std::cout<<markers_en_[set-1]<<", ";

        }
    }
    std::cout<<std::endl;
}
int NodesFactory::getMaxDegree() {
    int max = 0, m;
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it!=nodes_.end(); ++it){
        m=it->second->getDegree();
        if (max < m){
            max = m;
        }
    }
    return max;
}
void NodesFactory::printDegrees(int max) {
    int* hist = new int[max];
    for (int i=0; i< max; ++i){
        hist[i]=0;
    }
    int m;
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it!=nodes_.end(); ++it){
            m=it->second->getDegree();
            if (m < max){
                hist[m]++;
            }
    }
    for (int i=0; i<max;++i){
        std::cout<<i<<"\t"<<hist[i]<<std::endl;
    }
}

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

void NodesFactory::countComps(char lang) {
    if (lang == Node::pl) {
        comps_pl_.clear();
    }else if (lang == Node::en){
        comps_en_.clear();
    }
    int size, i=0, max = 0, isize=-1;

    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end();++it){
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
    std::cout<<lang<<": "<<max<<std::endl;
}

int NodesFactory::getMaxComp(char lang){
    if (lang == Node::pl)
        return max_comp_pl_;
    else return max_comp_en_;
}

void NodesFactory::setMainComp() {
    int pl=0, en=0, trans;
    std::cout<<max_comp_pl_<<" "<<max_comp_en_<<std::endl;
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end();++it) {
        if ((it->second->getComp() == max_comp_en_ && it->second->getLang() == Node::en) || (it->second->getComp() == max_comp_pl_ && it->second->getLang() == Node::pl)) {
            if (it->second->getLang() == Node::pl){
                pl++;
                    trans = it->second->getLinksTrans();
                if (trans > 0){
                    if (nodes_[trans]->getComp() == max_comp_en_){
                        it->second->setMain(true);
                    }
                }
            }else if (it->second->getLang() == Node::en){
                en++;
                    trans = it->second->getLinksTrans();
                    if (trans > 0){
                    if (nodes_[trans]->getComp() == max_comp_pl_){
                        it->second->setMain(true);
                    }
                }
            }
            it->second->setInMax(true);
        }
    }
    std::cout<<"pl: "<<pl<<std::endl;
    std::cout<<"en: "<<en<<std::endl;
}
int NodesFactory::dijkstra(int source, int target, int z, char lang) {
    int inf = 1000000000;
    int* dist = new int[z];
    int* prev = new int[z];
    source = nodes_[source]->getIndex();
    target = nodes_[target]->getIndex();
    int min=inf, imin=0, key, index;
    for (int i=0;i<z;++i){
        dist[i]=inf;
        prev[i]=-1;
    }
    std::vector<int> q;
    std::vector<int> nn;
    q.push_back(source);
    dist[source] = 0;
    std::vector<int> s;
    while (!q.empty()){
        min=inf;
        imin=0;
        for (std::vector<int>::iterator it=q.begin();it != q.end(); ++it){
            if (dist[*it] < min){
                min = dist[*it];
                imin = *it;
            }
        }
        q.erase(std::remove(q.begin(),q.end(), imin), q.end());
        s.push_back(imin);
        nn.clear();
        if (lang == Node::pl){
            key = keys_pl_[imin];
        }else{
            key = keys_en_[imin];
        }
        nn = nodes_[key]->getLinksInside();
        for (std::vector<int>::iterator it=nn.begin();it != nn.end(); ++it){
            index = nodes_[*it]->getIndex();
            if (std::find(s.begin(), s.end(), index) == s.end()){ //nie zawiera
                if (dist[imin] + 1 < dist[index]){
                    dist[index] = dist[imin]+1;
                    prev[index] = imin;
                    q.push_back(index);
                }
            }
            if (index == target){
                return dist[index];
            }
        }
    }
    return dist[target];
}
void NodesFactory::dijkstraFrom(int source, int z, char lang, int j) {
    int inf = 1000000000;
    int* dist = new int[z];
    int* prev = new int[z];
    source = nodes_[source]->getIndex();
    int min=inf, imin=0, key, index;
    for (int i=0;i<z;++i){
        dist[i]=inf;
        prev[i]=-1;
    }
    std::vector<int> q;
    std::vector<int> nn;
    q.push_back(source);
    dist[source] = 0;
    std::vector<int> s;
    while (!q.empty()){
        min=inf;
        imin=0;
        for (std::vector<int>::iterator it=q.begin();it != q.end(); ++it){
            if (dist[*it] < min){
                min = dist[*it];
                imin = *it;
            }
        }
        q.erase(std::remove(q.begin(),q.end(), imin), q.end());
        s.push_back(imin);
        nn.clear();
        if (lang == Node::pl){
            key = keys_pl_[imin];
        }else{
            key = keys_en_[imin];
        }
        nn = nodes_[key]->getLinksInside();
        for (std::vector<int>::iterator it=nn.begin();it != nn.end(); ++it){
            index = nodes_[*it]->getIndex();
            if (std::find(s.begin(), s.end(), index) == s.end()){ //nie zawiera
                if (dist[imin] + 1 < dist[index]){
                    dist[index] = dist[imin]+1;
                    prev[index] = imin;
                    q.push_back(index);
                }
            }
        }
    }
    if (lang == Node::pl){
        for (int i=0;i<z;++i){
            paths_pl_[i][j] = dist[i];
        }
    }else{
        for (int i=0;i<z;++i){
            paths_en_[i][j] = dist[i];
        }
    }
}
void NodesFactory::countPair(int i){
    int trans = nodes_[i]->getLinksTrans();
    if (trans < 0){
        std::cout<<"brak polaczenia"<<std::endl;
        return;
    }
    if (nodes_[i]->getLang() == Node::en){
        int tmp = i; //zrob ladniej
        i = trans;
        trans = tmp;
    }
    i = 25691;
    trans = 284799;
    int* locp = new int[markers_count_];
    int* loce = new int[markers_count_];
    for (int j=0;j<markers_count_;++j){
        locp[j]=dijkstra(i, markers_pl_[j], 111824, Node::pl);
        std::cout<<locp[j]<<std::endl;
        loce[j]=dijkstra(trans, markers_en_[j], 82192, Node::en);
        std::cout<<loce[j]<<std::endl;
    }
    std::cout<<cosine(locp, loce, markers_count_)<<std::endl;
}
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
void NodesFactory::countCos(int i) {
    int trans = nodes_[i]->getLinksTrans();
    int* v = new int[markers_count_];
    int index = nodes_[i]->getIndex();
    for (int j=0;j<markers_count_;++j){
        v[j] = paths_pl_[index][j];
    }
    double cos, tcos, mcos=-10.0;
    int trans_pos = 1, mkey=1;
    int trans_in = nodes_[trans]->getIndex();
    tcos = cosine(v, paths_en_[trans_in], markers_count_);
    if (isnan(tcos)){
        std::cout<<"nan: "<<trans_in<<", "<<trans<<std::endl;
        std::cout<<"slowo\ttrans\tmarkers"<<std::endl;
        for (int i=0;i<markers_count_;++i){
            std::cout<<v[i]<<"\t"<<paths_en_[trans_in][i]<<"\t"<<markers_en_[i]<<std::endl;
        }
    }
    for (int j=0;j<count_en_;++j){
        cos = cosine(v, paths_en_[j], markers_count_);
        if (cos > tcos){
            trans_pos++;
        }
        if (cos > mcos){
            mcos = cos;
            mkey = j;
        }
    }
    std::cout<<nodes_[i]->getSample()<<std::endl;
    std::cout<<markers_count_<<" markerow, pos: "<<trans_pos<<", wartość: "<<tcos<<", max: "<<mcos<<", "<<nodes_[keys_en_[mkey]]->getSample()<<", "<<nodes_[trans]->getSample()<<std::endl;
}

void NodesFactory::countSizeMax() {
    int comp;
    char lang;
    size_max_en_ = 0;
    size_max_pl_ = 0;
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
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

int NodesFactory::getSizeMaxEn() {
    return size_max_en_;
}
int NodesFactory::getSizeMaxPl() {
    return size_max_pl_;
}

double NodesFactory::cosine(int *v, int *u, int size) {
    int sum=0;
    double qv=0.0, qu=0.0, q;
    for (int i=0;i<size;++i){
        sum += (v[i]*u[i]);
        qv += (v[i]*v[i]);
        qu += (u[i]*u[i]);
    }
    q = sqrt(qv*qu);
    return double(sum)/q;
}

void NodesFactory::printSample(int id){
    std::cout<<nodes_[id]->getSample()<<std::endl;
    std::cout<<nodes_[id]->getComp()<<std::endl;
}
void NodesFactory::clearMarkers() {
    delete markers_en_;
    delete markers_pl_;
    delete paths_en_;
    delete paths_pl_;
}
NodesFactory::~NodesFactory() {
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        delete it->second;
    }
    nodes_.clear();
}