#include "Node.hpp"
#include "NodesFactory.hpp"

NodesFactory::NodesFactory():pl_id_(0),en_id_(0),markers_count_(10), markers_pl_exist_(false), markers_en_exist_(false), max_comp_pl_(-1), max_comp_en_(-1){

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
    while (set < markers_count_){
        std::map<int, Node*>::iterator it = nodes_.begin();
        std::advance(it, rand() % size);
        int trans = it->second->getLinksTrans();
        if (it->second->getDegree() == 1 && trans > 0){
            if (it->second->getLang() == Node::pl){
                markers_pl_[set] = it->first;
                markers_en_[set] = trans;
            }else if (it->second->getLang() == Node::en){
                markers_en_[set] = it->first;
                markers_pl_[set] = trans;
            }
            // std::cout<<markers_en_[set]<<std::endl;
            set++;
        }
    }
    markers_en_exist_=true;
    markers_pl_exist_ = true;
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
    int pl=0, en=0;
    std::cout<<max_comp_pl_<<" "<<max_comp_en_<<std::endl;
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end();++it) {
        if ((it->second->getComp() == max_comp_en_ && it->second->getLang() == Node::en) || (it->second->getComp() == max_comp_pl_ && it->second->getLang() == Node::pl)) {
            if (it->second->getLang() == Node::pl){
                pl++;
            }else if (it->second->getLang() == Node::en){
                en++;
            }
            it->second->setInMax(true);
        }
    }
    std::cout<<"pl: "<<pl<<std::endl;
    std::cout<<"en: "<<en<<std::endl;
}
int NodesFactory::dijkstra(int source, int target, int z, char lang) { //z - liczba wierzcholkow
    if (source == target) //uzywamy tlumaczonych keys
        return 0;
    int* dist = new int[z];
    int* prev = new int[z];
    int* visited = new int[z];
    for (int q=0; q<z;++q){
        dist[q] = 1000000000;
        prev[q] = -1;
        visited[q] = 0;
    }
    dist[source]=0;
    int tmp=10000000, visited_nodes=0, current=source, index, iw;
    std::vector<int> nn;
    while (visited_nodes<z){
        std::cout<<visited_nodes<<std::endl;
        for (int w=0;w<z;w++){
            if (visited[w]==0){
                nn.empty();
                if (lang == Node::pl)
                    index = keys_pl_[current];
                else if (lang == Node::en){
                    index = keys_en_[current];
                }
                nn = nodes_[index]->getLinksInside();
                for (std::vector<int>::iterator it=nn.begin(); it != nn.end(); ++it){
                    iw = nodes_[*it]->getIndex();
                    if (iw == w){
                        if (dist[current]+1 < dist[w]){
                            dist[w]=dist[current]+1;
                            prev[w]=current;
                        }
                    }
                }
            }
        }
        visited[current]=1;
        visited_nodes++;
        tmp=10000000;
        for (int v=0;v<z;v++){
            if (visited[v] == 0 && dist[v] < tmp){
                tmp = dist[v];
                current = v;
            }
        }
        if (current == target){
            int u = target;
            int steps=0;
            while (prev[u] != -1){
                steps++;
                u = prev[u];
            }
            return steps;
        }
    }
    return -1;
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
    int* locp = new int[markers_count_];
    int* loce = new int[markers_count_];
    for (int j=0;j<markers_count_;++j){
        locp[j]=dijkstra(i, markers_pl_[j], size_max_pl_, Node::pl);
        std::cout<<locp[j]<<std::endl;
        loce[j]=dijkstra(trans, markers_en_[j], size_max_en_, Node::en);
        std::cout<<loce[j]<<std::endl;
    }
    std::cout<<cosine(locp, loce, markers_count_)<<std::endl;
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
NodesFactory::~NodesFactory() {
    for (std::map<int, Node*>::iterator it=nodes_.begin(); it != nodes_.end(); ++it){
        delete it->second;
    }
    nodes_.clear();
}