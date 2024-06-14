#include <bits/stdc++.h>
using namespace std;

void extractcodes(int &noofchars,string &extention,ifstream &readfile,unordered_map<string,string> &m){
    int x;
    readfile>>x>>noofchars>>extention;
    string line;
    getline(readfile,line);
    for(int i=0;i<x;i++){
        getline(readfile,line);
        if(line[0]==' '){
            string key,value;
            key = line.substr(2);
            value = line[0]; 
            m[key] = value;
            continue;
        }
        string key,value;
        key = line.substr(line.find(' ')+1);
        value = line[0]; 
        m[key] = value;
    }
}

string dectobin(int decimal){
    string s="";
    for(int i = 7; i>=0; i--){
        if(decimal>>i & 1)
        {
            s.append("1");
        }
        else s.append("0");
    }
    return s;
}

void extractfile(int &noofchars,ifstream &readfile,ofstream &newfile,unordered_map<string,string> &m){
    char p;
    char *ptr;
    readfile.read(&p,1);
    string s = dectobin((int)p);
    int i = 0,count = 0,flag = 0,flag2=0;
    string x,towrite="";    
    while(count<noofchars){
        count++;
        flag = 0;
        if(flag2){
            towrite="";
            flag2=0;
        }
        for(int j=1;j<=s.length();j++){
            x = s.substr(0,j);
            if(m.find(x)!=m.end()){
                towrite = towrite.append(m[x]);
                readfile.read(&p,1);
                s = s.substr(j).append(dectobin((int)p));
                flag = 1;
                break;
            }
        }
        if(flag==0){
            s = s + dectobin((int)p);
        }
        if(count%10000==0){
            newfile.write(towrite.c_str(),10000);
            flag2=1;
        }
    }
    newfile.write(towrite.c_str(),noofchars%10000);
}

int main(int argc, char* argv[]){
    string file = argv[1];
    unordered_map<string,string> m;
    ifstream readfile(file,ios::binary);
    int noofchars;
    string extention;
    extractcodes(noofchars,extention,readfile,m);
    // for(auto i:m){
    //     cout<<i.first<<" "<<i.second<<endl;
    // }
    ofstream newfile(file.substr(0,file.find('-'))+"-decompressed"+extention);
    extractfile(noofchars,readfile,newfile,m);
    newfile.close();
    readfile.close();
}