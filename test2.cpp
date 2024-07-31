#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <string>
using namespace std;

void analyzeVHDLCode(const std::string &vhdlCode)
{
    std::string::size_type pos = 0;

    while (pos < vhdlCode.length())
    {
        std::string::size_type ifPos = vhdlCode.find("if ", pos);
        std::string::size_type elsifPos = vhdlCode.find("elsif ", pos);
        std::string::size_type elsePos = vhdlCode.find("else", pos);

        std::string::size_type constructPos;
        std::string constructType;
        if (ifPos != std::string::npos && (elsifPos == std::string::npos || ifPos < elsifPos) && (elsePos == std::string::npos || ifPos < elsePos))
        {
            constructPos = ifPos;
            constructType = "if";
        }
        else if (elsifPos != std::string::npos && (elsePos == std::string::npos || elsifPos < elsePos))
        {
            constructPos = elsifPos;
            constructType = "elsif";
        }
        else if (elsePos != std::string::npos)
        {
            constructPos = elsePos;
            constructType = "else";
        }
        else
        {
            break;
        }

        std::string::size_type endPos = vhdlCode.find("end if;", constructPos);
        if (endPos == std::string::npos)
        {
            std::cerr << "Error: Missing 'end if;' for " << constructType << " construct.\n";
            return;
        }

        std::string constructContent = vhdlCode.substr(constructPos, endPos - constructPos + 7);
        int flag = 0;
        std::regex assignmentRegex(R"(\s*(\w+)\s*<=\s*(.*?);)");
        // std::smatch matches;
        // std::string::const_iterator searchStart(constructContent.cbegin());
        // while (std::regex_search(searchStart, constructContent.cend(), matches, assignmentRegex))
        // {

        //     // cout<<matches.str(1)<<endl;

        //     // for (int i = 0; i < matches.str(2).length(); i++)
        //     // {
        //     //     if (matches.str(2)[i] != '1' || matches.str(2)[i] != '0')
        //     //     {
        //     //         std::cout << "Found assignment: " << matches[0] << "\n";
        //     //         std::cout << "Signal: " << matches[1] << ", Value: " << matches[2] << "\n";
        //     //         flag = 1;
        //     //     }
        //     // }
        //     searchStart = matches.suffix().first;
        // }

        string condition = "";
        for (int i = 0; i < constructContent.length()-4; i++)
        {
            if (constructContent[i] == 'r' && constructContent[i + 1] == 'e' && constructContent[i + 2] == 's' && constructContent[i + 3] == 'e' && constructContent[i + 4] == 't')
            {
                flag=1;
                break;
            }
        }
        // cout<<condition<<endl;

        // std::smatch matches;
        // std::string::const_iterator searchStart(condition.cbegin());
        // while (std::regex_search(searchStart, condition.cend(), matches, assignmentRegex))
        // {
        //     cout<<matches.str(1)<<endl;
        //     if (matches.str(1) == "reset")
        //     {
        //         flag = 1;
        //         break;
        //     }
        //     searchStart = matches.suffix().first;
        // }
        if (flag == 1)
        {
            std::cout << "Found " << constructType << " construct:\n";
            std::cout << "Condition: " << constructContent << "\n\n";
        }
        pos = endPos + 1;
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

    analyzeVHDLCode(vhdlCode);

    return 0;
}