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
#include <iomanip>

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
    Util::read_directory(path, ls);
    std::regex self_regex("[0-9]+");
    for ( string ent : ls ) {
        if(std::regex_search(ent, self_regex)) {
            pids.push_back(ent);
        }
    }
    return pids;

}

string ProcessParser::getVmSize(string pid) {
    string path=Path::basePath()+pid+Path::statusPath();
    ifstream stream;
    Util::getStream(path,stream);
    string output;
    string line;
    std::regex self_regex("VmSize");
    while ( getline(stream,line) ) {
        if( std::regex_search(line, self_regex) ) {
            output=output+line;
        }
    }
    return output;
}

std::string ProcessParser::getCpuPercent(string pid) {
    // Note the result is wrong on my test container because uptime for a docker container is
    // the host uptime not the container uptime. 
    // 
    string path=Path::basePath()+pid+"/"+Path::statPath();

    ifstream stream;
    Util::getStream(path,stream);

    string line;    
    getline(stream,line);

    vector<string> vline=Util::split (line," ");
    double jiffie_cputime=stod(vline[13])+stod(vline[14]);
    double Hertz = (double) sysconf(_SC_CLK_TCK);
    double jiffie_process_age=(double)ProcessParser::getSysUpTime()*Hertz - stod(vline[21])+stod(vline[14]);
    
    double cpu_percent=jiffie_cputime/jiffie_process_age*100.00; 
	
    // All the rest is to format the double to 2 digits precision. I hope there is an easier way...
    // Create an output string stream
    std::ostringstream streamObj3;
 
    // Set Fixed -Point Notation
    streamObj3 << std::fixed;
 
    // Set precision to 2 digits
    streamObj3 << std::setprecision(2);
 
    //Add double to stream
    streamObj3 << cpu_percent;
 
    // Get string from output string stream
    std::string strObj3 = streamObj3.str();

    return strObj3;
}

long int ProcessParser::getSysUpTime() {
    string path=Path::basePath()+Path::upTimePath();

    ifstream stream;
    Util::getStream(path,stream);

    string line;    
    getline(stream,line);

    vector<string> vline=Util::split (line," ");
    long int time=stol(vline[0]);

    return time;
}