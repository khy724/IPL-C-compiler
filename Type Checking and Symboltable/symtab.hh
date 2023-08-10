#include<bits/stdc++.h>
#include <string.h>
#include<map>
#include <string>
using namespace std;
class SymbTab;
class Entry{
    public:
    string varfun, global,ret_type;
    int size=0, offset=0;
    SymbTab* symbtab;

    Entry(string vf, string scope, string type,int s,int o){
        varfun = vf;
        global = scope;
        ret_type = type;
        size = s;
        offset = o;
    }
    

};

class SymbTab{
    public:
    map<string,Entry*> Entries;
    
    void printgst(){
        int count=0;
        cout<<"[";
        for (auto it = Entries.begin(); it != Entries.end(); ++it){
            count++;
            cout<<"[";
            if(it->second->varfun!="struct")
            cout<< "\""<<it->first<< "\", \""<<it->second->varfun<<"\", \""<<it->second->global<<"\", "<<it->second->size<<", "<<it->second->offset<<", \""<<it->second->ret_type<<"\""<<endl;
            else
            cout<< "\""<<it->first<< "\", \""<<it->second->varfun<<"\", \""<<it->second->global<<"\", "<<it->second->size<<", "<<"\"-\""<<", \""<<it->second->ret_type<<"\""<<endl;
            
            
            cout<<"]";
            if (next(it,1) != Entries.end()) 
            	cout << "," << endl;
            
        }
        cout<<endl<<"]"<<endl;

    }
    void print(){
        int count=0;
        cout<<"["<<endl;
        for (auto it = Entries.begin(); it != Entries.end(); ++it){
            count++;
            cout<<"[";
            cout<< "\""<<it->first<< "\", \""<<it->second->varfun<<"\", \""<<it->second->global<<"\", "<<it->second->size<<", "<<it->second->offset<<", \""<<it->second->ret_type<<"\""<<endl;
            

            
            cout<<"]"<<endl;
            if (next(it,1) != Entries.end()) 
            	cout << "," << endl;
            
        }
        cout<<"]"<<endl;
        
    }


};
