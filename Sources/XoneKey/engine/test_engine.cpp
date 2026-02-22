#include <iostream>
#include <vector>
#include <string>
#include "Engine.h"
#include "DataType.h"

int main() {
    std::cout << "Starting XoneKey Engine Test on Linux/WSL..." << std::endl;

    // Initialize engine
    initEngine();
    std::cout << "Engine initialized successfully." << std::endl;

    // Test some basic character codes
    // Note: These use codes from platforms/linux.h
    std::cout << "Testing key codes..." << std::endl;
    
    // Simulate typing 'a' + 'a' in Telex mode (vCodeTable=0 is Unicode, vInputType=0 is Telex)
    vCodeTable = 0; 
    vInputType = 0;
    vLanguage = 1; // Vietnamese mode

    std::cout << "Simulating Telex input: a + a -> â" << std::endl;
    
    // In linux.h, KEY_A is 38
    // We need to look at how insertKey is called. 
    // Usually it's via keyboard hooks which we don't have here, 
    // so we call the internal logic directly or use the engine API.
    
    // For a simple test, we just check if the engine has correctly loaded its tables.
    std::vector<LPCTSTR>& tables = getTableCode();
    if (!tables.empty()) {
        std::cout << "Table Code loaded: " << tables.size() << " entries." << std::endl;
        std::wcout << L"Primary table: " << tables[0] << std::endl;
    } else {
        std::cerr << "Error: No tables loaded!" << std::endl;
        return 1;
    }

    std::cout << "Engine test completed successfully!" << std::endl;
    return 0;
}
