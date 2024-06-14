#include <bits/stdc++.h>
using namespace std;

struct Node {
    char character;
    int freq;
    struct Node *l, *r;
};

void swap(Node *x, Node *y); 

class minheap{
    int capacity; 
    int heapsize;
    vector<Node*> heap;
public:
    minheap(int size);
    void insert(Node *key);
    void heapify(int i);
    void print();
    int sizeofheap();
    Node* extractMin();
};

minheap::minheap(int size){
    heapsize = 0;
    capacity = size;
    heap = vector<Node*>(size);
}

void minheap::insert(Node *key){
    if(heapsize==capacity){
        cout<<"overflow"<<endl;
    }
    heapsize++;
    int i = heapsize-1;
    heap[i] = key;
    
    while(i!=0 && heap[(i-1)/2]->freq>heap[i]->freq){
        swap(heap[i],heap[(i-1)/2]);
        i = (i-1)/2;
    }
}

Node* minheap::extractMin(){
    if(heapsize<=0){
        Node *n =  new Node();
        n->freq = -1;
        return(n);
    }
    if(heapsize==1){
        heapsize--;
        return(heap[0]);
    }
    Node* root = heap[0];
    heap[0]=heap[heapsize-1];
    heapsize--;
    heapify(0);
    return(root);
}

void minheap::heapify(int i){
    int l = 2*i+1;
    int r = 2*i+2;
    int smallest = i;
    if(l<heapsize && heap[l]->freq<heap[i]->freq){
        smallest = l;
    }
    if(r<heapsize && heap[r]->freq<heap[i]->freq){
        smallest = r;
    }
    if(smallest!=i){
        swap(heap[i],heap[smallest]);
        heapify(smallest);
    }
}

void minheap::print(){
    for(int i=0;i<heapsize;i++){
        cout<<heap[i]->freq<<" "<<heap[i]->character<<"  ";
    }
}

int minheap::sizeofheap(){
    return(heapsize);
}

void swap(Node *x, Node *y) 
{ 
    Node temp = *x; 
    *x = *y; 
    *y = temp; 
} 

struct Node * buildhuffmantree(minheap *heap){ 
    struct Node *l,*r,*top;
    while(heap->sizeofheap()>1){
        l = heap->extractMin();
        r = heap->extractMin();
        top = new Node();
        top->freq = l->freq+r->freq;
        top->character='$';
        top->l = l;
        top->r = r;
        heap->insert(top);
    }
    return(heap->extractMin());
}

void buildCodesHelper(Node *root,string code,map<char,string> &m){
    if(root==NULL){
        return;
    }
    if(root->character!='$'){
        m[root->character] = code;
    }
    buildCodesHelper(root->l,code+"0",m);
    buildCodesHelper(root->r,code+"1",m);
}

void buildCodes(Node *root,map<char,string> &m){
    buildCodesHelper(root,"",m);
}

void printcodesintofile(int noofchars,int count,ofstream &newfile,map<char,string> &bincodes,string extention){
    newfile<<count<<" "<<noofchars<<" "<<extention<<"\n";
    for(auto& i:bincodes){
        newfile<<i.first<<' '<<i.second<<endl;
    }
}

void printtextintofile(ifstream &file,ofstream &newfile,map<char,string> &codes){
    string text;
    int bitcount=0;
    int currbits = 0;
    while (getline (file, text)) {
        for(auto i:text){
            string s = codes[i];
            for(auto j:s){
                currbits = currbits<<1 | atoi(&j);
                bitcount++;
                if(bitcount==8){
                    newfile.write(reinterpret_cast<char*>(&currbits),1);
                    currbits=0;
                    bitcount=0;
                }
            }
        }
    }
}

int main(int argc, char* argv[]){
    string file = argv[1];
    map<char,int> m;
    int noofchars = 0;
    string myText;
    ifstream MyReadFile(file);
    while (getline (MyReadFile, myText)) {
        for(auto i:myText){
            m[i]++;
            noofchars++;
        }
    }
    MyReadFile.close(); 
    minheap heap(m.size());
    for(auto i:m){
        Node *n = new Node();
        n->freq = i.second;
        n->character = i.first;
        n->l=NULL;
        n->r=NULL;
        heap.insert(n);
    }
    struct Node * root = buildhuffmantree(&heap);
    map<char,string> codes;
    buildCodes(root,codes);
    map<char,int> bincodes;
    for(auto i:codes){
        char *ptr;
        char str[i.second.length()];
        strcpy(str,i.second.c_str());
        long parsed = strtol(str,&ptr,2);
        bincodes[i.first] = parsed;
    }
    ofstream newfile;
    newfile.open(file.substr(0,file.find(".")) + "-compressed" + ".bin" , ios::out | ios::binary);
    string extention = file.substr(file.find("."));
    printcodesintofile(noofchars,m.size(),newfile,codes,extention);
    ifstream readfile(file);
    printtextintofile(readfile,newfile,codes);
    readfile.close();
    newfile.close();
}