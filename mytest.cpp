#include <iostream>
#include "ProcessParser.h"
using namespace std;

int main() {
    std::cout << ProcessParser::getCmd("1") << "\n";
    vector<string> pidlist=ProcessParser::getPidList();
    for( string ent : pidlist ) {
        cout << ent << "\n";
    }
}
