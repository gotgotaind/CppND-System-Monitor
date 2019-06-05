#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"
#include "util.h"
#include <regex>

using namespace std;

class ProcessParser{
private:
    std::ifstream stream;
    public:
    static string getCmd(string pid);
    static vector<string> getPidList();
    static std::string getVmSize(string pid);
    static std::string getCpuPercent(string pid);
    static long int getSysUpTime();
    static std::string getProcUpTime(string pid);
    static string getProcUser(string pid);
    static vector<string> getSysCpuPercent(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
    static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:
string ProcessParser::getCmd(string pid) {
    string path=Path::basePath()+pid+Path::cmdPath();
    ifstream stream;
    Util::getStream(path,stream);
    string output;
    string line;
    while ( getline(stream,line) ) {
        output=output+line;
    }
    return output;
}

vector<string> ProcessParser::getPidList() {
    string path=Path::basePath();
    vector<string> ls;
    vector<string> pids;
    read_directory(path, ls);
    std::regex self_regex("[0-9]+");
    for ( string ent : ls ) {
        if(std::regex_search(ent, self_regex)) {
            pids.push_back(ent);
        }
    }
    return pids;

}