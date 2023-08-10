#include <cstring>
#include <cstddef>
#include <istream>
#include <iostream>
#include <fstream>
#include <map>

#include "scanner.hh"
#include "parser.tab.hh"

SymbTab gst, gstfun, gststruct; 
vector<string>func_names;
string filename;
stack<string> Reg_Stack;
extern std::map<string,abstract_astnode*> ast;
int Lcount = 0;

// std::map<std::string, datatype> predefined {
//             {"printf", createtype(VOID_TYPE)},
//             {"scanf", createtype(VOID_TYPE)},
//             {"mod", createtype(INT_TYPE)}
//         };

int main(int argc, char **argv)
{
	using namespace std;
	fstream in_file, out_file;
	

	in_file.open(argv[1], ios::in);

	IPL::Scanner scanner(in_file);

	IPL::Parser parser(scanner);

#ifdef YYDEBUG
	parser.set_debug_level(1);
#endif
parser.parse();

// create gstfun with function entries only

for (const auto &entry : gst.Entries)
{
	if (entry.second->varfun == "fun")
	gstfun.Entries.insert({entry.first, entry.second});
}
// create gststruct with struct entries only

for (const auto &entry : gst.Entries)
{
	if (entry.second->varfun == "struct")
	gststruct.Entries.insert({entry.first, entry.second});
}
// start the JSON printing

// cout << "{\"globalST\": " << endl;
// gst.printgst();
// cout << "," << endl;

// cout << "  \"structs\": [" << endl;

cout<<"\t.file\t\""<<argv[1]<<"\"\n";
cout<<"\t.text"<<endl;

Reg_Stack.push("%edi");
Reg_Stack.push("%esi");
Reg_Stack.push("%edx");
Reg_Stack.push("%ecx");
Reg_Stack.push("%ebx");
Reg_Stack.push("%eax");
for (auto it = gststruct.Entries.begin(); it != gststruct.Entries.end(); ++it)

{   
    // cout << "{" << endl;
	// cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	// cout << "\"localST\": " << endl;
	// it->second->symbtab->print();
	// cout << "}" << endl;
	// if (next(it,1) != gststruct.Entries.end()) 
	// cout << "," << endl;
}
// cout << "]," << endl;
// cout << "  \"functions\": [" << endl;

for (auto func = func_names.begin(); func != func_names.end(); ++func)

{
	SymbTab* it = gstfun.Entries[*func]->symbtab;
	int num_str = it->Localstr.size();
	if( num_str > 0){
		cout<<"\t.section\t.rodata"<<endl;
		for( auto i = it->Localstr.begin(); i != it->Localstr.end(); ++i){
			cout<<"."<<i->second<<":\n";
			cout<<"\t.string\t"<<i->first<<"\n";
		}
		cout<<"\t.text"<<endl;
	}

	cout<<"\t.globl\t"<<*func<<"\n\t.type\t"<<*func<<", @function"<<endl;
	cout<<*func<<":"<<endl;
	cout<<"\tpushl\t%ebp"<<endl;
    cout<<"\tmovl\t%esp, %ebp"<<endl;
    int local_sp = 0;
	vector<pair<int,int> > offset_sub;
    for (auto it1 = it->Entries.begin(); it1 != it->Entries.end(); ++it1){
        if(it1->second->global=="local"){
            if(it1->second->offset<local_sp){
                local_sp=it1->second->offset;
            }
        }

		if(it1->second->global=="param" 
		&& it1->second->ret_type.substr(0,6)!="struct"
		){

			offset_sub.push_back(make_pair(it1->second->offset,it1->second->size-4));
            it1->second->size = 4;
        }
    }
	map<string,int> og_new;

	for (auto it1 = it->Entries.begin(); it1 != it->Entries.end(); ++it1){

		for(int i = 0; i<offset_sub.size(); i++){

			if(it1->second->offset > offset_sub[i].first){
				int off = it1->second->offset;
				if(og_new.find(it1->first) != og_new.end()){
					off = og_new[it1->first];
				}
				
				int new_off = off - offset_sub[i].second;
				//  cout<<"3 "<<it1->first<< new_off<<endl;
				og_new[it1->first] = new_off;
			}
		}
		
    }
	for ( auto i = og_new.begin(); i != og_new.end(); ++i){
		//  cout<<i->first<<i->second<<endl;
		it->Entries[i->first]->offset = i->second;
	}
	cout<<"\tsubl\t$"<<local_sp*-1<<", %esp"<<endl;
	Lcount = ast[*func]->gencode(it,&gst,Reg_Stack,Lcount);

	// cout << "{" << endl;
	// cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	// cout << "\"localST\": " << endl;
	// it->second->symbtab->print();
	// cout << "," << endl;
	// cout << "\"ast\": " << endl;
	// ast[it->first]->print(0);
	// cout << "}" << endl;
	// if (next(it,1) != gstfun.Entries.end()) cout << "," << endl;
	if(gstfun.Entries[*func]->ret_type=="void"){
		cout<<"\tleave"<<endl;
		cout<<"\tret"<<endl;
		

		
	}
	cout<<"\t.size\t"<<*func<<", .-"<<*func<<endl;
	if(*func=="main") {
			cout<<"\t.ident	\"GCC: (Ubuntu 8.1.0-9ubuntu1~16.04.york1) 8.1.0\"\n\t.section	.note.GNU-stack,\"\",@progbits\n";
		}

	
}
	// cout << "]" << endl;
	// cout << "}" << endl;

	fclose(stdout);
}

