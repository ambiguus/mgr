#include "Node.hpp"
Node::Node():id_(0),lang_(Node::pl), linksTrans_(0), k_(0), k_changed_(false), comp_(-1), in_max_(false){

}
Node::Node(int id, char lang):id_(id),lang_(lang), linksTrans_(0), k_(0), k_changed_(false), comp_(-1), in_max_(false){
}
Node::Node(int id, char lang, std::string sample):id_(id),lang_(lang), sample_(sample), linksTrans_(0), k_(0), k_changed_(false), comp_(-1), in_max_(false){
}
char Node::getLang(){
    return lang_;
}
void Node::setLang(char lang){
    lang_ = lang;
}
int Node::getId(){
   return id_;
}
void Node::setId(int id){
    id_=id;
}
void Node::addLinkInside(int id){
    if (std::find(linksInside_.begin(), linksInside_.end(), id) == linksInside_.end()) {
        linksInside_.push_back(id);
        k_changed_ = true;
    }
}

void Node::addLinkTrans(int id){
    linksTrans_=id;
}
std::vector<int> Node::getLinksInside(){
    return linksInside_;
}
int Node::getLinksTrans(){
    return linksTrans_;
}
void Node::setSample(std::string sample){
    sample_ = sample;
}
std::string Node::getSample(){
    return sample_;
}
int Node::getDegree() {
    if (k_changed_) {
        k_ = linksInside_.size();
        k_changed_ = false;
    }
    return k_;
}
int Node::getComp(){
    return comp_;
}
void Node::setComp(int comp) {
    if (comp_ == -1)
        comp_ = comp;
}
bool Node::getInMax() {
    return in_max_;
}
void Node::setInMax(bool in_max) {
    in_max_=in_max;
}
void Node::setMain(bool main) {
    main_ = main;
}
bool Node::getMain() {
    return main_;
}
void Node::setIndex(int index) {
    index_=index;
}
int Node::getIndex() {
    return index_;
}
Node::~Node() {
    linksInside_.clear();
}
void Node::clearLoc() {
    loc_.clear();
}
void Node::addLoc(int i){
    loc_.push_back(i);
}
std::vector<int> Node::getLoc() {
    return loc_;
}
void Node::addRanking(double v) {
    if (v > trans_cos_){
        trans_pos_++;
    }
}
void Node::addTransCos(double v) {
    trans_cos_ = v;
    trans_pos_ = 1;
}
int Node::getTransPos() {
    return trans_pos_;
}
//int* getLoc(NodesFactory &factory){}
//int* countLoc(NodesFactory &factory);