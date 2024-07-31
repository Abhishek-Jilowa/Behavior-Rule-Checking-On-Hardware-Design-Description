#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int main() {
    // Open the Verilog file
    std::ifstream verilogFile("RULE3_a_control_fsm_rtl_T400.vhd");

    // Check if the file is open
    if (!verilogFile.is_open()) {
        std::cerr << "Error opening Verilog file." << std::endl;
        return 1;
    }

    // Define the regex patterns for the strings you want to detect
    std::regex pattern("(Trj|Trojan_Trig|Tj1|Tj2|Trojan_rom_data_i|Trigger|Trojan_\\*)");

    // Iterate through each line in the Verilog file
    std::string line;
    while (std::getline(verilogFile, line)) {
        // Search for the regex pattern in the current line
        std::smatch match;
        if (std::regex_search(line, match, pattern)) {
            // Print the line containing the matched string
            std::cout << "Match found in line: " << line << std::endl;
        }
    }

    // Close the Verilog file
    verilogFile.close();

    return 0;
}
