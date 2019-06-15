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
#include <pwd.h>
#include <sys/stat.h>



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
    vector<string> vline=Util::pid_stat_vec(pid);
    double jiffie_cputime=stod(vline[13])+stod(vline[14]);
    double Hertz = (double) sysconf(_SC_CLK_TCK);
    double jiffie_process_age=(double)ProcessParser::getSysUpTime()*Hertz - stod(vline[21]);
    
    double cpu_percent=jiffie_cputime/jiffie_process_age*100.00; 
	
    // All the rest is to format the double to 2 digits precision. I hope there is an easier way...
    // Create an output string stream
    std::ostringstream streamObj3;
    streamObj3 << std::fixed << std::setprecision(2) << cpu_percent;
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

std::string ProcessParser::getProcUpTime(string pid) {
    vector<string> vline=Util::pid_stat_vec(pid);
    double Hertz = (double) sysconf(_SC_CLK_TCK);
    double jiffie_process_age=(double)ProcessParser::getSysUpTime()*Hertz - stod(vline[21]);
	
    return to_string(jiffie_process_age/Hertz);
}

string ProcessParser::getProcUser(string pid) {
    string path=Path::basePath()+pid;

    // convert path to a C string
    char * c_path = new char(path.length()+1);
    strcpy(c_path,path.c_str());

    struct stat info;
    stat(c_path, & info);
    struct passwd *pw = getpwuid(info.st_uid);
    return pw->pw_name;
}


vector<string> ProcessParser::getSysCpuPercent(string coreNumber) {
    string path=Path::basePath()+Path::statPath();
    ifstream stream;
    Util::getStream(path,stream);
    string output;
    string line;
    std::regex self_regex("cpu"+coreNumber+" ");
    while ( getline(stream,line) ) {
        if( std::regex_search(line, self_regex) ) {
            output=output+line;
        }
    }
    return Util::split(output," ");
}

float ProcessParser::getSysRamPercent(){
    string path=Path::basePath()+Path::memInfoPath();

    ifstream stream;
    Util::getStream(path,stream);
    
    string MemFree_s;
    string MemTotal_s;
    std::regex MemFree_regex("MemFree:\\s*(\\d*) .*");
    std::regex MemTotal_regex("MemTotal:\\s*(\\d*) .*");
    std::smatch matches;

    string line;
    while ( getline(stream,line) ) {
        if( std::regex_search(line, matches, MemFree_regex) ) {
            MemFree_s=matches[1];
            //cout << "MemFree: " << MemFree_s << "\n";
        }
        if( std::regex_search(line, matches, MemTotal_regex) ) {
            MemTotal_s=matches[1];
            //cout << "MemTotal: " << MemTotal_s << "\n";
        }
    }

    double mem_total=stod(MemTotal_s);
    double mem_free=stod(MemFree_s);
    return (mem_total-mem_free)/mem_total*100;
}

string ProcessParser::getSysKernelVersion() {
    string path=Path::basePath()+Path::versionPath();

    ifstream stream;
    Util::getStream(path,stream);

    string line;    
    getline(stream,line);

    vector<string> vline=Util::split (line," ");
    return vline[2];

}

int ProcessParser::getTotalThreads() {

    // Initializing the result int to 0
    int total_threads=0;
    // Iterating over all pids
    for(string pid:getPidList()) {
        // Building the path to the pid /proc/pid/status file
        string path=Path::basePath()+pid+Path::statusPath();
        //cout << "Parsing " << path << "\n";

        // Getting a stream to /proc/pid/status file
        ifstream stream;
        Util::getStream(path,stream);
       
        // Defining a regular expression to find the number of threads
        // btw \s is 'space' class character, that is space or tab 
        // or could be unicode spaces if this std::regexp lib supports it?
        // and \d is 'digit'
        // also the \ in \s and \d must be escaped, thus the \\s and \\d
        // there's probably a way to avoid that to gain in clariry
        // because it's not like regexp can already get complicated
        // without having to escape backslashes...
        std::regex Threads_regex("Threads:\\s*(\\d*)");

        // A structure that will hold the strings matching the regular expression grouping (...)
        std::smatch matches;

        // Iterating on the stream lines
        string line;
        while ( getline(stream,line) ) {
            if( std::regex_search(line, matches, Threads_regex) ) {
                // Not sure why matches[1] is the good match. I suppose match[0] is the whole line?
                total_threads=total_threads+stoi(matches[1]);
                //cout << "MemTotal: " << MemTotal_s << "\n";
            }
        }         
    }
    return total_threads;
}

int ProcessParser::getTotalNumberOfProcesses()
{
    // Note that /proc/status "processes" line is wrong on my docker for windows, it's in the order of thousands
    // while my host system does have about 200 and my container about 10
    // Don't know where it gets this number from...
    int total_procs=0;
    // Iterating over all pids
    for(string pid:getPidList())
    {
        total_procs++;
    }
    return total_procs;
}

int ProcessParser::getNumberOfRunningProcesses()
{
        string path=Path::basePath()+Path::statPath();

        ifstream stream;
        Util::getStream(path,stream);
 
        std::regex procs_running_regex("procs_running\\s*(\\d*)");
        std::smatch matches;

        string line;
        while ( getline(stream,line) ) {
            if( std::regex_search(line, matches, procs_running_regex) ) {
                return stoi(matches[1]);
            }
        }         
}

string ProcessParser::getOSName()
{
    string path="/etc/os-release";
    ifstream stream;
    Util::getStream(path,stream);

    std::regex PRETTY_NAME_regex("PRETTY_NAME=\"(.*)\"");
    std::smatch matches;

    string line;
    while ( getline(stream,line) ) {
        if( std::regex_search(line, matches, PRETTY_NAME_regex) ) {
            return (matches[1]);
        }
    }         
}

string ProcessParser::PrintCpuStats(vector<string> values1, vector<string> values2)
{
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);
}