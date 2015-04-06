#include "util_time.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

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

Time::Time(const struct timeval& tv) {
    (*this) = tv;
}

Time::Time(const long& usecs) {
    (*this) = usecs;
}

Time& Time::operator=(const struct timeval& tv) {
    this->tv = tv;
    ready = false;
    return *this;
}

Time& Time::operator=(const long& usecs ) {
    this->tv.tv_usec = usecs%1000000L;
    this->tv.tv_sec = usecs / 1000000L;
    ready = false;
    return *this;
}

Time& Time::operator=(const Time& rhs) {
    this->tv = rhs.tv;
    this->ready = false;
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
    epoch.tm_mon=0;
    epoch.tm_year=70;
    epoch.tm_isdst=-1; // <0 = unknown
    epoch.tm_gmtoff = 0;
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

    /*
     * atoi (whilst nasty) is ok here since c_str() guarentees null termination
     * and substr guarentess the number is short enough not to overflow
     *
     * In the case where they provide garbage, atoi will return 0, which is
     * good enough to represent garbage data.
     */
    time.tm_year = atoi(timestamp.substr(0,4).c_str()); 
    if ( time.tm_year != 0 ) {
        time.tm_year -=1900;
    }

    time.tm_mon = atoi(timestamp.substr(4,2).c_str());  
    if ( time.tm_mon != 0 ) {
        --time.tm_mon;
    }

    time.tm_mday = atoi(timestamp.substr(6,2).c_str());
    time.tm_hour = atoi(timestamp.substr(9,2).c_str());
    time.tm_min  = atoi(timestamp.substr(12,2).c_str());
    time.tm_sec  = atoi(timestamp.substr(15,2).c_str());

     /*
     * calculate timeval...
     * --------------------
     */   
     tv.tv_sec = difftime(mktime(&time),mktime(&epoch));
     tv.tv_usec =  atoi(timestamp.substr(18,6).c_str());

     ready = true;

    return *this;
}

Time& Time::SetNow() {
    gettimeofday(&tv,NULL);
    ready = false;
    return *this;
}

void Time::MakeReady() const {
    if ( !ready) {
        SetTmFromTimeval();
    }
}

void Time::SetTmFromTimeval() const {
    gmtime_r(&tv.tv_sec,&time);
}

string Time::Timestamp() const {
    MakeReady();
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
    MakeReady();
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

long Time::EpochUSecs() const {
    return tv.tv_usec + 1e6L* (long(tv.tv_sec));
}

int Time::EpochSecs() const {
    return tv.tv_sec;
}
