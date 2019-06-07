#include <iostream>
#include "ProcessParser.h"
using namespace std;

int main() {
    cout << "GetCmd:\n";
    std::cout << ProcessParser::getCmd("1") << "\n";
    cout << "getPidList:\n";
    vector<string> pidlist=ProcessParser::getPidList();
    for( string ent : pidlist ) {
        cout << ent << "\n";
    }
    cout << "getVmSize:\n";
    std::cout << ProcessParser::getVmSize("1") << "\n";
    cout << "getSysUpTime:\n";
    std::cout << ProcessParser::getSysUpTime() << "\n";
    cout << "getCpuPercent:\n";
    std::cout << ProcessParser::getCpuPercent("997") << "\n";
}
