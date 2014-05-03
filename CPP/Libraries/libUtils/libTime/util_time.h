#ifndef __ELF_64_UTIL_TIME__
#define __ELF_64_UTIL_TIME__

#include <sys/time.h>
#include <string>

class Time {
public:
    Time ();
    Time (const Time& othertime);
    Time (const std::string& timestamp);
    Time& operator=(const Time& rhs);
    Time& operator=(const std::string& timestamp);

    Time& SetNow();

    // Components
    int Year()   const { return time.tm_year + 1900;}
    int Month()  const { return time.tm_mon +1;}
    int MDay()   const { return time.tm_mday;}
    int Hour()   const { return time.tm_hour +1;}
    int Minute() const { return time.tm_min +1;}
    int Second() const { return time.tm_sec +1;}
    int MSec()   const { return tv.tv_usec / 1000;}
    int USec()   const { return tv.tv_usec;}

    // Diffs: Time since rhs: (this - rhs)
    int  DiffSecs (const Time& rhs) const;
    long DiffUSecs (const Time& rhs) const;

    std::string Timestamp() const;
    std::string FileTimestamp() const;
private:
    struct timeval tv;
    struct tm time;
};

#endif
