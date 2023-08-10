%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 
%define api.namespace {IPL}
%define api.parser.class {Parser}

%define parse.trace

%code requires{
    #include "astnode.hh"
    #include "symtab.hh"
    #include "location.hh"
    
   
   namespace IPL {
      class Scanner;
   }

  // # ifndef YY_NULLPTR
  // #  if defined __cplusplus && 201103L <= __cplusplus
  // #   define YY_NULLPTR nullptr
  // #  else
  // #   define YY_NULLPTR 0
  // #  endif
  // # endif

}
%printer { std::cerr << $$; } IF
%printer { std::cerr << $$; } WHILE
%printer { std::cerr << $$; } FOR
%printer { std::cerr << $$; } ELSE
%printer { std::cerr << $$; } FLOAT
%printer { std::cerr << $$; } VOID
%printer { std::cerr << $$; } INT
%printer { std::cerr << $$; } STRUCT
%printer { std::cerr << $$; } FLOAT_CONSTANT
%printer { std::cerr << $$; } INT_CONSTANT
%printer { std::cerr << $$; } RETURN
%printer { std::cerr << $$; } OR_OP
%printer { std::cerr << $$; } AND_OP
%printer { std::cerr << $$; } EQ_OP
%printer { std::cerr << $$; } NE_OP
%printer { std::cerr << $$; } LE_OP
%printer { std::cerr << $$; } GE_OP
%printer { std::cerr << $$; } INC_OP
%printer { std::cerr << $$; } PTR_OP
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } OTHERS

%parse-param { Scanner  &scanner  }
%locations
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>
   #include <map>
   #include<bits/stdc++.h>
   #include "scanner.hh"
   extern SymbTab gst;
   std::map<string,abstract_astnode*> ast;
   int nodeCount = 0;
   SymbTab* localtb;
   string type;
   string fname;
   string funtype;
   string scope;
   int offset;
   int size;
   int PARAM=12;
   int LOCAL=0;
   int RET_ADDR=4,RET_VAL=8;
   type_specifier_class *base_type,*local_type;
   map<string,vector<pair<string,string>>> funpartype;
   string curstruct;
   


#undef yylex
#define yylex IPL::Parser::scanner.yylex

}




%define api.value.type variant
%define parse.assert

%start translation_unit



%token <std::string> IF
%token <std::string> WHILE
%token <std::string> FOR
%token <std::string> ELSE
%token <std::string> FLOAT
%token <std::string> VOID
%token <std::string> INT
%token <std::string> STRUCT
%token <std::string> FLOAT_CONSTANT
%token <std::string> INT_CONSTANT
%token <std::string> RETURN
%token <std::string> OR_OP
%token <std::string> AND_OP
%token <std::string> EQ_OP
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> INC_OP
%token <std::string> PTR_OP
%token <std::string> STRING_LITERAL
%token <std::string> IDENTIFIER 
%token <std::string> OTHERS

%token ',' ':' '(' ')' '+' '-' '*' '/' '>' '<' '[' ']' '!' '&' ';' '.' '=' '{' '}'


%nterm <exp_astnode*> primary_expression;
//%nterm <abstract_astnode*> translation_unit;
//%nterm <abstract_astnode*> struct_specifier;
//%nterm <abstract_astnode*> function_definition;
/*



%nterm <declaration_class*> declaration;
%nterm <declarator_list_class*> declarator_list;
%nterm <declaration_list_class*> declaration_list;
*/
%nterm <exp_astnode*> expression;
%nterm <declarator_class*> declarator_arr;
%nterm <declarator_class*> declarator;
//%nterm <fun_declarator_class*> fun_declarator;
%nterm <parameter_list_class*> parameter_list;
%nterm <declarator_class*> parameter_declaration;
%nterm <type_specifier_class*> type_specifier;
%nterm <statement_astnode*> statement;
%nterm <assignS_astnode*> assignment_statement;

%nterm <statement_astnode*> compound_statement;
%nterm <seq_astnode*> statement_list;
%nterm <assignE_astnode*> assignment_expression;
%nterm <exp_astnode*> logical_and_expression;
%nterm <exp_astnode*> equality_expression;
%nterm <exp_astnode*> relational_expression;
%nterm <exp_astnode*> additive_expression;
%nterm <exp_astnode*> multiplicative_expression;
%nterm <exp_astnode*> unary_expression;
%nterm <exp_astnode*> postfix_expression;

%nterm <int> unary_operator;
%nterm <vector<exp_astnode*>> expression_list;
%nterm <proccall_astnode*> procedure_call;
%nterm <statement_astnode*> selection_statement;
%nterm <statement_astnode*> iteration_statement;

%nterm <int> translation_unit struct_specifier  function_definition declaration declaration_list declarator_list fun_declarator;

%%

translation_unit: 
      struct_specifier 
      {
            
            
      }
      | function_definition 
      {
            
      }
      | translation_unit struct_specifier 
      {
            
      }
      | translation_unit function_definition 
      {
            
      }
      ;

struct_specifier: 
      STRUCT IDENTIFIER {localtb = new SymbTab(); size=0;offset=0;
            
            // string name = $1 + $2;
            // gst.Entries.insert({name,a});
            curstruct = $1+" "+$2;
            Entry* a = new Entry("struct","global","-",size,0);
            a->symbtab = localtb;
            
            //SymbTab* l = new SymbTab(name,);
           // $$ = new abstract_astnode($1,$2,$3);
           if(gst.Entries.find($1+" "+$2)!=gst.Entries.end()){
                 error(@$, "\"" + $1+" "+$2 + "\" has a previous definition");
           }
           gst.Entries.insert({$1+" "+$2, a});
           
           fname = "@$";
      }'{' declaration_list '}' ';' 
      {
            
            gst.Entries[$1+" "+$2]->size = size;
            curstruct = "!!";
            
      }
      ;

function_definition: 
      type_specifier {localtb = new SymbTab(); funtype = base_type->type; offset = PARAM; } fun_declarator {
            offset=LOCAL;
            Entry* a = new Entry("fun","global",funtype,0,0);
            a->symbtab = localtb;
            string name = fname;
            if(gst.Entries.find(name)!=gst.Entries.end()){
                 error(@$, "The function \"" + name + "\" has a previous definition");
           }
            gst.Entries.insert({name,a});
            
            } compound_statement 
      { 
            
            ast.insert({fname,$5});       
            
      }
      ;

type_specifier: 
      VOID 
      {
            //$$=nodeCount;
            
            base_type = new type_specifier_class($1,0);
            $$ = base_type;
            // base_type = $$;
      }
      | INT 
      {
            //$$=nodeCount;
            base_type = new type_specifier_class($1,4);
           $$ = base_type;
            // base_type = $$;
      }
      | FLOAT 
      {
            //$$=nodeCount;
            base_type = new type_specifier_class($1,4);
            $$ = base_type;
            // base_type = $$;
      }
      | STRUCT IDENTIFIER 
      {
            //$$=nodeCount;
            if(gst.Entries.find($1+" "+$2)==gst.Entries.end() ){
                  error(@$,"\""+$1+" "+$2+ "\" is not defined");
            }
            base_type = new type_specifier_class($1+" "+$2,gst.Entries[$1+" "+$2]->size);
            $$ = base_type;
            // base_type = $$;
      }
      ;

fun_declarator: 
      IDENTIFIER '(' {scope = "param";} parameter_list ')' 
      {
            //$$=nodeCount;
            fname = $1;
            //$$ = new fun_declarator_class($1,$3);
            vector<pair<string,string>> par;

            for(auto & it : $4->list) {
                  if(it->type=="void"){
                        error(@$,"Cannot declare variable of type \"void\"");
                  }
                  offset -= it->size;
                  
                  
                  Entry* a = new Entry("var","param",it->type,it->size,offset);
                  localtb->Entries.insert({it->name,a});
                  

                  size_t open_br = (it->type).find('[');
                  size_t close_br = (it->type).find(']');
                  size_t open_par = (it->type).find('(');
                  if(open_br != string::npos && open_par == string::npos){
                        string t = it->type;
                        it->type =  t.substr(0,open_br)+"(*)"+t.substr(close_br+1,t.length()-close_br-1);
                  }
                  else if(it->type[(it->type).length()-1]=='*'){
                        it->type = it->type.substr(0,it->type.length()-1)+"(*)";
                  }
                  par.push_back({it->name,it->type});
                  
                  
            }
            funpartype.insert({fname,par});
            scope = "";
            
            
            
      }
      | IDENTIFIER '(' ')' 
      {
            //$$=nodeCount;
            fname = $1;
            vector<pair<string,string>> par;
            funpartype.insert({fname,par});
            //$$ = new fun_declarator_class($1);
      }
      ;

parameter_list: 
      parameter_declaration 
      {
            //$$=nodeCount;
            $$ = new parameter_list_class($1);
      }
      | parameter_list ',' parameter_declaration 
      {
            //$$=nodeCount;
            $$= $1;
          

            $$->add_parameter($3);
      }
      ;

parameter_declaration: 
      type_specifier {local_type = new type_specifier_class(base_type->type, base_type->size);} declarator 
      {
            //$$=nodeCount;

            $$ = $3;
            
      }
      ;

declarator_arr: 
      IDENTIFIER 
      {
            //$$ = new declarator_class($1,0,0,0);

            if(localtb->Entries.find($1) != localtb->Entries.end()){
                  error(@$, "\"" + $1 +"\" has a previous declaration");
            }

            if(scope=="local"){
                  $$ = new declarator_class($1,local_type->size, local_type->type);
                  // Entry * a = new Entry("var","local",local_type->type,local_type->size,offset);
                  // localtb->Entries.insert({$1,a});
                  // offset -= local_type->size;
                  // size += local_type->size;
            }
            else if(scope=="param"){

                  $$ = new declarator_class($1,local_type->size, local_type->type);
                  
                 
            }
      }
      | declarator_arr '[' INT_CONSTANT ']' 
      {
            //$$ = new declarator_class($1,1,$3,0);
            local_type->type = $1->type + "["+$3+"]";
            local_type->size = $1->size * stoi($3);
            $1->type = local_type->type;
            $1->size = local_type->size;
            $$ = $1;
            
      }
      ;

declarator: 
      declarator_arr 
      {
            //$$ = $1;
            $$ = $1;
            if(scope == "param"){
                  offset += $1->size;
            }
      }
      | '*' {local_type->type = local_type->type+ "*";local_type->size = 4;} declarator 
      {
            // $1->derefLevel++;
            $$ = $3;
            
      }
      ;

compound_statement: 
      '{' '}' 
      {
            // empty_astnode = new
            $$ = new seq_astnode();
            
      }
      | '{' statement_list '}' 
      {
            $$ = $2;
            
      }
      | '{' declaration_list '}' 
      {
            $$ = new seq_astnode();
            // for(auto it=$3->list.begin(),it!=$3->list.end(),it++){
            //       Entry* a = new Entry("var","local",it->type->type,it->type->size,offset);
            //       localtb->Entries.insert({fname,a});
            //       offset -= it->type->size;
                  
            // }
      }
      | '{' declaration_list statement_list '}' 
      {
            $$ = $3;
            // for(auto it=$3->list.begin(),it!=$3->list.end(),it++){
            //       Entry* a = new Entry("var","local",it->type->type,it->type->size,offset);
            //       localtb->Entries.insert({fname,a});
            //       offset -= it->type->size;
            // }
      }
      ;

statement_list: 
      statement 
      {
            $$ = new seq_astnode($1);
      }
      | statement_list statement
      {
            $$ = $1;
            $$->add_statement($2);
      }
      ;

statement: 
      ';' 
      {

            $$ = new empty_astnode();
      }
      | '{' statement_list '}' 
      { 
            $$ = $2;
      }
      | selection_statement 
      {
            $$=$1;
      }
      | iteration_statement
      {
            $$=$1;
      }
      | assignment_statement
      {
            $$=$1;
      }
      | procedure_call 
      {
            $$=$1;
      }
      | RETURN expression ';' 
      {

            string t = gst.Entries[fname]->ret_type;

            if($2->typeval != gst.Entries[fname]->ret_type){
                  if($2->typeval=="int" && t=="float"){
                        exp_astnode* a;
                        a = new op_unary_astnode(6,$2);
                        a->typeval = $2->typeval;
                        $$ = new return_astnode(a);

                  }
                  else if($2->typeval=="float" && t=="int"){
                        exp_astnode* a;
                        a = new op_unary_astnode(7,$2);
                        a->typeval = $2->typeval;
                        $$ = new return_astnode(a);
                  }
                  else{
                        error(@$,"Incompatible type \""+$2->typeval+"\" returned, expected \""+t+"\"");
                  }
            }
            else{
            $$ = new return_astnode($2);
            }
      }
      ;

assignment_expression: 
      unary_expression '=' expression
      {
            string l=$1->typeval;
            string r=$3->typeval;

            bool assignable = true;

            size_t open_br = (l).find('[');
            size_t close_br = (l).find(']');
            size_t open_par = (l).find('(');
            
            
            // std::cout<<l<<endl;
            // std::cout<<r<<endl;

            if(open_br != string::npos && open_par == string::npos && !$1->isparam){
                  assignable = false;
            }

            //lvalue
            if(!$1->lvalue){
                  error(@$,"Left operand of assignment should have an lvalue");
            }

            int l_c = count(l.begin(),l.end(),'*') + count(l.begin(),l.end(),'[');
            int r_c = count(r.begin(),r.end(),'*') + count(r.begin(),r.end(),'[');


            if(l==r && assignable){
                  $$ = new assignE_astnode($1,$3);
                  $$->typeval = l;
            }
            else if(l=="float" && r=="int" && assignable){
                  exp_astnode * tc_r = new op_unary_astnode(6,$3);
                  $$ = new assignE_astnode($1,tc_r);
                  $$->typeval = "float";

            }
            else if(l=="int" && r=="float" && assignable){
                  exp_astnode * tc_r = new op_unary_astnode(7,$3);
                  $$ = new assignE_astnode($1,tc_r);
                  $$->typeval = "int";
            }
            else if(assignable && l_c>0 && r_c>0 && l!="void*" && l!="void(*)" && r!="void*" && r!="void(*)"){
                  string new_l, new_r;
                  if(count(l.begin(),l.end(),'(')==0){
                        if(l[l.length()-1]=='*'){
                              new_l = l.substr(0,l.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = l.find('[');
                              close_br = l.find(']');
                              new_l = l.substr(0,open_br)+"(*)"+l.substr(close_br+1,l.length()-close_br-1);
                        }

                  }
                  else{
                        new_l = l;
                  }
                  if(count(r.begin(),r.end(),'(')==0){
                        if(r[r.length()-1]=='*'){
                              new_r = r.substr(0,r.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = r.find('[');
                              close_br = r.find(']');
                              new_r = r.substr(0,open_br)+"(*)"+r.substr(close_br+1,r.length()-close_br-1);
                        }

                  }
                  else{
                        new_r = r;
                  }

                  if(new_l == new_r){
                        $$ = new assignE_astnode($1,$3);
                        $$->typeval = l;
                  }
                  
                  else{
                        error(@$, "Incompatible assignment when assigning to type \"" + l+ "\" from type \"" +r+"\"");
                  }

            }
            else if(l=="void*" || l=="void(*)"){
                  if(r_c>0 || $3->iszero){
                        $$ = new assignE_astnode($1,$3);
                        $$->typeval = l;
                  }
                  else{
                        error(@$, "Incompatible assignment when assigning to type \"" + l+ "\" from type \"" +r+"\"");
                  }
            }
            else if(assignable && (r=="void*" || r=="void(*)")){
                  if(l_c > 0){
                        $$ = new assignE_astnode($1,$3);
                        $$->typeval = l;
                  }
                  else{
                        error(@$, "Incompatible assignment when assigning to type \"" + l+ "\" from type \"" +r+"\"");
                  }
            }
            else if(assignable && l_c>0 && $3->iszero){
                  $$ = new assignE_astnode($1,$3);
                  $$->typeval = l;
            }
            else{
                  error(@$, "Incompatible assignment when assigning to type \"" + l+ "\" from type \"" +r+"\"");
            }
            $$->lvalue = false;
      }
      ;

assignment_statement: 
      assignment_expression ';' 
      {
            $$ = new assignS_astnode($1);
      }
      ;

procedure_call: 
      IDENTIFIER '(' ')' ';' 
      {     
            if(funpartype.find($1) == funpartype.end()){
                  error(@$,"Procedure \"" + $1+"\" not declared");
            }
            if(funpartype[$1].size()!=0){
                  error(@$,"Procedure \"" + $1+"\" called with too few arguments");

            }
            $$ = new proccall_astnode($1);

            // $$->typeval = gst.Entries[$1]->ret_type;
      }
      | IDENTIFIER '(' expression_list ')' ';' 
      {
            if(funpartype.find($1) != funpartype.end()||($1!="printf" && $1!="scanf")){
            if(funpartype.find($1) == funpartype.end()){
                  error(@$,"Procedure \"" + $1+"\"  not declared");
            }
            if(funpartype[$1].size()>$3.size()){
                  error(@$,"Procedure \"" + $1+"\"  called with too few arguments");

            }
            if(funpartype[$1].size()<$3.size()){
                  error(@$,"Procedure \"" + $1+"\"  called with too many arguments");

            }
            
            
            for(int i=0;i<$3.size();i++){
                  string lt = funpartype[$1][i].second;
                  string rt = $3[i]->typeval;
                  int l_c = count(lt.begin(), lt.end(), '[') + count(lt.begin(), lt.end(), '*');
                  int r_c = count(rt.begin(), rt.end(), '[') + count(rt.begin(), rt.end(), '*');
                  if(funpartype[$1][i].second!=$3[i]->typeval ){
                        if(funpartype[$1][i].second == "int" && $3[i]->typeval=="float"){
                              exp_astnode * tc_param = new op_unary_astnode(7,$3[i]);
                              $3[i] = tc_param;
                        }
                        else if(funpartype[$1][i].second == "float" && $3[i]->typeval=="int"){
                              exp_astnode * tc_param = new op_unary_astnode(6,$3[i]);
                              $3[i] = tc_param;
                        }
                        else if(l_c>0 && $3[i]->iszero){
                              //$3[i]=$3[i];
                        }
                        else if(l_c > 0 && r_c > 0 && lt!="void*" && lt!="void(*)" && rt!="void*" && rt!="void(*)"){
                              
                              string new_lt, new_rt;
                              if(count(lt.begin(),lt.end(),'(')==0){
                                    if(lt[lt.length()-1]=='*'){
                                          new_lt = lt.substr(0,lt.length()-1)+"(*)";
                                    }
                                    else {
                                          size_t open_br, close_br;
                                          open_br = lt.find('[');
                                          close_br = lt.find(']');
                                          new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                                    }

                              }
                              else{
                                    new_lt = lt;
                              }

                              if(count(rt.begin(),rt.end(),'(')==0){
                                    if(rt[rt.length()-1]=='*'){
                                          new_rt = rt.substr(0,rt.length()-1)+"(*)";
                                    }
                                    else {
                                          size_t open_br, close_br;
                                          open_br = rt.find('[');
                                          close_br = rt.find(']');
                                          new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                                    }

                              }
                              else{
                                    new_rt = rt;
                              }
                              if(new_lt != new_rt) {
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }

                        }
                        else if(lt=="void*" || lt=="void(*)"){
                              if(r_c==0){
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }
                        }
                        else if(rt=="void*" || rt=="void(*)"){
                              if(l_c == 0){
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }
                        }
                        else{
                              string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                              error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                        }
                  }
            }
            }
            $$ = new proccall_astnode($1,$3);
            
            // $$->typeval = gst.Entries[$1]->ret_type;
                        
            // $$ = new proccall_astnode($1,$3);
      }
      ;

expression: 
      logical_and_expression
      {    
            
            $$ = $1;
      }
      | expression OR_OP logical_and_expression
      { 
            string lt = $1->typeval;
            string rt = $3->typeval;
            size_t open_par_l = lt.find('(');
            size_t open_par_r = rt.find('(');
            if(lt[lt.length()-1] !='*' && lt[lt.length()-1]!=']'&& lt!="int" && lt!="float" && open_par_l==string::npos){
                  error(@$, "Invalid operand of ||,  not scalar");
            }
            if(rt[rt.length()-1] !='*' && rt[rt.length()-1]!=']'&& rt!="int" && rt!="float" && open_par_r==string::npos){
                  error(@$, "Invalid operand of ||,  not scalar");
            }
            $$ = new op_binary_astnode($1,"OR_OP",$3);
            $$->typeval = "int";
            $$->lvalue = false;
      }
   ;

logical_and_expression: 
      equality_expression
      {
            $$ = $1;
           
      }
      | logical_and_expression AND_OP equality_expression
      {
            string lt = $1->typeval;
            string rt = $3->typeval;
            size_t open_par_l = lt.find('(');
            size_t open_par_r = rt.find('(');
            if(lt[lt.length()-1] !='*' && lt[lt.length()-1]!=']'&& lt!="int" && lt!="float" && open_par_l==string::npos){
                  error(@$, "Invalid operand of &&,  not scalar");
            }
            if(rt[rt.length()-1] !='*' && rt[rt.length()-1]!=']'&& rt!="int" && rt!="float" && open_par_r==string::npos){
                  error(@$, "Invalid operand of &&,  not scalar");
            }
            $$ = new op_binary_astnode($1,"AND_OP",$3);
            $$->typeval = "int";
            $$->lvalue=false;
      };

equality_expression :
      relational_expression 
      {
            $$=$1;
            
            
      }
      | equality_expression EQ_OP relational_expression 
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"EQ_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"EQ_OP_FLOAT",right);
                  $$->typeval = "int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"EQ_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if((lt=="void*"|| lt=="void(*)" ) && (r_c>0)){
                  $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if((rt=="void*"|| rt=="void(*)" ) && (l_c>0)){
                  $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(($1->iszero ) && (r_c>0)){
                  $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(($3->iszero ) && (l_c>0)){
                  $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"EQ_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary == , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary == , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue= false;
      }
      | equality_expression NE_OP relational_expression 
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  
                  $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  
                  $$ = new op_binary_astnode($1,"NE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
  
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"NE_OP_FLOAT",right);
                  $$->typeval = "int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"NE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if((lt=="void*"|| lt=="void(*)" ) && (r_c>0)){
                  $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if((rt=="void*"|| rt=="void(*)" ) && (l_c>0)){
                  $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(($1->iszero ) && (r_c>0)){
                  $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(($3->iszero) && (l_c>0)){
                  $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"NE_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary != , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary != , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue=false;
           
      }
      ;

relational_expression:
      additive_expression 
      {
            $$=$1;
      }
      | relational_expression '<' additive_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"LT_OP_INT",$3);
                  $$->typeval="int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"LT_OP_FLOAT",$3);
                  $$->typeval="int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"LT_OP_FLOAT",right);
                  $$->typeval="int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"LT_OP_FLOAT",$3);
                  $$->typeval="int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"LT_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary < , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary < , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
      }
      | relational_expression '>' additive_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"GT_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"GT_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"GT_OP_FLOAT",right);
                  $$->typeval = "int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"GT_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"GT_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary > , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary > , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
      }
      | relational_expression LE_OP additive_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"LE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"LE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"LE_OP_FLOAT",right);
                  $$->typeval = "int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"LE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"LE_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary <= , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary <= , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
      }
      
      | relational_expression GE_OP additive_expression
      {
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"GE_OP_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"GE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"GE_OP_FLOAT",right);
                  $$->typeval = "int";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"GE_OP_FLOAT",$3);
                  $$->typeval = "int";
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"GE_OP_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary >= , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }
            }
            else{
                  error(@$,"Invalid operand types for binary >= , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
            
      }
      ;

additive_expression:
      multiplicative_expression
      {
            $$=$1;
            
      }
      | additive_expression '+' multiplicative_expression
      {
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"PLUS_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"PLUS_FLOAT",$3);
                  $$->typeval = "float";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"PLUS_FLOAT",right);
                  $$->typeval = "float";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"PLUS_FLOAT",$3);
                  $$->typeval = "float";
            }
            else if(l_c > 0 && rt=="int"){ 
                  $$ = new op_binary_astnode($1,"PLUS_INT",$3);
                  $$->typeval = $1->typeval;
                  //POINTERS
            }
            else if(r_c>0 && lt=="int"){
                  $$ = new op_binary_astnode($1,"PLUS_INT",$3);
                  $$->typeval = $3->typeval;
            }
            else{
                  error(@$,"Invalid operand types for binary + , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
            
      }
      | additive_expression '-' multiplicative_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            string lt = $1->typeval;
            string rt = $3->typeval;
            int l_c = count(lt.begin(),lt.end(),'*') + count(lt.begin(),lt.end(),'[');
            int r_c = count(rt.begin(),rt.end(),'*') + count(rt.begin(),rt.end(),'[');
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"MINUS_INT",$3);
                  $$->typeval="int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"MINUS_FLOAT",$3);
                  $$->typeval="float";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"MINUS_FLOAT",right);
                  $$->typeval="float";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"MINUS_FLOAT",$3);
                  $$->typeval="float";
            }
            // else if(l_c > 0 && rt=="int"){ 
            //       $$ = new op_binary_astnode($1,"MINUS_INT",$3);
            //       $$->typeval = $1->typeval;
            //       //POINTERS
            // }
            else if(l_c>0 && rt=="int"){
                  $$ = new op_binary_astnode($1,"MINUS_INT",$3);
                  $$->typeval = $1->typeval;
            }
            else if(r_c > 0 && l_c > 0){
                  string new_lt, new_rt;
                  if(count(lt.begin(),lt.end(),'(')==0){
                        if(lt[lt.length()-1]=='*'){
                              new_lt = lt.substr(0,lt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = lt.find('[');
                              close_br = lt.find(']');
                              new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                        }

                  }
                  else{
                        new_lt = lt;
                  }

                  if(count(rt.begin(),rt.end(),'(')==0){
                        if(rt[rt.length()-1]=='*'){
                              new_rt = rt.substr(0,rt.length()-1)+"(*)";
                        }
                        else {
                              size_t open_br, close_br;
                              open_br = rt.find('[');
                              close_br = rt.find(']');
                              new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                        }

                  }
                  else{
                        new_rt = rt;
                  }

                  if(new_lt == new_rt){
                        $$ = new op_binary_astnode($1,"MINUS_INT",$3);
                        $$->typeval = "int";
                  }
                  else{
                        error(@$,"Invalid operand types for binary - , \""+$1->typeval+"\" and \""+$3->typeval+"\"");     
                  }

            }
            else{
                  error(@$,"Invalid operand types for binary - , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
      }
      ; 

unary_expression:
      postfix_expression
      {
            $$=$1;
            
      }
      | unary_operator unary_expression
      {
            $$ = new op_unary_astnode($1,$2);
            if($1==1 ){
                  if($2->typeval=="int" || $2->typeval=="float"){
                        $$->typeval = $2->typeval;
                        $$->lvalue = $2->lvalue;
                  }
                  else{
                        error(@$,"Operand of unary '-' is not of type \"int\" or \"float\"");
                  }
            }
            else if($1==2){
                  size_t ast = $2->typeval.find('*');
                  size_t open_br = $2->typeval.find('[');
                  if($2->typeval=="int" || $2->typeval=="float" || ast!=string::npos || open_br!=string::npos){
                        $$->typeval = "int";
                        $$->lvalue = false;
                  }
                  else{
                        error(@$,"Operand of unary '-' is not of type \"int\", \"float\" or \"pointer\"");
                  }
                  
            }
            else if($1==3){
                  //"&"

                  //lvalue
                  if(!$2->lvalue){
                        error(@$,"Operand of & should have lvalue");
                  }

                  string t = $2->typeval;
                  size_t open_br;
                  open_br = t.find('[');
                  if(open_br == string::npos){
                        $$->typeval = $2->typeval + "(*)";
                  }
                  else{
                        $$->typeval = t.substr(0,open_br) +"(*)" + t.substr(open_br,t.length()-open_br);
                  }
                  $$->lvalue = false;
            }
            else if($1==4){
                  //"*"
                  string t = $2->typeval;
                  size_t open_par;
                  open_par = t.find('(');
                  if(t=="void*" || t=="void(*)"){
                        error(@$, "Invalid operand type \""+t+"\" of unary *");
                  }
                  if(open_par!=string::npos){
                        $$->typeval = t.substr(0,open_par)+t.substr(open_par+3,t.length()-open_par-3);
                  }
                  else if(t[t.length()-1]=='*'){
                        $$->typeval = t.substr(0,t.length()-1); 
                  }
                  else if(t[t.length()-1]==']'){
                        size_t open_br,close_br;
                        open_br = $2->typeval.find('[');
                        close_br = $2->typeval.find(']');
                        $$->typeval = $2->typeval.substr(0,open_br)+$2->typeval.substr(close_br+1,$2->typeval.length()-close_br -1);
                  }
                  
                  else{
                       
                        error(@$, "Invalid operand type \""+t+"\" of unary *");
                  }
                  $$->lvalue = true;

            }
      }
      ;

multiplicative_expression:
      unary_expression 
      {
            $$=$1;
      }
      | multiplicative_expression '*' unary_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"MULT_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"MULT_FLOAT",$3);
                  $$->typeval = "float";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"MULT_FLOAT",right);
                  $$->typeval = "float";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"MULT_FLOAT",$3);
                  $$->typeval = "float";
            }
            else{
                  error(@$,"Invalid operand types for binary * , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
            
      }
      | multiplicative_expression '/' unary_expression
      {
            string t;
            exp_astnode* left;
            exp_astnode* right;
            if($1->typeval=="int" && $3->typeval=="int" ){
                  $$ = new op_binary_astnode($1,"DIV_INT",$3);
                  $$->typeval = "int";
            }
            else if($1->typeval=="float" && $3->typeval=="float" ){
                  $$ = new op_binary_astnode($1,"DIV_FLOAT",$3);
                  $$->typeval = "float";
            }
            else if($1->typeval=="float" && $3->typeval=="int" ){
                  right = new op_unary_astnode(6,$3);
                  $$ = new op_binary_astnode($1,"DIV_FLOAT",right);
                  $$->typeval = "float";
            }
            else if($1->typeval=="int" && $3->typeval=="float" ){
                  left = new op_unary_astnode(6,$1);
                  $$ = new op_binary_astnode(left,"DIV_FLOAT",$3);
                  $$->typeval = "float";
            }
            else{
                  error(@$,"Invalid operand types for binary / , \""+$1->typeval+"\" and \""+$3->typeval+"\"");
            }
            $$->lvalue = false;
      }
      ;

postfix_expression: 
      primary_expression 
      {
            $$=$1;
            
      }
      | postfix_expression '[' expression ']' 
      {
            $$ = new arrayref_astnode($1,$3);
            if($3->typeval!="int" ){
                  error(@$,"Array subscript is not an integer");
            }
            else if ($1->typeval == "void*" || $1->typeval =="void(*)"){
                  error(@$, "Dereferencing void* pointer");
            }
            else{
                  size_t open_par, open_br;
                  open_par = $1->typeval.find('(');

                  open_br = $1->typeval.find('[');
                  string t = $1->typeval;

                  if(open_par != string::npos){
                        
                        $$->typeval = t.substr(0,open_par) + t.substr(open_par+3, t.length()-open_par-3);
                  
                  }
                  else if(open_br != string::npos){
                        size_t close_br = t.find(']');
                        $$->typeval = t.substr(0,open_br)+t.substr(close_br + 1, t.length()-close_br-1);
                        
                  }
                  else if(t[t.length()-1]=='*'){
                        $$->typeval = t.substr(0,t.length()-1);
                  }
                  else{
                        error(@$, "Subscripted value is neither array nor pointer");
                  }
                  // if(open_br ==string::npos){ 
                  //       string t = $1->typeval;
                  //       if(t[t.length()-1] == '*'){
                  //             $$->typeval = t.substr(0,t.length()-1);

                  //       }
                  //       else if(t[t.length()-1]==')'){
                  //             $$->typeval = t.substr(0,t.length()-3);
                  //       }                        
                  //       else{
                  //             error(@$, "Subscripted value is neither array nor pointer");
                  //       }
                  // }
                  // else{
                  //       // index range check if()
                  //       string_t open_par 
                  //       if(open_br !=string::npos){
                  //       $$->typeval = $1->typeval.substr(0,open_br);
                  //       }
                  //       else{
                  //             $$->typeval = $1->typeval.substr(0,$1->typeval.length()-1);
                  //       }
                  // }

            }
            $$->lvalue = true;
            
      }
      | IDENTIFIER '(' ')' 
      {
            if(funpartype.find($1) == funpartype.end()){
                  error(@$,"Function \"" + $1+"\" not declared");
            }
            if(funpartype[$1].size()!=0){
                  error(@$,"Function \"" + $1+"\" called with too few arguments");

            }
            $$ = new funcall_astnode($1);
            $$->lvalue = false;
            $$->typeval = gst.Entries[$1]->ret_type;
      }
      | IDENTIFIER '(' expression_list ')' 
      {
            if(funpartype.find($1) == funpartype.end()){
                  error(@$,"Function \"" + $1+"\"  not declared");
            }
            if(funpartype[$1].size()>$3.size()){
                  error(@$,"Function \"" + $1+"\"  called with too few arguments");

            }
            if(funpartype[$1].size()<$3.size()){
                  error(@$,"Function \"" + $1+"\"  called with too many arguments");

            }
            
            for(int i=0;i<$3.size();i++){
                  string lt = funpartype[$1][i].second;
                  string rt = $3[i]->typeval;
                  int l_c = count(lt.begin(), lt.end(), '[') + count(lt.begin(), lt.end(), '*');
                  int r_c = count(rt.begin(), rt.end(), '[') + count(rt.begin(), rt.end(), '*');
                  if(funpartype[$1][i].second!=$3[i]->typeval ){
                        if(funpartype[$1][i].second == "int" && $3[i]->typeval=="float"){
                              exp_astnode * tc_param = new op_unary_astnode(7,$3[i]);
                              $3[i] = tc_param;
                        }
                        else if(funpartype[$1][i].second == "float" && $3[i]->typeval=="int"){
                              exp_astnode * tc_param = new op_unary_astnode(6,$3[i]);
                              $3[i] = tc_param;
                        }
                        else if(l_c>0 && $3[i]->iszero){
                              // $3[i]=$3[i];
                        }
                        else if(l_c > 0 && r_c > 0 && lt!="void*" && lt!="void(*)" && rt!="void*" && rt!="void(*)"){
                              
                              string new_lt, new_rt;
                              if(count(lt.begin(),lt.end(),'(')==0){
                                    if(lt[lt.length()-1]=='*'){
                                          new_lt = lt.substr(0,lt.length()-1)+"(*)";
                                    }
                                    else {
                                          size_t open_br, close_br;
                                          open_br = lt.find('[');
                                          close_br = lt.find(']');
                                          new_lt = lt.substr(0,open_br)+"(*)"+lt.substr(close_br+1,lt.length()-close_br-1);
                                    }

                              }
                              else{
                                    new_lt = lt;
                              }

                              if(count(rt.begin(),rt.end(),'(')==0){
                                    if(rt[rt.length()-1]=='*'){
                                          new_rt = rt.substr(0,rt.length()-1)+"(*)";
                                    }
                                    else {
                                          size_t open_br, close_br;
                                          open_br = rt.find('[');
                                          close_br = rt.find(']');
                                          new_rt = rt.substr(0,open_br)+"(*)"+rt.substr(close_br+1,rt.length()-close_br-1);
                                    }

                              }
                              else{
                                    new_rt = rt;
                              }
                              if(new_lt != new_rt) {
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }

                        }
                        else if(lt=="void*" || lt=="void(*)"){
                              if(r_c==0){
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }
                        }
                        else if(rt=="void*" || rt=="void(*)"){
                              if(l_c == 0){
                                    string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                                    error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                              }
                        }
                        else{
                              string og_type = gst.Entries[$1]->symbtab->Entries[funpartype[$1][i].first]->ret_type;
                              error(@$,"Expected \""+og_type+"\" but argument is of type \""+$3[i]->typeval+"\"");
                        }
                  }
            }
            $$ = new funcall_astnode($1,$3);
            $$->lvalue = false;
            $$->typeval = gst.Entries[$1]->ret_type;
            
      }
      | postfix_expression '.' IDENTIFIER 
      {
            string t = $1->typeval;

            if(t.substr(0,6)!="struct" || gst.Entries.find(t)==gst.Entries.end()){
                  error(@$, "Left operand of \".\"  is not a structure");
            }
      


            SymbTab * lst = gst.Entries[t]->symbtab;
            if(lst->Entries.find($3) == lst->Entries.end()){
                  error(@$, "Struct \""+t+"\" has no member named \""+$3+"\"");
            }

            $$ = new member_astnode($1,$3);
            $$->typeval = lst->Entries[$3]->ret_type;
            $$->lvalue = $1->lvalue;

            
      }
      | postfix_expression PTR_OP IDENTIFIER 
      {
            string t = $1->typeval;
            int br=0;
            int astr=0;
            br = count(t.begin(),t.end(),'[');
            astr =  count(t.begin(),t.end(),'*');
            
            // if(open_br ==string::npos || close_br==string::npos){
            //       error(@$, "Subscripted value is neither array nor pointer");
            // }
            if(t.substr(0,6)!="struct" || br+astr!=1){
                 error(@$, "Left operand of \"->\"  is not a pointer to a structure");
            }
            string st;
            size_t ind;
            ind = $1->typeval.find('(');
            if(ind == string::npos){
                  ind = $1->typeval.find_last_of('[');
                  if(ind ==string::npos ){
                        ind = $1->typeval.find_last_of('*');
                  }
            }
            
            st = t.substr(0,ind);
            if( gst.Entries.find(st)==gst.Entries.end()){
                  error(@$, "Left operand of \"->\"  is not a pointer to a structure");
            }
            SymbTab * lst = gst.Entries[st]->symbtab;
            if(lst->Entries.find($3) == lst->Entries.end()){
                  error(@$, "Struct \""+st+"\" has no member named \""+$3+"\"");
            }

            $$ = new arrow_astnode($1,$3);
            $$->lvalue = true;
            $$->typeval = lst->Entries[$3]->ret_type;
      }
      | postfix_expression INC_OP 
      {
            string t = $1->typeval;

            //lvalue
            if(!$1->lvalue){
                  error(@$, "Operand of \"++\" should have lvalue");
            }
            
            size_t ast = t.find('*');
            if(ast!=string::npos ||t=="int"|| t=="float"){
                  $$ = new op_unary_astnode(5,$1);
                  $$->typeval = $1->typeval;
            }
            
            else{
                  error(@$,"Operand of \"++\" should be a int, float or pointer");
            }
            $$->lvalue = false;
            
      }
      ;

primary_expression:
      IDENTIFIER 
      {  
            if(localtb->Entries.find($1)==localtb->Entries.end()){
                  error(@$,"Variable \""+$1+"\" not declared");
            }
            
               $$ = new identifier_astnode($1);
               if(localtb->Entries[$1]->global=="param"){
                     $$->isparam=true;
               }
               $$->typeval = localtb->Entries[$1]->ret_type;
            

      }
      | FLOAT_CONSTANT 
      {     $$ = new floatconst_astnode($1);
            $$->typeval = "float";
            $$->lvalue = false;
      }
      | INT_CONSTANT 
      {
            $$ = new intconst_astnode($1);
            $$->typeval = "int";
            if(stoi($1)==0){
                  $$->iszero=true;
            }
            $$->lvalue = false;
            
      }
      | STRING_LITERAL 
      {
            $$ = new stringconst_astnode($1);
            $$->typeval = "string";
            $$->lvalue = false;
      }
      | '(' expression ')' 
      {
            $$ = $2;
            
      }
    ;


expression_list: 
      expression 
      {
            $$=vector<exp_astnode*>{$1};

       }
      | expression_list ',' expression 
      {
            $$=$1;
            $$.push_back($3);
       }
      ;

unary_operator:
      '-'
      {
            $$ = 1;
      }
      | '!'  
      {
            $$ = 2;
      }
      | '&' 
      {
           $$ = 3;
      }
      | '*' 
      {
            $$ = 4;
      }
      ;

selection_statement:
      IF '(' expression ')' statement ELSE statement 
      {
            string t = $3->typeval;
            size_t ast,open_br;
            ast = t.find('*');
            open_br = t.find('[');
            if(ast!=string::npos || open_br != string::npos || t=="int" || t=="float"){
            $$ = new if_astnode($3,$5,$7);
            }
            else{
                  error(@$,"used "+t+" where scalar is required");
            }
            
            
      }
      ;

iteration_statement:
      WHILE '(' expression ')' statement 
      {
            string t = $3->typeval;
            size_t ast,open_br;
            ast = t.find('*');
            open_br = t.find('[');
            if(ast!=string::npos || open_br != string::npos || t=="int" || t=="float"){
            $$ = new while_astnode($3,$5);
            }
            else{
                  error(@$,"used "+t+" where scalar is required");
            }
            
       }
      | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement 
      {
            string t = $5->typeval;
            size_t ast,open_br;
            ast = t.find('*');
            open_br = t.find('[');
            if(ast!=string::npos || open_br != string::npos || t=="int" || t=="float"){
            $$ = new for_astnode($3,$5,$7,$9);
            }
            else{
                  error(@$,"used "+t+" where scalar is required");
            }
            

      }
      ;

declaration_list:
      declaration 
      {
      //      $$=new declaration_list_class($1);
      }
      | declaration_list declaration 
      {
      //      $$ = $1;
      //       $$->list.push_back($2);
      }
      ;

declaration: 
      type_specifier declarator_list ';' 
      {
            // $$ = new declaration_class($1,$2);
            


      }
      ;

declarator_list: 
      {scope="local";
      
      local_type = new type_specifier_class(base_type->type,base_type->size);} declarator 
      {
            
            // $$=new declarator_list_class($1);
            
            offset-=local_type->size;
            Entry *a;
            if(fname != "@$") a = new Entry("var", "local", local_type->type, local_type->size, offset);
            else a = new Entry("var", "local", local_type->type, local_type->size, size);
            size+=local_type->size;
            if(local_type->type=="void"){
                  error(@$,"Cannot declare variable of type \"void\"");
            }
            if(local_type->type==curstruct){
                  error(@$,curstruct+" not defined");
            }
            localtb->Entries.insert({$2->name,a});
            scope = "";

      }
      | declarator_list ',' {scope = "local"; local_type = new type_specifier_class(base_type->type,base_type->size);} declarator 
      {
            // $$ = $1;
            // $$->list.push_back($2);
            
            
            offset-=local_type->size;
            Entry *a;
            if(fname != "@$") a = new Entry("var", "local", local_type->type, local_type->size, offset);
            else a = new Entry("var", "local", local_type->type, local_type->size, size);
            size+=local_type->size;
            if(local_type->type=="void"){
                  error(@$,"Cannot declare variable of type \"void\"");
            }
            if(local_type->type==curstruct){
                  error(@$,curstruct+" not defined");
            }
            localtb->Entries.insert({$4->name,a});
            scope = "";
      }
      ;




%%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "Error at line " << l.begin.line << ": " << err_message << "\n";
   exit(1);
}

