#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <string>
using namespace std;
void computeLPSArray(string pat, int M, int lps[])
{
    int len = 0;
    int i = 1;
    lps[0] = 0;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = len;
                i++;
            }
        }
    }
}

int KMPSearch(string pat, string txt)
{
    int M = pat.length();
    int N = txt.length();
    int lps[M];
    int j = 0; 

    computeLPSArray(pat, M, lps);

    int i = 0;
    int res = 0;
    int next_i = 0;

    while (i < N)
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }
        if (j == M)
        {
            j = lps[j - 1];
            res++;
        }

        else if (i < N && pat[j] != txt[i])
        {
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    return res;
}

string removeSpaces(string str) 
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
} 

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
        int count_and_or = 0;
        string condition = "";
        if (constructType != "else")
        {
            for (int i = constructType.length(); i < constructContent.length(); i++)
            {
                if (constructContent[i] == 't' && constructContent[i + 1] == 'h' && constructContent[i + 2] == 'e' && constructContent[i + 3] == 'n')
                {
                    break;
                }
                condition += constructContent[i];
            }
        }
        condition=removeSpaces(condition);
        int flag = KMPSearch("s_command(7downto3)=MOV_RR_DATA", condition);
        if (flag)
        {
            std::cout << "Found " << constructType << " construct:\n";
            std::cout << "Condition: " << constructContent << "\n\n";
            pos = endPos + 1;
        }
        else
        {
            pos += constructType.length();
        }
    }
    
}

int main()
{
    std::string inputFileName = "RULE1___control_mem_rtl_T300.vhd.txt"; // Replace with your input file name
    std::string outputFileName = "remove_comment" + inputFileName;  // Replace with your output file name

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open input file: " << inputFileName << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open output file: " << outputFileName << std::endl;
        return 1;
    }

    std::string line;
    std::string code;
    bool inBlockComment = false;

    while (std::getline(inputFile, line))
    {
        // Remove line comments (--) and block comments (/* ... */)
        std::regex commentRegex("--.*|/\\*.*?\\*/");
        code += std::regex_replace(line, commentRegex, "") + '\n';

        if (inBlockComment)
        {
            // Check for the end of a block comment (*/)
            if (line.find("*/") != std::string::npos)
            {
                inBlockComment = false;
            }
        }
        else
        {
            // Check for the start of a block comment (/*)
            if (line.find("/*") != std::string::npos)
            {
                inBlockComment = true;
            }
        }
    }
    outputFile << code;

    inputFile.close();
    outputFile.close();


    ifstream vhdlFile(outputFileName);
    if (!vhdlFile)
    {
        cerr << "Error: Could not open VHDL file." << endl;
        return 1;
    }
    string vhdlCode((istreambuf_iterator<char>(vhdlFile)), istreambuf_iterator<char>());
    analyzeVHDLCode(vhdlCode);

    return 0;
}
