#include "util_time.h"
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

Time::Time() {
    SetNow();
}

Time::Time(const Time& rhs) {
    (*this) = rhs;
}

Time::Time(const std::string& timestamp) {
    (*this) = timestamp;
}

Time& Time::operator=(const Time& rhs) {
    this->tv = rhs.tv;
    this->time = rhs.time;
    return *this;
}

Time& Time::operator=(const std::string& timestamp) {

    /*
     * Initialise the time to Midnight on the 1st of January 1970
     * ----------------------------------------------------------
     */
    struct tm epoch;
    epoch.tm_sec = 0;
    epoch.tm_min = 0;
    epoch.tm_hour= 0;
    epoch.tm_mday= 1;
    epoch.tm_mon=1;
    epoch.tm_year=0;
    epoch.tm_isdst=-1; // <0 = unknown
    //epoch.tm_wday: Not used by mktime
    //epoch.tm_yday: Not used by mktime
    time = epoch;

    /*
     * Pull apart the string
     * ---------------------
     *  Exected format: YYYYMMDD HH:MM:SS.UUUUUU
     *  Index:          012345678901234567890123
     *  (Until gcc 4.9 we can't regex our way out of this :( )
     */
    std::stringstream YEAR(timestamp.substr(0,4), ios_base::in);
    std::stringstream MONTH(timestamp.substr(4,2), ios_base::in);
    std::stringstream DAY(timestamp.substr(6,2), ios_base::in);
    std::stringstream HOUR(timestamp.substr(9,2), ios_base::in);
    std::stringstream MIN(timestamp.substr(12,2), ios_base::in);
    std::stringstream SEC(timestamp.substr(15,2), ios_base::in);
    std::stringstream USEC(timestamp.substr(18,6), ios_base::in);
    YEAR >> time.tm_year; time.tm_year -=1900;
    MONTH >> time.tm_mon; --time.tm_mon;
    DAY >> time.tm_mday; 
    HOUR >> time.tm_hour; --time.tm_hour;
    MIN >> time.tm_min;   -- time.tm_min;
    SEC >> time.tm_sec;   -- time.tm_sec;

     /*
     * calculate timeval...
     * --------------------
     */   
     tv.tv_sec = difftime(mktime(&time),mktime(&epoch));
     USEC >> tv.tv_usec;

    return *this;
}

Time& Time::SetNow() {
    gettimeofday(&tv,NULL);
    localtime_r(&tv.tv_sec,&time);
    return *this;
}

string Time::Timestamp() const {
    stringstream strtime;
    strtime << Year();
    strtime << setw(2) << setfill ('0');
    strtime << Month();
    strtime << setw(2) << setfill('0') << MDay();
    strtime << setw(1) << " ";
    strtime << setw(2) << setfill('0') << Hour();
    strtime << setw(1) << ":";
    strtime << setw(2) << Minute(); 
    strtime << setw(1) << ":";
    strtime << setw(2) << setfill('0') << Second();
    strtime << setw(1) << ".";
    strtime << setw(6) <<  setfill('0') << USec();
    return strtime.str();
}

string Time::FileTimestamp() const {
    stringstream strtime;
    strtime << Year() << "-" << Month() << "-" << MDay();
    strtime << "_" << Hour() << ":" << Minute() << ":" << Second();
    strtime << "-" << USec();
    return strtime.str();
}

int Time::DiffSecs(const Time& rhs) const {
    int diff = (tv.tv_sec - rhs.tv.tv_sec);
    if ( tv.tv_usec < rhs.tv.tv_usec) {
        diff-=1;
    }
    return diff;
}
long Time::DiffUSecs(const Time& rhs) const {
    long diff = (  static_cast<long>(tv.tv_sec) - 
                   static_cast<long>(rhs.tv.tv_sec)
                )*1000000;
    diff+=(tv.tv_usec - rhs.tv.tv_usec);
    return diff;
}
