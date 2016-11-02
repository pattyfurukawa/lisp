#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <functional>

using namespace std;

struct node {
    string data;
    vector<node> nodes;

};

bool isWhitespace(const string& ent) {
    if (ent == " " || ent == "\t" || ent == "\n")
        return true;
    else
        return false;
}

bool isParenthesis(const string& ent) {
    if (ent == "(" || ent == ")")
        return true;
    else
        return false;
}

bool isNumber(const string& ent) {
    char * p;
    strtol(ent.c_str(), &p, 10);
    return (*p == 0);
}

int add(vector<node> values) {
    int s = 0;
    for (size_t i = 0; i<values.size(); i++)
        s += stoi(values[i].data);
    return s;
}

int substract(vector<node> values) {
    if (values.size() > 2) {
        cout << "Error in Substraction: Too Many Values" << endl;
    }
    return (stoi(values[0].data) - stoi(values[1].data));
}

int divide(vector<node> values) {
    if (values.size() > 2) {
        cout << "Error in Division: Too Many Values" << endl;
    }
    return (stoi(values[0].data) / stoi(values[1].data));
}

int multiply(vector<node> values) {
    int s = 1;
    for (size_t i = 0; i<values.size(); i++)
        s = s *  stoi(values[i].data);
    return s;
}

int greatthan(vector<node> values) {
    if (values.size() > 2) {
        cout << "Error in Comparision: Too Many Values" << endl;
        return -1;
    }
    return (stoi(values[0].data) > stoi(values[1].data));
}

int lessthan(vector<node> values) {
    if(values.size() > 2) {
        cout << "Error in Comparision: Too Many Values" << endl;
        return -1;
    }
    return (stoi(values[0].data) < stoi(values[1].data));
}

int greatequal(vector<node> values) {
    if(values.size() > 2) {
        cout << "Error in Comparision: Too Many Values" << endl;
        return -1;
    }
    return (stoi(values[0].data) >= stoi(values[1].data));
}

int lessequal(vector<node> values) {
    if(values.size() > 2) {
        cout << "Error in Comparision: Too Many Values" << endl;
        return -1;
    }
    return (stoi(values[0].data) <= stoi(values[1].data));
}

int equalequal(vector<node> values) {
    if(values.size() > 2) {
        cout << "Error in Comparision: Too Many Values" << endl;
        return -1;
    }
    return (stoi(values[0].data) == stoi(values[1].data));
}

string NumbertoString(int n) {
    stringstream convert;
    convert << n;
    return convert.str();
}

bool isIf(const string& ent) {
    if(ent == "if")
        return 1;
    else
        return 0;
}

int solveNode(const node& entry) {
    unordered_map<string, function<int(vector<node>)>> funct = { {"+", add}, {"-", substract}, {"*", multiply}, {"/", divide}, {">", greatthan}, {"<", lessthan}, {">=", greatequal}, {"<=", lessequal}, {"==", equalequal} };
    vector<node> elem;
    if(entry.data.empty()) {
        auto a = funct.find(entry.nodes[0].data);
        if(a == funct.end()) {
            if (isIf(entry.nodes[0].data)) {
                if (solveNode(entry.nodes[1])) {
                    return (solveNode(entry.nodes[2]));
                } else {
                    if (entry.nodes[3].nodes.size() == 0)
                        return 0;
                    else
                        return (solveNode(entry.nodes[3]));
                }
            } else {
                cout << "Function not defined" << endl;
                return 0;
            }
        } else {
            for(size_t i = 1; i<entry.nodes.size(); i++){
                if(entry.nodes[i].data.empty()) {
                    node n;
                    n.data = NumbertoString(solveNode(entry.nodes[i]));
                    elem.push_back(n);
                }
                else {
                    elem.push_back(entry.nodes[i]);
                }
            }
            return a->second(elem);
        }
    } else {
        cout << "Error: Wrong Entry" << endl;
        return 0;
    }
}

string checkNode(const string& entry) {
    if(isWhitespace(entry.substr(0,1)))
        cout << "Error: Invalid whitespace" << endl;
    if(entry.substr(0,1) == "(") {
        string curr;
        int count = 1;
        for (size_t i = 1; i<entry.size(); i++) {
            curr = entry.substr(i,1);
            if (curr == "(") count += 1;
            if (curr == ")") count -= 1;
            if (count == 0)
                return entry.substr(0,i+1);
        }
        cout << "Error: Unbalanced parenthesis" << endl;
    }
    else {
        string curr;
        string token;
        for (size_t i = 0; i<entry.size(); i++) {
            curr = entry.substr(i,1);
            if (isWhitespace(curr) || isParenthesis(curr)){
                return token;
            }
            else
                token.append(curr);
        }
    }
    return " ";
}

node parser(const string& entry) {
    node n;
    if (entry.substr(0,1) != "(") {
        cout << "Error: Invalid Entry. Missing Opening Parenthesis" << endl;
        return n;
    }
    for(size_t i = 1; i<entry.size(); i++) {
        string curr = entry.substr(i,1);
        string token;
        if (isWhitespace(curr)) {
            continue;
        } else if (curr == "(") {
            token = checkNode(entry.substr(i));
            i += token.size();
            n.nodes.push_back(parser(token));
        } else if (curr == ")") {
            cout << "Extra Closing Parenthesis Found" << endl;
        } else {
            token = checkNode(entry.substr(i));
            i += token.size();
            node t;
            t.data = token;
            n.nodes.push_back(t);
        }
    }
    return n;
}

string printNode(const node& entry) {
    string r = "";
    if (entry.data.empty()) {
        r += "(";
        for(size_t i = 0; i<entry.nodes.size(); i++) {
            if(!entry.nodes[i].data.empty()) {
                r += entry.nodes[i].data;
                r += " ";
            } else
                r += printNode(entry.nodes[i]);
        }
        r += ")";
    }
    return r;
}

int main() {
    node example;
    //string entry = "(/ (+ 5 5) (- 5 3))";
    string entry = "(if (> 3 1) (- 20 5) (+ 2 6))";
    example = parser(entry);
    cout << solveNode(example) << endl;
}
