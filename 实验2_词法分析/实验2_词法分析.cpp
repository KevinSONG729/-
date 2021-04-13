#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#define MAX 100000
using namespace std;

struct words {
    string type;
    string word;
};

string readFile() {
    char temp[1024] = { 0 };
    string data;
    ifstream infile;
    infile.open("input.txt");
    infile.getline(temp, MAX, 0);
    infile.close();
    data = temp;
    transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}

vector<string> Match(string data) {
    smatch m;
    string k("");
    regex e1("([0-9|$|!|%|@|//^|//~||//、]+[a-z|_]+)+",regex_constants::icase);
    // |\\+|-|\\*|/|=|#|<=?|>=?|:=
    // ([a-z]+[0-9]*)+|[0-9]+|\\(|\\)|,|;|.|\\+|-|\\*|/|=|#|<=?|>=?|:=
    regex e2("([a-z|_]+[0-9]*)+|[0-9]+\\.?[0-9]*|\\(|\\)|,|;|\\.|[\\+|-|\\*|/|:|#|<|>|=]+", regex_constants::icase);

    regex e3("\\r|\\n");
    data = regex_replace(data, e3, k);

    bool err_flag = regex_search(data, m, e1);
    if (err_flag == true) {
        cout << m[0]<< " is an invalid ident!"<<endl;
        exit(0);
    }

    vector<string> matched;
    sregex_iterator beginitr(data.cbegin(), data.cend(), e2);
    sregex_iterator enditr;
    for (sregex_iterator itr = beginitr; itr != enditr; ++itr) {
        if (itr->str() != " " && itr->str() != "\n") {
            cout << itr->str() << endl;
            matched.push_back(itr->str());
        }
    }
    /*for (auto &i : matched) {
        cout << i;
    }*/
    return matched;
}

vector<words> group(vector<string> matched, words w[]) {
    vector<words> res;
    regex e1("[0-9]+");
    regex e2("([a-z|_]+[0-9]*)+", regex_constants::icase);
    regex e3("[\\+|-|\\*|/|:|#|<|>|=]+");
    regex e4("[0-9]+\\.[0-9]+");
    regex e[4] = { e1,e2,e3,e4 };
    for (auto& i : matched) {
        bool flag = 0;
        for (int j = 0; j < 29; j++) {
            if (i == w[j].word) {
                res.push_back(w[j]);
                flag = 1;
                break;
            }
        }
        if (!flag) {
            bool p = false;
            for (int j = 0; j < 4; j++) {
                p = regex_match(i, e[j]);
                if (p) {
                    switch (j) {
                    case 0: res.push_back({ "number",i }); break;
                    case 1: res.push_back({ "ident",i }); break;
                    case 2: {
                        cout << i<< " is an invalid symbol!" << endl;
                        exit(0);
                        }
                    case 3: {
                        cout << i << " is an invalid number!" << endl;
                        exit(0);
                        }
                    }
                }
            }
        }
    }
    return res;
}

void display(vector<words> res) {
    for (auto& i : res) {
        cout <<"("<< i.type << "," << i.word <<")"<< endl;
    }
    ofstream outfile;
    outfile.open("output.txt");
    for (auto& i : res) {
        string x = "(" + i.type + "," + i.word + ")\n";
        outfile << x;
    }
    outfile.close();
}

int main()
{
    words w[] = { {"beginsym","begin"}, {"callsym","call"}, {"constsym","const"}, {"dosym","do"},
        {"endsym","end"}, {"ifsym","if"}, {"oddsym","odd"}, {"proceduresym","procedure"}, {"readsym","read"},
        {"thensym","then"}, {"varsym","var"}, {"whilesym","while"}, {"writesym","write"}, {"plus","+"}, {"minus","-"},
        {"times","*"}, {"slash","/"}, {"eql","="}, {"neq","#"}, {"lss","<"}, {"leq","<="}, {"gtr",">"}, {"geq",">="},
        {"becomes",":="}, {"Lparen","("}, {"rparen",")"}, {"comma",","}, {"semicolon",";"}, {"period","."} };
    string data = readFile();
    // cout << data<< endl;
    vector<string> matched = Match(data);
    vector<words> res = group(matched, w);
    display(res);
}
