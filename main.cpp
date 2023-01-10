#include<bits/stdc++.h>
using namespace std;
class FD{
        public:
        string det;
        string dep;
        //det -> dep
        //constructor takes det and dep as input
        FD(string det,string dep){
                this->det=det;
                this->dep=dep;
        }
};

class Table{
        public:
        string name;
        vector<char> attributes;
        vector<FD> fds;
        //vector of det
        vector<string> det;
        vector<vector<int>> DM;
        vector<vector<int>> DG;
        vector<vector<int>> path;
        int offset;
        //constructor takes name and attributes and FD as input
        Table(string name,vector<char> attributes,vector<FD> fds){
                this->name=name;
                this->attributes=attributes;
                this->fds=fds;
                //initialise det with fds.det
                for(int i=0;i<fds.size();i++){
                        det.push_back(fds[i].det);
                }
                DM.assign(det.size(),vector<int>(attributes.size(),0));
                DG.assign(det.size(),vector<int>(det.size(),0));
                path.assign(det.size(),vector<int>(det.size(),0));
                //first 8 digits of pi as offset
                offset=31415926;
        }
        //function to check if a character c is present in a string s
        bool isPresent(char c,string s){
                for(int i=0;i<s.size();i++){
                        if(s[i]==c){
                                return true;
                        }
                }
                return false;
        }
        void fill_DM(){
                for(int i=0;i<det.size();i++){
                        for(int j=0;j<attributes.size();j++){
                                if(isPresent(attributes[j],det[i])){
                                        DM[i][j]=2;
                                }else if(isPresent(attributes[j],fds[i].dep)){
                                        DM[i][j]=1;
                                }
                        }
                }
        }
        //template general function to find index of element in a vector
        template<typename T>
        int findIndex(T element,vector<T> v){
                for(int i=0;i<v.size();i++){
                        if(v[i]==element){
                                return i;
                        }
                }
                return -1;
        }

        void fill_DG(){
                for(int i=0;i<det.size();i++){
                        for(int j=0;j<det.size();j++){
                                if(i==j){
                                        DG[i][j]=1;
                                }else{
                                        for(int k=0;k<det[j].size();k++){
                                                if(!DM[i][findIndex(det[j][k],attributes)]){
                                                        DG[i][j]=-1;
                                                        break;
                                                }
                                        }
                                        if(DG[i][j]!=-1){
                                                DG[i][j]=1;
                                        }
                                }
                        }
                }
        }
        //function that marks DG[x][y] = 1 if there is a path from x to y using floyd warshall
        void mark_DG(){
                path = DG;
                for(int k=0;k<det.size();k++){
                        for(int i=0;i<det.size();i++){
                                for(int j=0;j<det.size();j++){
                                        if(path[i][k]==1 && path[k][j]==1){
                                                path[i][j]=1;
                                        }
                                }
                        }
                }
        }
        void re_fill_DM(){
                //store copy of DM
                vector<vector<int>> DM_copy=DM;
                for(int i=0;i<det.size();i++){
                        for(int j=0;j<det.size();j++){
                                if(i != j and path[i][j]==1){
                                        for(int k=0;k<attributes.size();k++){
                                                if(DM[j][k] != 0 and DM[j][k] != 2){
                                                        DM[i][k] = offset+j;
                                                }
                                        }
                                }
                        }
                }

                //check for circular dependancy and replace with original
                for(int i=0;i<det.size();i++){
                        for(int j=0;j<attributes.size();j++){
                                if(DM[i][j] >= offset and DM_copy[i][j] == 1){
                                        bool cyc = true;
                                        int t = i;
                                        for(int k = 0; k < det.size(); k++){
                                                if(DM[t][j] < offset){
                                                        cyc = false;
                                                        break;
                                                }
                                                t = DM[t][j] - offset;
                                        }
                                        if(cyc){
                                                DM[i][j] = DM_copy[i][j];
                                        }
                                }
                        }
                }
        }

        
        //function to print path
        void print_path(){
                cout<<"Path Matrix for "<<name<<endl;
                for(int i=0;i<det.size();i++){
                        for(int j=0;j<det.size();j++){
                                cout<<path[i][j]<<" ";
                        }
                        cout<<endl;
                }
        }
        //function to print DM beautifully with column and row with value greater than or equal to offset replaced by the corresponding det
        void print_DM(){
                cout<<"Dependency Matrix for "<<name<<endl;
                cout<<" ";
                for(int i=0;i<attributes.size();i++){
                        cout<<" "<<attributes[i];
                }
                cout<<endl;
                for(int i=0;i<det.size();i++){
                        cout<<det[i]<<" ";
                        for(int j=0;j<attributes.size();j++){
                                if(DM[i][j]>=offset){
                                        cout<<det[DM[i][j]-offset]<<" ";
                                }else{
                                        cout<<DM[i][j]<<" ";
                                }
                        }
                        cout<<endl;
                }
        }
        //function to print DG beautifully with column and row
        void print_DG(){
                cout<<"DG for "<<name<<endl;
                cout<<" ";
                for(int i=0;i<det.size();i++){
                        cout<<det[i]<<" ";
                }
                cout<<endl;
                for(int i=0;i<det.size();i++){
                        cout<<det[i]<<" ";
                        for(int j=0;j<det.size();j++){
                                cout<<DG[i][j]<<" ";
                        }
                        cout<<endl;
                }
        }

        void preprocess(){
                fill_DM();
                print_DM();
                fill_DG();
                print_DG();
                mark_DG();
                print_path();
                re_fill_DM();
                print_DM();
        }
        void preprocess_no_print(){
                fill_DM();
                fill_DG();
                mark_DG();
                re_fill_DM();
        }




        //print the table
        void print(){
                cout<<"Table: "<<name<<endl;
                cout<<"Attributes: ";
                for(int i=0;i<attributes.size();i++){
                        cout<<attributes[i]<<" ";
                }
                cout<<endl;
                cout<<"Functional Dependencies: "<<endl;
                for(int i=0;i<fds.size();i++){
                        cout<<fds[i].det<<" -> "<<fds[i].dep<<endl;
                }
                cout<<endl;
        }
};

vector<Table> norm2NF(Table in){
        vector<Table> ans;
        vector<string> rem;
        for(int i = 0; i < in.det.size(); i++){
                if(in.det[i].size() > 1){
                        if(count(in.DM[i].begin(),in.DM[i].end(),0) == 0){
                                for(auto t: in.det[i]){
                                        string check = string(1,t);
                                        if(count(in.det.begin(),in.det.end(),check)){
                                                rem.push_back(in.det[i]);
                                                break;
                                        }
                                }
                        }
                }
        }
        for(auto t: rem){
                set<char> alpha;
                vector<FD> fds;
                for(auto fd: in.fds){
                        if(fd.det == t){
                                fds.push_back(fd);
                                for(auto tt: fd.dep){
                                        alpha.insert(tt);
                                }
                        }
                }
                string name = in.name +" "+ t;
                vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
                Table temp(name, alphab, fds);
                temp.preprocess_no_print();
                ans.push_back(temp);
        }
        set<char> alpha;
        vector<FD> fds;
        for(auto fd: in.fds){
                if(count(rem.begin(),rem.end(),fd.det) == 0){
                        fds.push_back(fd);
                        for(auto tt: fd.dep){
                                alpha.insert(tt);
                        }
                }
        }
        string name = in.name;
        vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
        Table temp(name, alphab, fds);
        temp.preprocess_no_print();
        ans.push_back(temp);
        return ans;
}

vector<Table> norm3NF(vector<Table> in){
        vector<Table> ans;
        for(auto tab_in : in){
                set<string> rem;
                for(int i = 0; i < tab_in.det.size(); i++){
                        for(int j =0 ; j < tab_in.attributes.size(); j++){
                                if(tab_in.DM[i][j] >= tab_in.offset){
                                        rem.insert(tab_in.det[tab_in.DM[i][j]-tab_in.offset]);
                                }
                        }
                }
                for(auto t: rem){
                        set<char> alpha;
                        vector<FD> fds;
                        for(auto fd: tab_in.fds){
                                if(fd.det == t){
                                        fds.push_back(fd);
                                        for(auto tt: fd.dep){
                                                alpha.insert(tt);
                                        }
                                }
                        }
                        string name = tab_in.name +" "+ t;
                        vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
                        Table temp(name, alphab, fds);
                        temp.preprocess_no_print();
                        ans.push_back(temp);
                }
                set<char> alpha;
                vector<FD> fds;
                for(auto fd: tab_in.fds){
                        if(count(rem.begin(),rem.end(),fd.det) == 0){
                                fds.push_back(fd);
                                for(auto tt: fd.dep){
                                        alpha.insert(tt);
                                }
                        }
                }
                string name = tab_in.name;
                vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
                Table temp(name, alphab, fds);
                temp.preprocess_no_print();
                ans.push_back(temp);
        }
        return ans;
}

vector<Table> normBCNF(vector<Table> in){
        vector<Table> ans;
        for(auto tab_in : in){
                set<string> rem;
                for(int i = 0; i < tab_in.det.size(); i++){
                        if(count(tab_in.DM[i].begin(),tab_in.DM[i].end(),0) > 0){
                                rem.insert(tab_in.det[i]);
                        }
                }
                for(auto t: rem){
                        set<char> alpha;
                        vector<FD> fds;
                        for(auto fd: tab_in.fds){
                                if(fd.det == t){
                                        fds.push_back(fd);
                                        for(auto tt: fd.dep){
                                                alpha.insert(tt);
                                        }
                                }
                        }
                        string name = tab_in.name +" "+ t;
                        vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
                        Table temp(name, alphab, fds);
                        temp.preprocess_no_print();
                        ans.push_back(temp);
                }
                set<char> alpha;
                vector<FD> fds;
                for(auto fd: tab_in.fds){
                        if(count(rem.begin(),rem.end(),fd.det) == 0){
                                fds.push_back(fd);
                                for(auto tt: fd.dep){
                                        alpha.insert(tt);
                                }
                        }
                }
                string name = tab_in.name;
                vector<char> alphab = vector<char>(alpha.begin(),alpha.end());
                Table temp(name, alphab, fds);
                temp.preprocess_no_print();
                ans.push_back(temp);
        }
        return ans;
}





int main(){
        //Table t1("Example 1",{'A','B','C','D','E','F','G'},{FD("A","BCD"),FD("C","DE"),FD("EF","DG"),FD("D","G")});//example 1
        //t1.print();                                                                                                                   
        //t1.preprocess();
        Table t2("Example 2",{'A','B','C','D','E','F','G','H','I','J','K','L'},{FD("A","BC"),FD("E","AD"),FD("G","AEJK"),FD("GH","FI"),FD("K","AL"),FD("J","K")});//example 2
        t2.print();
        t2.preprocess();
        vector<Table> tb = norm2NF(t2);
        //dotted line seperator of length 100
        cout << string(100,'-') << endl;
        cout<<"2NF: "<<endl;
        cout << string(100,'-') << endl;
        cout << "Number of tables: " << tb.size() << endl;
        for(auto t: tb){
                t.print();
        }
        tb = norm3NF(tb);
        cout << string(100,'-') << endl;
        cout<<"3NF: "<<endl;
        cout << string(100,'-') << endl;
        cout << "Number of tables: " << tb.size() << endl;
        for(auto t: tb){
                t.print();
        }
        tb = normBCNF(tb);
        cout << string(100,'-') << endl;
        cout<<"BCNF: "<<endl;
        cout << string(100,'-') << endl;
        cout << "Number of tables: " << tb.size() << endl;
        for(auto t: tb){
                t.print();
        }
        cout << string(100,'-') << endl;
        for(auto t: tb){
                t.print_DM();
        }
}


