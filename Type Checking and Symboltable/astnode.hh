
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdarg.h>
#include <iostream>
//#include "print.hh"
using namespace std;

class abstract_astnode
{
public:

virtual void print(int blanks) = 0;
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
// exp_astnode* left;
// exp_astnode* right;
virtual void print(int blanks) = 0;
//enum typeExp exp_astnode_type;

protected:
};


class ref_astnode : public exp_astnode
{
public:
virtual void print(int blanks) = 0;
//enum typeExp ref_astnode_type;

protected:
};

class identifier_astnode : public ref_astnode
{
public:
string name;

identifier_astnode(string var){
    name = "\""+var+"\"";
}
void print(int blanks){
    
    // printf("{\n\"identifier\": \"%s\"\n}\n",name.c_str());
    printAst("\0","s","identifier",name.c_str());
};


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
    op = "\""+o+"\"";

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
    op = "\""+op+"\"";
    unary_exp = unary_e;
}

void print(int blanks){
    printAst("op_unary", "sa",
            "op", op.c_str(),
            "child", unary_exp
            );
};
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

protected:
};

class return_astnode : public statement_astnode
{
public:
exp_astnode* ret_val;
return_astnode(exp_astnode* a){
    ret_val=a;
}
void print(int blanks){
    printAst("\0","a",
    "return",ret_val);
};
// enum typeExp return_astnode_type;

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

protected:
};



class arrayref_astnode : public ref_astnode
{
public:
exp_astnode* left;
exp_astnode* right;

arrayref_astnode(exp_astnode* a, exp_astnode* b){
    left = a;
    right = b;
}
void print(int blanks){
    printAst("arrayref","aa",
            "array",left,
            "index",right);
};
// enum typeExp arrayref_astnode_type;

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
}
void print(int blanks){
    printAst("member", "aa",
            "struct", left, 
            "field", member
            );
};
// enum typeExp member_astnode_type;

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
}
void print(int blanks){
    printAst("arrow", "aa",
            "pointer", left, 
            "field", right
            );
};
// enum typeExp arrow_astnode_type;

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

