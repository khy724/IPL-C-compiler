
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdarg.h>
#include <iostream>
#include<bits/stdc++.h>
#include <string.h>
#include<map>
#include <string>
// #include "symtab.hh"
//#include "print.hh"
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
    map<string,string> Localstr;
    
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

class abstract_astnode
{
public:

virtual void print(int blanks) = 0;
virtual int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount)=0;
//enum typeExp astnode_type;

protected:
};

static void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
 {   
 	typedef vector<abstract_astnode *>* pv;
 	va_list args;
 	va_start(args, fmt);
 	if ((astname != NULL) && (astname[0] != '\0'))
 	{
 		cout << "{ ";
 		cout << "\"" << astname << "\"" << ": ";
 	}
 	cout << "{" << endl;
 	while (*fmt != '\0')
 	{
 		if (*fmt == 'a')
 		{
 			char * field = va_arg(args, char *);
 			abstract_astnode *a = va_arg(args, abstract_astnode *);
            if(a){
                cout << "\"" << field << "\": " << endl;
                
                a->print(0);
            }
            else{
                cout << "\"" << field << "\": empty" << endl;
            }
 		}
 		else if (*fmt == 's')
 		{
 			char * field = va_arg(args, char *);
 			char *str = va_arg(args, char *);
 			cout << "\"" << field << "\": ";

 			cout <<str << endl;
 		}
 		else if (*fmt == 'i')
 		{
 			char * field = va_arg(args, char *);
 			int i = va_arg(args, int);
 			cout << "\"" << field << "\": ";

 			cout << i;
 		}
 		else if (*fmt == 'f')
 		{
 			char * field = va_arg(args, char *);
 			double f = va_arg(args, double);
 			cout << "\"" << field << "\": ";
 			cout << f;
 		}
 		else if (*fmt == 'l')
 		{
 			char * field = va_arg(args, char *);
 			pv f =  va_arg(args, pv);
 			cout << "\"" << field << "\": ";
 			cout << "[" << endl;
 			for (int i = 0; i < (int)f->size(); ++i)
 			{
 				(*f)[i]->print(0);
 				if (i < (int)f->size() - 1)
 					cout << "," << endl;
 				else
 					cout << endl;
 			}
 			cout << endl;
 			cout << "]" << endl;
 		}
 		++fmt;
 		if (*fmt != '\0')
 			cout << "," << endl;
 	}
 	cout << "}" << endl;
 	if ((astname != NULL) && (astname[0] != '\0'))
 		cout << "}" << endl;
 	va_end(args);
 }



class statement_astnode : public abstract_astnode
{
public:
virtual void print(int blanks) = 0;
virtual int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount) = 0;
//enum typeExp statement_astnode_type;

protected:
};

class exp_astnode : public abstract_astnode
{
public:
string typeval;
bool iszero=false;
bool lvalue=true;
bool isparam=false;
bool isaddr=false;
int label=0;
string addr;
// exp_astnode* left;
// exp_astnode* right;
virtual void print(int blanks) = 0;
virtual int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount) = 0;
//enum typeExp exp_astnode_type;

protected:
};


class ref_astnode : public exp_astnode
{
public:
virtual void print(int blanks) = 0;
virtual int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount) = 0;
//enum typeExp ref_astnode_type;

protected:
};

class identifier_astnode : public ref_astnode
{
public:
string name;

identifier_astnode(string var){
    // name = "\""+var+"\"";
    name = var;
}
void print(int blanks){
    
    // printf("{\n\"identifier\": \"%s\"\n}\n",name.c_str());
    printAst("\0","s","identifier",name.c_str());
};

int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){

    int of = s->Entries[name]->offset;
    string l = s->Entries[name]->ret_type;
    int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    if(!(l.substr(0,6)=="struct" && l_c==0 && g->Entries[l]->size>4)){
        addr = to_string(of)+"(%ebp)";

        cout<<"\tmovl\t"<<of<<"(%ebp), "<<rstack.top()<<endl;
    }
    else{
        
        
        addr = to_string(of)+"(%ebp)";
    }
    // if(s->Entries[name]->global=="param"){
    // string new_l;
    // string l = s->Entries[name]->ret_type;
    // int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    //     if(count(l.begin(),l.end(),'(')==0){
    //         if(l[l.length()-1]=='*'){
    //                 new_l = l.substr(0,l.length()-1)+"(*)";
    //         }
    //         else {
    //                 size_t open_br, close_br;
    //                 open_br = l.find('[');
    //                 close_br = l.find(']');
    //                 new_l = l.substr(0,open_br)+"(*)"+l.substr(close_br+1,l.length()-close_br-1);
    //         }

    //     }
    //     else{
    //         new_l = l;
    //     }
    //     s->Entries[name]->ret_type = new_l;
    // }
    return Lcount;
}
// enum typeExp identifier_astnode_type;

protected:
};

class op_binary_astnode : public exp_astnode
{
public:
exp_astnode* left;
exp_astnode* right;
string op;

op_binary_astnode(exp_astnode* l, string o, exp_astnode* r){

    left = l;
    right = r;
    // char temp[]="\"";
    // char q[] = "\""; 
    // strcat(temp,o.c_str());
    // strcat(temp,q);
    op = o;

    // if(o[o.length()-1]!="?"){
    //     op = o;
    // }
    // else{
    //     string temp = o.substr(0,o.length()-1);
    //     strcat(temp,"_");
    //     strcat(temp,left.typeval);
    //     op = temp;
    // }
    
}

void print(int blanks){
    printAst("op_binary", "saa",
            "op", op.c_str(),
            "left", left, 
            "right", right
            );
}
//enum typeExp op_binary_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    // cout<<op<<endl;
    Lcount = left->gencode(s,g,rstack, Lcount);
    bool inmem=false;
    string reg = rstack.top();
    
    string l=left->typeval;
    string r = right->typeval;
    int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    int r_c = count(r.begin(),r.end(),'*') + count(r.begin(),r.end(),'[');
    
    if(rstack.size()>2){
        
        rstack.pop();
    }
    else{
        inmem=true;
        cout<<"\tpushl\t"<<rstack.top()<<endl;
        reg = "(%esp)";
    }
    Lcount = right->gencode(s,g,rstack, Lcount);
    
    if(op=="PLUS_INT"){
        
       
        if(l_c>0){
            cout<<"\timull\t$4, "<<rstack.top()<<endl;
        }
        cout<<"\taddl\t"<<rstack.top()<<", "<<reg<<endl;
    }
    else if(op=="MINUS_INT"){
       
        if(l_c>0 && r_c==0){
            cout<<"\timull\t$4, "<<rstack.top()<<endl;
        }
        cout<<"\tsubl\t"<<rstack.top()<<", "<<reg<<endl;

    }
    else if(op=="MULT_INT"){
        cout<<"\timull\t"<<rstack.top()<<", "<<reg<<endl;
    }
    else if(op=="OR_OP"){
        cout<<"\tcmpl\t$0, "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tjne\t.L"<<newl1<<endl;
        Lcount++;
        cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
        int newl2 = Lcount;
        cout<<"\tje\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl3 = Lcount;
        cout<<"\tjmp\t.L"<<newl3<<endl;
        Lcount++;
        cout<<".L"<<newl2<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl3<<":\n";

    } 
    else if(op=="AND_OP"){
        cout<<"\tcmpl\t$0, "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tje\t.L"<<newl1<<endl;
        Lcount++;
        cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
        cout<<"\tje\t.L"<<newl1<<endl;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="EQ_OP_INT"){
        cout<<"\tcmpl\t"<<reg<<", "<<rstack.top()<<endl;
        int newl1 = Lcount;
        cout<<"\tjne\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="NE_OP_INT"){
        cout<<"\tcmpl\t"<<rstack.top()<<", "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tje\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="LT_OP_INT"){
        cout<<"\tcmpl\t"<<rstack.top()<<", "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tjge\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";

    }
    else if(op=="GT_OP_INT"){
        cout<<"\tcmpl\t"<<rstack.top()<<", "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tjle\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="LE_OP_INT"){
        cout<<"\tcmpl\t"<<rstack.top()<<", "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tjg\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="GE_OP_INT"){
        cout<<"\tcmpl\t"<<rstack.top()<<", "<<reg<<endl;
        int newl1 = Lcount;
        cout<<"\tjl\t.L"<<Lcount<<endl;
        Lcount++;
        cout<<"\tmovl\t$1, "<<reg<<endl;
        int newl2 = Lcount;
        cout<<"\tjmp\t.L"<<newl2<<endl;
        Lcount++;
        cout<<".L"<<newl1<<":\n";
        cout<<"\tmovl\t$0, "<<reg<<endl;
        cout<<".L"<<newl2<<":\n";
    }
    else if(op=="DIV_INT"){
        bool edx = false;
        if(rstack.top()=="%edx") edx = true;
        if(reg!="%eax"){
            cout<<"\tpushl\t%eax"<<endl;}
        cout<<"\tpushl\t%edx"<<endl;
        if(edx){
            cout<<"\tmovl\t%edx, %esi"<<endl;

        }
        if(reg!="%eax"){
            cout<<"\tmovl\t"<<reg<<", %eax"<<endl;
        }
        cout<<"\tcltd"<<endl;
        if(edx){
             cout<<"\tidivl\t"<<"%esi"<<endl;
        }
        else{ 
            cout<<"\tidivl\t"<<rstack.top()<<endl;
        }
        if(reg!="%eax")
            cout<<"\tmovl\t%eax, "<<reg<<endl;
        cout<<"\tpopl\t%edx"<<endl;
        if(reg!="%eax")
            cout<<"\tpopl\t%eax"<<endl;
        // else
        //     cout<<"\addl\t$4, %esp"<<endl;
        
        // bool edx = false;
        
        // if(rstack.size()==5){

        // }
        // else if(rstack.size()==3){
        //     cout<<"\tpushl\t%eax"<<endl;
        //     cout<<"\tmovl\t%edx, %esi"<<endl;
        //     edx = true;
        //     rstack.pop();
        // }
        // else if(rstack.size()>=2){
        //     cout<<"\tpushl\t%eax"<<endl;
        //     if(inmem){
                
        //         cout<<"\tpushl\t%edx"<<endl;
        //     }
        // }
        // else{
        //     cout<<"\tpushl\t%eax"<<endl;
        //     cout<<"\tpushl\t%edx"<<endl;
        // }
        // if(reg!="%eax"){
        //     cout<<"\tmovl\t"<<reg<<", %eax"<<endl;
        // }
        // cout<<"\tcltd"<<endl;
        // cout<<"\tidivl\t"<<rstack.top()<<endl;
        // if(reg!="%eax"){
        //     cout<<"\tmovl\t%eax, "<<reg<<endl;
        // }
        // if(rstack.size()==5){
            
        // }
        // else if(edx){
        //     cout<<"\tpopl\t%edx"<<endl;
        //     cout<<"\tpopl\t%eax"<<endl;
        //     rstack.push("%edx");
        // }
        // else if(rstack.size()>=2){
        //     if(inmem){
        //         cout<<"\tpopl\t%edx"<<endl;
        //     }
        //     cout<<"\tpopl\t%eax"<<endl;
        // }
        // else{
            
        //     cout<<"\tpopl\t%edx"<<endl;
        //     cout<<"\tpopl\t%eax"<<endl;
        // }
        

    }
    
    if(!inmem){
        rstack.push(reg);
    }
    else{
        cout<<"\tpopl\t"<<rstack.top()<<endl;
    }
    
    
    // addr = rstack.top();
    return Lcount;
}
protected:
};

class op_unary_astnode : public exp_astnode
{
public:
string op;
exp_astnode* unary_exp;

op_unary_astnode(int o, exp_astnode* unary_e){
    this->typeval = unary_e->typeval;
	if(o==1){
		op="UMINUS";
	}
	else if(o==2){
		op = "NOT";
	}
	else if(o==3){
		op = "ADDRESS";
	}
	else if(o==4){
		op = "DEREF";
	}
	else if(o==5){
		op = "PP";
	}
    else if (o==6){
        op = "TO_FLOAT";
        this->typeval = "float";

    }
    else if(o==7){
        op="TO_INT";
        this->typeval = "int";
    }
    // op = o;
    // op = "+op+"\"";
    unary_exp = unary_e;
}

void print(int blanks){
    printAst("op_unary", "sa",
            "op", op.c_str(),
            "child", unary_exp
            );
};
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    string l=unary_exp->typeval;
    int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    
    if (op=="UMINUS"){
        Lcount = unary_exp->gencode(s,g,rstack,Lcount);
        cout<<"\tnegl\t"<<rstack.top()<<endl;
    }
    else if(op=="NOT"){
        Lcount = unary_exp->gencode(s,g,rstack,Lcount);
        cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
        cout<<"\tjne\t.L"<<Lcount<<endl;
        int newl1=Lcount++;
        cout<<"\tmovl\t$1, "<<rstack.top()<<endl;
        cout<<"\tjmp\t.L"<<Lcount<<endl;
        int newl2=Lcount++;
        cout<<".L"<<newl1<<":"<<endl;
        cout<<"\tmovl\t$0, "<<rstack.top()<<endl;
        cout<<".L"<<newl2<<":"<<endl;
        
    }
    else if(op=="PP"){
        Lcount = unary_exp->gencode(s,g,rstack,Lcount);
        if(l_c==0){
        cout<<"\taddl\t$1, "<<unary_exp->addr<<endl;
        }
        else{
            cout<<"\taddl\t$4, "<<unary_exp->addr<<endl;
        }
    }
    else if(op=="DEREF"){
        Lcount = unary_exp->gencode(s,g,rstack,Lcount);
        // ERROR HANDLING
        bool inmem=false;
        string value_reg = rstack.top();
        rstack.pop();
        string l=unary_exp->typeval;
        int strc = count(l.begin(),l.end(),'*');
        if(!unary_exp->isaddr){
            cout<<"\tmovl\t"<<value_reg<<", "<<rstack.top()<<endl;
            
        }
        else{
            cout<<"\tleal\t"<<unary_exp->addr<<", "<<rstack.top()<<endl;
            cout<<"\tleal\t"<<unary_exp->addr<<", "<<value_reg<<endl;
            
        }
        this->addr = "("+rstack.top()+")";
       
            rstack.push(value_reg);
        
        cout<<"\tmovl\t("<<rstack.top()<<"), "<<rstack.top()<<endl;
        
    }
    else if(op=="ADDRESS"){
        Lcount = unary_exp->gencode(s,g,rstack,Lcount);
        if(unary_exp->isaddr){
        cout<<"\tleal\t"<<unary_exp->addr<<", "<<rstack.top()<<endl;
        }
        else{
            
            cout<<"\tmovl\t"<<unary_exp->addr<<", "<<rstack.top()<<endl;
        }
        this->addr = rstack.top();

    }
    return Lcount;

}
//enum typeExp op_unary_astnode_type;

protected:
};

class assignE_astnode : public exp_astnode
{
public:
exp_astnode* left;
exp_astnode* right;
assignE_astnode(exp_astnode*a, exp_astnode*b){
    left = a;
    right= b;
}
void print(int blanks){
    printAst("assignE","aa",
    "left",left,
    "right",right);
};
//enum typeExp assignE_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    string l = left->typeval;
    int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    Lcount = right->gencode(s,g, rstack, Lcount);
        
    if(!(l_c==0 && l.substr(0,6)=="struct" && g->Entries[l]->size>4)){
        bool inmem = false;
        string reg;
        if(rstack.size()>2){
            reg = rstack.top();
            rstack.pop();
        }
        else{
            inmem = true;
            cout<<"\tpushl\t"<<rstack.top()<<endl;
            reg = "(%esp)";
        }
        
    
        Lcount = left->gencode(s,g,rstack,Lcount);
        cout<<"\tmovl\t"<<reg<<", "<<rstack.top()<<endl;
        cout<<"\tmovl\t"<<rstack.top()<<", "<<left->addr<<endl;
        if(!inmem){
            rstack.push(reg);
        }
        else{
            cout<<"\taddl\t$4, %esp"<<endl;
        }
    }
    else{
        // cout<<"here"<<endl;
        bool inmem = false;
        string reg;
        if(rstack.size()>2){
            reg = rstack.top();
            rstack.pop();
        }
        else{
            inmem = true;
            cout<<"\tpushl\t"<<rstack.top()<<endl;
            reg = "(%esp)";
        }
        
        
            if(right->isaddr){
            cout<<"\tleal\t"<<right->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<right->addr<<", "<<rstack.top()<<endl;
            }
            
            string reg1 = rstack.top();
            rstack.pop();
            Lcount = left->gencode(s,g,rstack,Lcount);
            if(left->isaddr){
                cout<<"\tleal\t"<<left->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<left->addr<<", "<<rstack.top()<<endl;
            }
            for(auto it=g->Entries[l]->symbtab->Entries.begin();it!=g->Entries[l]->symbtab->Entries.end();++it){
                
                
                
                    cout<<"\taddl\t$"<<it->second->offset<<", "<<reg1<<endl;
                    cout<<"\taddl\t$"<<it->second->offset<<", "<<rstack.top()<<endl;
                    cout<<"\tmovl\t("<<reg1<<"), "<<reg1<<endl;
                    cout<<"\tmovl\t"<<reg1<<", ("<<rstack.top()<<")"<<endl;
                    if(right->isaddr){
                        cout<<"here1"<<endl;
                        cout<<"\tleal\t"<<right->addr<<", "<<reg1<<endl;
                        }
                        else{
                            cout<<"\tmovl\t"<<right->addr<<", "<<reg1<<endl;
                    }
                
            }
        
        
        if(!inmem){
            rstack.push(reg);
        }
        else{
            cout<<"\taddl\t$4, %esp"<<endl;
        }
        if(right->addr=="%esp"){
            cout<<"\taddl\t$"<<g->Entries[right->typeval]->size<<", %esp"<<endl;
        }
        
    }
    
    return Lcount;
}
protected:
};

class funcall_astnode : public exp_astnode
{
public:
vector<exp_astnode*> list;
identifier_astnode* name;
funcall_astnode(vector<exp_astnode*>a){
    list = a;
}
funcall_astnode(string a, vector<exp_astnode*> b){
    name = new identifier_astnode(a);
    list=b;
    
}
funcall_astnode(string a){
    name = new identifier_astnode(a);
}
// void add_exp(vector<exp_astnode*>a){
//     list = 
// }
void print(int blanks){
   // printf("funcall astnode\n");
    printAst("funcall","al",
            "fname",name,
            "params",list
            );
};
//enum typeExp funcall_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    
    int caller_push=0;
    
    caller_push=6 - rstack.size();
    if(g->Entries[name->name]->ret_type.substr(0,6)=="struct" && g->Entries[g->Entries[name->name]->ret_type]->size>4){
        this->isaddr = false;
        cout<<"\tsubl\t$"<<g->Entries[g->Entries[name->name]->ret_type]->size<<", %esp"<<endl;
        cout<<"\tpushl\t%eax"<<endl;
        cout<<"\tpushl\t%ebx"<<endl;
        cout<<"\tpushl\t%ecx"<<endl;
         cout<<"\tpushl\t%edx"<<endl;
         cout<<"\tpushl\t%esi"<<endl;
         cout<<"\tpushl\t%edi"<<endl;
    }
    else{
    for(int i=0;i<caller_push;i++){
        if(i==0){
            cout<<"\tpushl\t%eax"<<endl;
        }
        else if(i==1){
            cout<<"\tpushl\t%ebx"<<endl;
        }
        else if(i==2){
            cout<<"\tpushl\t%ecx"<<endl;
        }
        else if(i==3){
            cout<<"\tpushl\t%edx"<<endl;
        }
        else if(i==4){
            cout<<"\tpushl\t%esi"<<endl;
        }
        else{
            cout<<"\tpushl\t%edi"<<endl;
        }
        
    }
    }
    
    
    //push params
   
    for(auto it=list.begin();it!=list.end();++it){
        
        // string new_l;
        string l = (*it)->typeval;
      
        // int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
        // if(count(l.begin(),l.end(),'(')==0){
        //     if(l[l.length()-1]=='*'){
        //             new_l = l.substr(0,l.length()-1)+"(*)";
        //     }
        //     else {
        //             size_t open_br, close_br;
        //             open_br = l.find('[');
        //             close_br = l.find(']');
        //             new_l = l.substr(0,open_br)+"(*)"+l.substr(close_br+1,l.length()-close_br-1);
        //     }

        // }
        //     else{
        //         new_l = l;
        //     }
        //     (*it)->typeval = new_l;
            // cout<<new_l<<endl;
        // int l_c = count(l.begin(),l.end(),'(') + count(l.begin(),l.end(),'[');
        // cout<<l<<endl;
        
        
        int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
        if(l.substr(0,6)=="struct" && l_c==0 && g->Entries[l]->size>4){
            // cout<<g->Entries[l]->size<<endl;
            Lcount = (*it)->gencode(s,g,rstack,Lcount);
            if((*it)->isaddr){
            cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            // string reg1 = rstack.top();
            // rstack.pop();
            for(auto it1=g->Entries[l]->symbtab->Entries.rbegin();it1!=g->Entries[l]->symbtab->Entries.rend();++it1){
                
                    // cout<<"\taddl\t$"<<it->second->offset<<", "<<reg1<<endl;
                    cout<<"\taddl\t$"<<it1->second->offset<<", "<<rstack.top()<<endl;
                    cout<<"\tmovl\t("<<rstack.top()<<"), "<<rstack.top()<<endl;
                    cout<<"\tpushl\t"<<rstack.top()<<endl;
                    if((*it)->isaddr){
                        cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
                        }
                        else{
                            cout<<"\tmovl\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
                        }
                
            }
            
            
        }
        else{
            Lcount = (*it)->gencode(s,g,rstack,Lcount);
            if((*it)->isaddr && l_c!=0){
                cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            
            cout<<"\tpushl\t"<<rstack.top()<<endl;
        }

    }
    
    //call f
    
    cout<<"\tcall\t"<<name->name<<endl;
    
    //pop params
    int param_sp = 0;
    for (auto it1 = g->Entries[name->name]->symbtab->Entries.begin(); it1 != g->Entries[name->name]->symbtab->Entries.end(); ++it1){
        if(it1->second->global=="param"){
            string l = it1->second->ret_type;
            int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
            if((it1->second->ret_type).substr(0,6)=="struct" &&l_c==0 && g->Entries[it1->second->ret_type]->size>4){
            param_sp+=it1->second->size;
            }
            else{
                param_sp+=4;
            }
        }
    }
    this->addr = "%esp";
    cout<<"\taddl\t$"<<param_sp<<", %esp"<<endl;
    if(rstack.top()!="%eax"){
    cout<<"\tmovl\t%eax, "<<rstack.top()<<endl;
    }
    if(g->Entries[name->name]->ret_type.substr(0,6)=="struct" && g->Entries[g->Entries[name->name]->ret_type]->size>4){
        
        cout<<"\tpopl\t%edi"<<endl;
        cout<<"\tpopl\t%esi"<<endl;
        cout<<"\tpopl\t%edx"<<endl;
         cout<<"\tpopl\t%ecx"<<endl;
         cout<<"\tpopl\t%ebx"<<endl;
         cout<<"\tpopl\t%eax"<<endl;
    }
    else{
    for(int i=caller_push-1;i>=0;i--){
        if(i==0){
            
            cout<<"\tpopl\t%eax"<<endl;
        }
        else if(i==1){
            
            cout<<"\tpopl\t%ebx"<<endl;
        }
        else if(i==2){
            cout<<"\tpopl\t%ecx"<<endl;
        }
        else if(i==3){
            cout<<"\tpopl\t%edx"<<endl;
        }
        else if(i==4){
            cout<<"\tpopl\t%esi"<<endl;
        }
        else{
            cout<<"\tpopl\t%edi"<<endl;
        }
        
    }
    }
        

    return Lcount;
}
protected:
};

class intconst_astnode : public exp_astnode
{
public:
int val;
intconst_astnode(string a){
    val = atoi(a.c_str());
}
void print(int blanks){
    printAst("\0","i","intconst",val);
    
};
//enum typeExp intconst_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    cout<<"\tmovl\t$"<<val<<","<<rstack.top()<<endl;
    return Lcount;
}
protected:
};

class floatconst_astnode : public exp_astnode
{
public:
double val;
floatconst_astnode(string a){
    char* temp;
    val = strtod(a.c_str(),&temp);

}
void print(int blanks){
    printAst("\0","f","floatconst",val);

}
//enum typeExp floatconst_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    return Lcount;
    
}
protected:
};

class stringconst_astnode : public exp_astnode
{
public:
string val;
stringconst_astnode(string a){
    val = a;
}
void print(int blanks){
    printAst("\0","s","stringconst",val.c_str());
    
};
//enum typeExp stringconst_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    return Lcount;
    
}
protected:
};


class empty_astnode : public statement_astnode
{
public:
void print(int blanks){
    //printAst("\0","s","","\"empty\"");
    printf("\"empty\"\n");
};
// enum typeExp empty_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    // cout<<"\t nop"<<endl;
    return Lcount;
}
protected:
};

class seq_astnode : public statement_astnode
{
public:
vector<statement_astnode*> list;
seq_astnode(){

}
seq_astnode(statement_astnode* a){
    list.push_back(a);
}
void add_statement(statement_astnode* a){
    list.push_back(a);
}
void print(int blanks){
    //printf("inside parameter list class\n");
    printAst("\0","l","seq",list);
};
// enum typeExp seq_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    
    
    for(auto it=this->list.begin(); it!=this->list.end(); ++it){
        Lcount = (*it)->gencode(s,g,rstack, Lcount);
    }
    
    return Lcount;

}
protected:
};

class assignS_astnode : public statement_astnode
{
public:
exp_astnode* left;
exp_astnode* right;
assignS_astnode(assignE_astnode* a){
    left = (*a).left;
    right = (*a).right;
}
void print(int blanks){
    printAst("assignS","aa",
    "left",left,
    "right",right);
}
// enum typeExp assignS_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    string l = left->typeval;
    int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
    Lcount = right->gencode(s,g, rstack, Lcount);
        
    if(!(l_c==0 && l.substr(0,6)=="struct" && g->Entries[l]->size>4)){
        bool inmem = false;
        string reg;
        if(rstack.size()>2){
            reg = rstack.top();
            rstack.pop();
        }
        else{
            inmem = true;
            cout<<"\tpushl\t"<<rstack.top()<<endl;
            reg = "(%esp)";
        }
        
    
        Lcount = left->gencode(s,g,rstack,Lcount);
        cout<<"\tmovl\t"<<reg<<", "<<rstack.top()<<endl;
        cout<<"\tmovl\t"<<rstack.top()<<", "<<left->addr<<endl;
        if(!inmem){
            rstack.push(reg);
        }
        else{
            cout<<"\taddl\t$4, %esp"<<endl;
        }
    }
    else{
        // cout<<"here"<<endl;
        bool inmem = false;
        string reg;
        if(rstack.size()>2){
            reg = rstack.top();
            rstack.pop();
        }
        else{
            inmem = true;
            cout<<"\tpushl\t"<<rstack.top()<<endl;
            reg = "(%esp)";
        }
        
        
            if(right->isaddr){
               
            cout<<"\tleal\t"<<right->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<right->addr<<", "<<rstack.top()<<endl;
            }
            
            string reg1 = rstack.top();
            rstack.pop();
            Lcount = left->gencode(s,g,rstack,Lcount);
            if(left->isaddr){
                cout<<"\tleal\t"<<left->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<left->addr<<", "<<rstack.top()<<endl;
            }
            for(auto it=g->Entries[l]->symbtab->Entries.begin();it!=g->Entries[l]->symbtab->Entries.end();++it){
                
                
               
                    cout<<"\taddl\t$"<<it->second->offset<<", "<<reg1<<endl;
                    cout<<"\taddl\t$"<<it->second->offset<<", "<<rstack.top()<<endl;
                    cout<<"\tmovl\t("<<reg1<<"), "<<reg1<<endl;
                    cout<<"\tmovl\t"<<reg1<<", ("<<rstack.top()<<")"<<endl;
                    if(right->isaddr){
                        cout<<"\tleal\t"<<right->addr<<", "<<reg1<<endl;
                        }
                        else{
                            cout<<"\tmovl\t"<<right->addr<<", "<<reg1<<endl;
                    }
                    
                
            }
        
        
        if(!inmem){
            rstack.push(reg);
        }
        else{
            cout<<"\taddl\t$4, %esp"<<endl;
        }
        if(right->addr=="%esp"){
            cout<<"\taddl\t$"<<g->Entries[right->typeval]->size<<", %esp"<<endl;
        }
        
    }
    
    return Lcount;
    
}
protected:
};

class return_astnode : public statement_astnode
{
public:
exp_astnode* ret_val;
string fname;
return_astnode(exp_astnode* a, string fn){
    ret_val=a;
    fname = fn;
}
void print(int blanks){
    printAst("\0","a",
    "return",ret_val);
};
// enum typeExp return_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    //set retval
    
    
    string l = ret_val->typeval;

    if(l.substr(0,6)=="struct" && g->Entries[l]->size>4){
        Lcount = ret_val->gencode(s,g,rstack,Lcount);
        int param_sz=0;
        for(auto it=s->Entries.rbegin();it!=s->Entries.rend();++it){
            if(it->second->global=="param")
            param_sz+= it->second->size;
        }
        if(ret_val->isaddr){
            cout<<"\tleal\t"<<ret_val->addr<<", "<<rstack.top()<<endl;
        }
        else{
            cout<<"\tmovl\t"<<ret_val->addr<<", "<<rstack.top()<<endl;
        }
        string reg = rstack.top();
        rstack.pop();
        // cout<<"\tleal\t"<<(param_sz+32)<<"(%ebp), "<<rstack.top()<<endl;
        for(auto it=g->Entries[l]->symbtab->Entries.rbegin();it!=g->Entries[l]->symbtab->Entries.rend();++it){
            // cout<<"heree"<<endl;
            

                    // cout<<"\taddl\t$"<<it->second->offset<<", "<<reg1<<endl;
                    cout<<"\taddl\t$"<<it->second->offset<<", "<<reg<<endl;
                    cout<<"\tmovl\t("<<reg<<"), "<<reg<<endl;
                    cout<<"\tmovl\t"<<reg<<", "<<param_sz+32+it->second->offset<<"(%ebp)"<<endl;
                    
                        if(ret_val->isaddr){
                            cout<<"\tleal\t"<<ret_val->addr<<", "<<reg<<endl;
                        }
                        else{
                            cout<<"\tmovl\t"<<ret_val->addr<<", "<<reg<<endl;
                        }
                 
                    
                

        }
        rstack.push(reg);
    }
    else{
        Lcount = ret_val->gencode(s,g,rstack,Lcount);
        if(rstack.top()!="%eax"){
            cout<<"\tmovl\t"<<rstack.top()<<", %eax"<<endl;
        }
    }
    cout<<"\tleave"<<endl;
    cout<<"\tret"<<endl;

    return Lcount;
    
}
protected:
};

class if_astnode : public statement_astnode
{
public:
exp_astnode* cond=NULL;
statement_astnode* tr=NULL;
statement_astnode* fa=NULL;
if_astnode(exp_astnode* a, statement_astnode* b, statement_astnode* c){
    cond = a;
    tr = b;
    fa = c;
}
void print(int blanks){
    printAst("if","aaa",
    "cond",cond,
    "then",tr,
    "else",fa);
};
// enum typeExp if_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    Lcount = cond->gencode(s,g,rstack,Lcount);
    cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
    cout<<"\tje\t.L"<<Lcount<<endl;
    int newl1 = Lcount++;
    Lcount = tr->gencode(s,g,rstack,Lcount);
    cout<<"\tjmp\t.L"<<Lcount<<endl;
    int newl2 = Lcount++;
    cout<<".L"<<newl1<<":\n";
    Lcount = fa->gencode(s,g,rstack,Lcount);
    cout<<".L"<<newl2<<":\n";
    return Lcount;
    
}
protected:
};

class while_astnode : public statement_astnode
{
    public:
    exp_astnode* left=NULL;
    statement_astnode* right=NULL;
    while_astnode(exp_astnode* exp,statement_astnode* stmt){
        left = exp;
        right = stmt;
    }
    void print(int blanks){
        printAst("while","aa",
        "cond",left,
        "stmt",right);
    }
    // enum typeExp while_astnode_type;
    int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
        int newl1 = Lcount++;
        cout<<".L"<<newl1<<":\n";
        Lcount = left->gencode(s,g,rstack,Lcount);
        cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
        cout<<"\tje\t.L"<<Lcount<<endl;
        int newl2 = Lcount++;
        Lcount = right->gencode(s,g,rstack,Lcount);
        cout<<"\tjmp\t.L"<<newl1<<endl;
        cout<<".L"<<newl2<<":\n";
        return Lcount;
    
}
    protected:
};

class for_astnode : public statement_astnode
{
public:
exp_astnode* init=NULL;
exp_astnode* cond=NULL;
exp_astnode* assign=NULL;
statement_astnode* stmt=NULL;
for_astnode(exp_astnode* a,exp_astnode*b,exp_astnode*c,statement_astnode*s){
    init = a;
    cond = b;
    assign = c;
    stmt = s;
}
void print(int blanks){
    printAst("for","aaaa",
    "init",init,
    "guard",cond,
    "step",assign,
    "body",stmt);
}
// enum typeExp for_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    Lcount = init->gencode(s,g,rstack,Lcount);
    int newl1 = Lcount++;
    cout<<".L"<<newl1<<":\n";
    Lcount = cond->gencode(s,g,rstack,Lcount);
    cout<<"\tcmpl\t$0, "<<rstack.top()<<endl;
    cout<<"\tje\t.L"<<Lcount<<endl;
    int newl2 = Lcount++;
    Lcount = stmt->gencode(s,g,rstack,Lcount);
    Lcount = assign->gencode(s,g,rstack,Lcount);
    cout<<"\tjmp\t.L"<<newl1<<endl;
    cout<<".L"<<newl2<<":\n";
    return Lcount;
    
}
protected:
};

class proccall_astnode : public statement_astnode
{
public:
identifier_astnode* name;
// funcall_astnode* exp_list=NULL;
vector<exp_astnode*> exp_list;
proccall_astnode(string a){
    name = new identifier_astnode(a);
    
}
proccall_astnode(string a,vector<exp_astnode*> b){
    name = new identifier_astnode(a);
    exp_list = b;
}
void print(int blanks){
    printAst("proccall","al",
            "fname",name,
            "params",exp_list);
};
// enum typeExp proccall_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    int caller_push=0;
    
    caller_push=6 - rstack.size();
    if(g->Entries[name->name]->ret_type.substr(0,6)=="struct" && g->Entries[g->Entries[name->name]->ret_type]->size>4){
        cout<<"\tsubl\t$"<<g->Entries[g->Entries[name->name]->ret_type]->size<<", %esp"<<endl;
        cout<<"\tpushl\t%eax"<<endl;
        cout<<"\tpushl\t%ebx"<<endl;
        cout<<"\tpushl\t%ecx"<<endl;
         cout<<"\tpushl\t%edx"<<endl;
         cout<<"\tpushl\t%esi"<<endl;
         cout<<"\tpushl\t%edi"<<endl;
    }
    else{
    for(int i=0;i<caller_push;i++){
        if(i==0){
            cout<<"\tpushl\t%eax"<<endl;
        }
        else if(i==1){
            cout<<"\tpushl\t%ebx"<<endl;
        }
        else if(i==2){
            cout<<"\tpushl\t%ecx"<<endl;
        }
        else if(i==3){
            cout<<"\tpushl\t%edx"<<endl;
        }
        else if(i==4){
            cout<<"\tpushl\t%esi"<<endl;
        }
        else{
            cout<<"\tpushl\t%edi"<<endl;
        }
        
    }
    }
    
    
    //push params
   
    for(auto it=exp_list.begin();it!=exp_list.end();++it){
        
        // string new_l;
        string l = (*it)->typeval;
      
        // int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
        // if(count(l.begin(),l.end(),'(')==0){
        //     if(l[l.length()-1]=='*'){
        //             new_l = l.substr(0,l.length()-1)+"(*)";
        //     }
        //     else {
        //             size_t open_br, close_br;
        //             open_br = l.find('[');
        //             close_br = l.find(']');
        //             new_l = l.substr(0,open_br)+"(*)"+l.substr(close_br+1,l.length()-close_br-1);
        //     }

        // }
        //     else{
        //         new_l = l;
        //     }
        //     (*it)->typeval = new_l;
            // cout<<new_l<<endl;
        // int l_c = count(l.begin(),l.end(),'(') + count(l.begin(),l.end(),'[');
        // cout<<l<<endl;
        
        
        int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
        if(l.substr(0,6)=="struct" && l_c==0 && g->Entries[l]->size>4){
            // cout<<g->Entries[l]->size<<endl;
            Lcount = (*it)->gencode(s,g,rstack,Lcount);
            if((*it)->isaddr){
            cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            else{
                cout<<"\tmovl\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            // string reg1 = rstack.top();
            // rstack.pop();
            for(auto it1=g->Entries[l]->symbtab->Entries.rbegin();it1!=g->Entries[l]->symbtab->Entries.rend();++it1){
                
                    // cout<<"\taddl\t$"<<it->second->offset<<", "<<reg1<<endl;
                    cout<<"\taddl\t$"<<it1->second->offset<<", "<<rstack.top()<<endl;
                    cout<<"\tmovl\t("<<rstack.top()<<"), "<<rstack.top()<<endl;
                    cout<<"\tpushl\t"<<rstack.top()<<endl;
                    if((*it)->isaddr){
                        cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
                        }
                        else{
                            cout<<"\tmovl\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
                        }

                
            }
            
            // this->isaddr = false;
        }
        else{
            Lcount = (*it)->gencode(s,g,rstack,Lcount);
            if((*it)->isaddr && l_c!=0){
                cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
            }
            
            cout<<"\tpushl\t"<<rstack.top()<<endl;
        }

    }
    
    //call f
    
    cout<<"\tcall\t"<<name->name<<endl;
    
    //pop params
    int param_sp = 0;
    for (auto it1 = g->Entries[name->name]->symbtab->Entries.begin(); it1 != g->Entries[name->name]->symbtab->Entries.end(); ++it1){
        if(it1->second->global=="param"){
            string l = it1->second->ret_type;
            int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
            if((it1->second->ret_type).substr(0,6)=="struct" &&l_c==0 && g->Entries[it1->second->ret_type]->size>4){
            param_sp+=it1->second->size;
            }
            else{
                param_sp+=4;
            }
        }
    }
    // this->addr = "%esp";
    cout<<"\taddl\t$"<<param_sp<<", %esp"<<endl;
    if(rstack.top()!="%eax"){
    cout<<"\tmovl\t%eax, "<<rstack.top()<<endl;
    }
    if(g->Entries[name->name]->ret_type.substr(0,6)=="struct" && g->Entries[g->Entries[name->name]->ret_type]->size>4){
        
        cout<<"\tpopl\t%edi"<<endl;
        cout<<"\tpopl\t%esi"<<endl;
        cout<<"\tpopl\t%edx"<<endl;
         cout<<"\tpopl\t%ecx"<<endl;
         cout<<"\tpopl\t%ebx"<<endl;
         cout<<"\tpopl\t%eax"<<endl;
    }
    else{
    for(int i=caller_push-1;i>=0;i--){
        if(i==0){
            
            cout<<"\tpopl\t%eax"<<endl;
        }
        else if(i==1){
            
            cout<<"\tpopl\t%ebx"<<endl;
        }
        else if(i==2){
            cout<<"\tpopl\t%ecx"<<endl;
        }
        else if(i==3){
            cout<<"\tpopl\t%edx"<<endl;
        }
        else if(i==4){
            cout<<"\tpopl\t%esi"<<endl;
        }
        else{
            cout<<"\tpopl\t%edi"<<endl;
        }
        
    }
    }
        

    return Lcount;

}
protected:
};

class printf_astnode : public statement_astnode
{
public:
// identifier_astnode* name;
// funcall_astnode* exp_list=NULL;
vector<exp_astnode*> exp_list;
string name;
printf_astnode(string str){
    // name = new identifier_astnode(a);
    name=str;
}
printf_astnode(string a,vector<exp_astnode*> b){
    name = a;
    exp_list = b;
}
void print(int blanks){
    // printAst("proccall","al",
    //         "fname",name,
    //         "params",exp_list);
};
// enum typeExp proccall_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    //push params
    for(auto it=exp_list.rbegin();it!=exp_list.rend();++it){
        Lcount = (*it)->gencode(s,g,rstack, Lcount);
        cout<<"\tpushl\t"<<rstack.top()<<endl;
    }
    // for(auto it=exp_list.begin();it!=exp_list.end();++it){
    //     Lcount = (*it)->gencode(s,g,rstack,Lcount);
    //     string l = (*it)->typeval;
    //     int l_c = count(l.begin(),l.end(),'(') + count(l.begin(),l.end(),'[');
    //     // cout<<l<<endl;
    //     if(l_c!=0){
    //         cout<<"\tleal\t"<<(*it)->addr<<", "<<rstack.top()<<endl;
    //     }
    //     cout<<"\tpushl\t"<<rstack.top()<<endl;

    // }
    //push string
    
    cout<<"\tpushl\t$."<<s->Localstr[name]<<endl;
    cout<<"\tcall\tprintf"<<endl;
    cout<<"\tpopl\t"<<rstack.top()<<endl;
    //pop params
    for(auto it=exp_list.begin();it!=exp_list.end();++it){
        cout<<"\tpopl\t"<<rstack.top()<<endl;
    }
    // cout<<"\taddl	$12, %esp"<<endl;
    return Lcount;

}
protected:
};



class arrayref_astnode : public ref_astnode
{
public:
exp_astnode* left;
exp_astnode* right;
map<string,int> *size_map;

arrayref_astnode(exp_astnode* a, exp_astnode* b,map<string,int> *c){
    left = a;
    right = b;
    size_map = c;
}
int size_oft(string type, SymbTab* s, SymbTab* g){
    
    size_t sp = type.find(' ');
    int size=1;
    if(sp==string::npos){
        size_t l = type.find('[');
        size_t r = type.find(']');
        while(l!=string::npos){
            
            string num = type.substr(l+1,r-l-1);
            int s = stoi(num);
            size = size*s;
            type = type.substr(0,l)+type.substr(r+1,type.length()-r-1);
            l = type.find('[');
            r = type.find(']');
        }
        size = size*4;

    }
    else{
        //struct
        size_t l = type.find('[');
        size_t r = type.find(']');
        bool isarr=false;
        while(l!=string::npos){
            
            string num = type.substr(l+1,r-l-1);
            int s = stoi(num);
            size = size*s;
            type = type.substr(0,l)+type.substr(r+1,type.length()-r-1);
            l = type.find('[');
            r = type.find(']');
        }
        size_t ptrpos = type.find('*');
        if(ptrpos!=string::npos){
            type = type.substr(0,ptrpos);
        }
        size = size*g->Entries[type]->size;
        
    }
    return size;
}
void print(int blanks){
    printAst("arrayref","aa",
            "array",left,
            "index",right);
};
// enum typeExp arrayref_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    bool ispush=false;
    if(rstack.size()<3){
        cout<<"\tpushl\t%edx"<<endl;
        rstack.push("%edx");
        ispush=true;
    }
    Lcount = left->gencode(s,g,rstack,Lcount);
    
    string reg = rstack.top();
    rstack.pop();
    string reg1 = rstack.top();
    rstack.pop();
    bool pushreg1=false, pushreg=false;
    
    Lcount = right->gencode(s,g,rstack,Lcount);
    int size = size_oft(this->typeval,s,g);
    // for(auto it=size_map->begin();it!=size_map->end();++it){
    //     cout<<it->first<<" "<<it->second<<endl;
    // }
    string l=left->typeval;
    int strc = count(l.begin(),l.end(),'*');
    cout<<"\timull\t$"<<size<<", "<<rstack.top()<<endl;
    // cout<<l<<endl;
    if(left->isaddr){
        string ad =left->addr;
        // if(ad[0]=='('){
        //     ad = ad.substr(1,ad.length()-2);
        // }
        
        cout<<"\tleal\t"<<ad<<", "<<reg<<endl;
        
    }
    else{
        string ad =left->addr;
        if(ad[0]=='('){
            ad = ad.substr(1,ad.length()-2);
        }
        cout<<"\tmovl\t"<<ad<<", "<<reg<<endl;
    }
    cout<<"\taddl\t"<<rstack.top()<<", "<<reg<<endl;
    cout<<"\tmovl\t"<<reg<<", "<<reg1<<endl;
    cout<<"\tmovl\t("<<reg<<"), "<<reg<<endl;
    // this->addr ="("+rstack.top()+")";
   this->addr = "("+reg1+")";

    rstack.push(reg1);
    rstack.push(reg);
    if(ispush){
        cout<<"\tmovl\t%esi, %edi"<<endl;
        cout<<"\tmovl\t%edx, %esi"<<endl;
        cout<<"\tpopl\t%edx"<<endl;
    }


// no segfault printf

    // Lcount = left->gencode(s,g,rstack,Lcount);
    // string reg = rstack.top();
    // rstack.pop();
    // Lcount = right->gencode(s,g,rstack,Lcount);
    // int size = (*size_map)[this->typeval];
    // cout<<"\timull\t$"<<size<<", "<<rstack.top()<<endl;
    // cout<<"\taddl\t"<<left->addr<<", "<<rstack.top()<<endl;
    // this->addr =rstack.top();
    
    // cout<<"\tmovl\t("<<rstack.top()<<"), "<<reg<<endl;
    // rstack.push(reg);


    return Lcount;
    
}
protected:
};
class member_astnode : public ref_astnode
{
public:
exp_astnode* left;
identifier_astnode* member;
member_astnode(exp_astnode* a,string b){
    left = a;
    member = new identifier_astnode(b);
    this->isaddr = a->isaddr;
}
void print(int blanks){
    printAst("member", "aa",
            "struct", left, 
            "field", member
            );
};
// enum typeExp member_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
    Lcount = left->gencode(s,g,rstack,Lcount);

    int off = g->Entries[left->typeval]->symbtab->Entries[member->name]->offset;
    if(left->isaddr){
        cout<<"\tleal\t"<<left->addr<<", "<<rstack.top()<<endl;
        }
    else{
        cout<<"\tmovl\t"<<left->addr<<", "<<rstack.top()<<endl;
    }
    cout<<"\taddl\t$"<<off<<", "<<rstack.top()<<endl;
    string reg = rstack.top();
    rstack.pop();
    cout<<"\tmovl\t"<<reg<<", "<<rstack.top()<<endl;
    this->addr = "("+rstack.top()+")";
    rstack.push(reg);
    cout<<"\tmovl\t("<<reg<<"), "<<reg<<endl;
    this->isaddr = left->isaddr;
    if(left->addr=="%esp"){
        cout<<"\taddl\t$"<<g->Entries[left->typeval]->size<<", %esp"<<endl;
    }
    // cout<<"\tmovl\t("<<rstack.top()<<"), "<<rstack.top()<<endl;
    // cout<<"\tmovl\t"<<off<<"(%ebp)"<<rstack.top();
    
    return Lcount;
    
}
protected:
};
class arrow_astnode : public ref_astnode
{
public:
exp_astnode* left;
identifier_astnode* right;
arrow_astnode(exp_astnode* a, string b){
    left = a;
    right =new identifier_astnode(b);
    this->isaddr = a->isaddr;
}
void print(int blanks){
    printAst("arrow", "aa",
            "pointer", left, 
            "field", right
            );
};
// enum typeExp arrow_astnode_type;
int gencode(SymbTab* s, SymbTab *g, stack<string>rstack,int Lcount){
   
    Lcount = left->gencode(s,g,rstack,Lcount);
    string type = left->typeval;
    size_t str = type.find('*');
    size_t sqb = type.find('[');
    size_t br = type.find('(');
   if(br!=string::npos){
        type = type.substr(0,br);
    }
    else if(str!=string::npos){
        type = type.substr(0,str);
    }
    else if(sqb!=string::npos){
        type = type.substr(0,sqb);
    }
    
    else{
        cout<<"type checking failed"<<endl;
    }
   
    int off = g->Entries[type]->symbtab->Entries[right->name]->offset;
  
    // cout<<off<<endl;
    if(left->isaddr){
        
        cout<<"\tleal\t"<<left->addr<<", "<<rstack.top()<<endl;
    }
    else{
        
        string ad =left->addr;
        if(ad[0]=='('){
            ad = ad.substr(1,ad.length()-2);
        }
        
        // cout<<"idhar"<<endl;
        cout<<"\tmovl\t"<<ad<<", "<<rstack.top()<<endl;
    }
    
    cout<<"\taddl\t$"<<off<<", "<<rstack.top()<<endl;
    string reg = rstack.top();
    rstack.pop();
    cout<<"\tmovl\t"<<reg<<", "<<rstack.top()<<endl;
    this->addr = "("+rstack.top()+")";
    rstack.push(reg);
    cout<<"\tmovl\t("<<reg<<"), "<<reg<<endl;
    return Lcount;
    
}
protected:
};

class type_specifier_class{
public:
string type;
int size;
type_specifier_class(string a,int s){
    type = a;
    size = s;
}

//enum typeExp type_specifier_class_type;

protected:
};

class declarator_class{
   public:
string name;
int size;
string type;
declarator_class(string n,int s,string t){
    name=n;
    size = s;
    type = t;
}


//enum typeExp declarator_class_type;

protected:
};

class declaration_class{
public:
string name;
int size;
string type;
declaration_class(string n,int s,string t){
    name=n;
    size = s;
    type = t;
}

//enum typeExp declaration_class_type;

protected:
};

class declarator_list_class{
public:


//enum typeExp declarator_list_class_type;

protected:
};

class declaration_list_class{
public:


//enum typeExp declaration_list_class_type;

protected:
};

class fun_declarator_class{
public:


//enum typeExp type_specifier_class_type;

protected:
};

class parameter_declaration_class{
public:
type_specifier_class* type;
// declarator_class* dec;
parameter_declaration_class(type_specifier_class* a){
    type=a;
    // dec=b;
}

//enum typeExp parameter_declaration_class_type;

protected:
};
class parameter_list_class{
public:
vector<declarator_class*> list;
parameter_list_class(declarator_class* a){
    list.push_back(a);
}
void add_parameter(declarator_class* a){
    list.push_back(a);
}

//enum typeExp parameter_list_class_type;

protected:
};

