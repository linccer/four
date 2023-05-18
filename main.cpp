#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <iterator>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <locale>
#include <algorithm>
using std::multimap;
using std::make_pair;
using std::set;
using std::less;
using std::string;
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::locale;

void output (multimap<  string, int, less<string>  > &words, set<  string, less<string>  > &urls);
void readExtensions (vector<string> &e);

vector <string> special{"ą", "Ą", "č", "Č", "ę", "Ę", "ė", "Ė", "į", "Į", "š", "Š", "ų", "Ų", "ū", "Ū", "ž", "Ž"};

int main(){

    vector<string> extensions;
    readExtensions(extensions);

    multimap<  string, int, less<string>  > words;
    set<  string, less<string>  > urls;

    ifstream file("text.txt", std::ios_base::binary);
    file.imbue(locale());
    try {if (!file) throw std::runtime_error("Failas nerastas.");
    } catch (std::exception &e) { std::cerr<<e.what()<<endl; return 0;};

    string line="";
    int lc=0; //line count
    string w;
    int wc=0; //word count
    short int dot=-1;
    

    while (getline (file, line)){
        lc++;
        stringstream ss(line);
        while(ss>>w){
            wc++;

            // find urls
            dot!=-1;
            dot=w.find('.');
            
            if (dot!=-1){
                for (auto e:extensions){
                    dot=w.find(e);
                    if (dot!=-1){
                    urls.insert(w);
                    break;
                    }//if
                }//for (extension checking)

            }//if (found dot)


            if (dot==-1) {
            //remove punctuation
            w.erase(std::remove(w.begin(), w.end(), '.'), w.end());
            w.erase(std::remove(w.begin(), w.end(), ','), w.end());
            w.erase(std::remove(w.begin(), w.end(), '('), w.end());
            w.erase(std::remove(w.begin(), w.end(), ')'), w.end());
            w.erase(std::remove(w.begin(), w.end(), ':'), w.end());
            w.erase(std::remove(w.begin(), w.end(), ';'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '%'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '-'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '+'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '/'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '['), w.end());
            w.erase(std::remove(w.begin(), w.end(), ']'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '0'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '1'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '2'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '3'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '4'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '5'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '6'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '7'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '8'), w.end());
            w.erase(std::remove(w.begin(), w.end(), '9'), w.end());
            w.erase(std::remove(w.begin(), w.end(), ' '), w.end());

            if (w.length()==0) continue;
            if (w.find("–")!=-1 && w.length()==3) continue;
            
            //words
            w[0]=toupper(w[0]); 
            words.insert(make_pair(w, lc)); }

        }//one line
    }//lines
    file.close();


    cout<<"\nThe document has "<<lc<<" lines"<<endl;
    cout<<"The document has "<<wc<<" words (including urls)"<<endl;
    cout<<"The document has "<<words.size()<<" words"<<endl;
    cout<<"The document has "<<urls.size()<<" urls\n"<<endl;


    output(words, urls);
    words.clear();
}





void output (multimap<  string, int, less<string>  > &words, set<  string, less<string>  > &urls){

    ofstream output("output.txt");
    output.imbue(locale());
    string outs="";

    //urls
    outs+="URLS:\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    set<string>::iterator iter;
    for (iter = urls.begin(); iter!=urls.end();++iter) outs+=(*iter+"\n");
    output<<outs;
    outs="";


    //words used more than once
    output<<"\n\n\n\nCross-reference table:\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"<<left<<setw(30)<<"Word"<<setw(15)<<"Instances"<<"Lines found"<<"\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";

    string lastword="";
    string line="";
    short int n=0;

    for(auto word:words){

        if (words.count(word.first)<2) continue;

        else if (lastword == word.first) { outs+=", "+std::to_string(word.second);   continue; }

        else { //new word
            lastword=word.first;
            n=0;

            //check for special chars
            short int spec=-2;
            for (auto s:special){
                spec=word.first.find(s);
                if (spec!=-2) {
                while (n<10){
                    if (spec==-1) {break;}
                    n++;
                    spec=word.first.find(s, spec+1);
                }//while
                }//if (found 1 special char)   
            }//for special

            stringstream ss;
            ss<<left<<setw(34+n)<<word.first<<setw(12)<<words.count(word.first)<<word.second;

            getline(ss, line);
            outs+="\n"+line;
            line="";
        }//else (new word)
    }//for (auto words)


    output << outs;
    output.close();
};





void readExtensions (vector<string> &extensions){

    ifstream efile("extensions.txt", std::ios_base::binary);
    efile.imbue(locale());
    string e="";

    while(!efile.eof()){
        efile>>e;
        extensions.push_back(e);
    }

    efile.close();
    extensions.shrink_to_fit();
};