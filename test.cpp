#include <iostream>
#include <fstream>
#include <regex>
#include <string>
using namespace std;


void analyzeVHDLCode(const std::string &vhdlCode)
{
    std::regex assignmentRegex(R"(\s*(\w+)\s*<=\s*(.*?);)");
    std::smatch matches;
    std::string::const_iterator searchStart(vhdlCode.cbegin());
    while (std::regex_search(searchStart, vhdlCode.cend(), matches, assignmentRegex))
    {
        std::cout << "Found assignment: " << matches[0] << "\n";
        std::cout << "Signal: " << matches[1] << ", Value: " << matches[2] << "\n";
        searchStart = matches.suffix().first;
    }
}

int main()
{
    ifstream vhdlFile("RULE1___control_mem_rtl_T300.vhd.txt");
    if (!vhdlFile)
    {
        cerr << "Error: Could not open VHDL file." << endl;
        return 1;
    }
    string vhdlCode((istreambuf_iterator<char>(vhdlFile)), istreambuf_iterator<char>());
    string s="s_intpre<='1'; state='0';",s1="if Rising_Edge(clk) then state <= s_nextstate;";
    // analyzeVHDLCode(s);
    vector<int> idx;
    for(int i=0;i<s.size();i++){
        if(s[i]=='=') idx.push_back(i);
    }
    int f=0;
    for(int i=0;i<idx.size();i++){
        if(s[idx[i]+2]=='1' or s[idx[i]+2]=='0'){
            if(s[idx[i]+3]!='1' and s[idx[i]+3]!='0'){
                f=1;
                break;
            }
        }
    }
    

    return 0;
}