#include <iostream>
#include "ProcessParser.h"
using namespace std;

int main(int argc, char* argv[]) {
    string pid=argv[1];
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
    std::cout << ProcessParser::getCpuPercent(pid) << "\n";
    cout << "getProcUptime:\n";
    std::cout << ProcessParser::getProcUpTime(pid) << "\n";
    cout << "getProcUser:\n";
    std::cout << ProcessParser::getProcUser(pid) << "\n";
    cout << "getSysCpuPercent():\n";
    std::string result;
    for (auto const& s : ProcessParser::getSysCpuPercent()) { result += " "+s; }
    std::cout << result << "\n";
    cout << "getSysCpuPercent(\"1\"):\n";
    result="";
    for (auto const& s : ProcessParser::getSysCpuPercent("1")) { result += " "+s; }
    std::cout << result << "\n";
    cout << "getSysRamPercent():\n";
    std::cout << ProcessParser::getSysRamPercent() << "\n";
    cout << "getSysKernelVersion():\n";
    std::cout << ProcessParser::getSysKernelVersion() << "\n";
}
