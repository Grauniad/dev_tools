#include "tester.h"
#include "util_time.h"
#include "logger.h"

const string reftime = "20140403 10:11:02.294930";
int CheckTime(testLogger& log, const Time& time); 
int ReadFromTimestamp(testLogger& log);
int Copy(testLogger& log);
int DiffSeconds(testLogger& log);
int DiffUSeconds(testLogger& log);
int USecsEpoch (testLogger& log);

int main(int argc, const char *argv[])
{
    Test("Initialising from timestamp...",ReadFromTimestamp).RunTest();
    Test("Copy Constructing...",Copy).RunTest();
    Test("Testing Diff in seconds",DiffSeconds).RunTest();
    Test("Testing Diff in useconds",DiffUSeconds).RunTest();
    Test("Testing Epoch handling...",USecsEpoch).RunTest();
    return 0;
}

int CheckTime(testLogger&log, const Time& testTime) {
    log << "Expected time: " << reftime << endl;
    log << "Constructed time: " << testTime.Timestamp() << endl;
    if ( testTime.Timestamp() != reftime) {
        log << "Timestamp missmatch!" << endl;
        return 1;
    }
    if (testTime.Year() != 2014) {
        log << "Invalid Year!" << testTime.Year();
        return 1;
    }
    if (testTime.Month() != 4) {
        log << "Invalid Month!" << testTime.Month();
        return 1;
    }
    if (testTime.MDay() != 3) {
        log << "Invalid Day!" << testTime.MDay();
        return 1;
    }
    if (testTime.MDay() != 3) {
        log << "Invalid Day!" << testTime.MDay();
        return 1;
    }
    if (testTime.Hour() != 10) {
        log << "Invalid Hour!" << testTime.Hour();
        return 1;
    }
    if (testTime.Minute() != 11) {
        log << "Invalid Min!" << testTime.Minute();
        return 1;
    }
    if (testTime.Second() != 2) {
        log << "Invalid Min!" << testTime.Second();
        return 1;
    }
    if (testTime.MSec() != 294) {
        log << "Invalid MSec!" << testTime.MSec();
        return 1;
    }
    if (testTime.USec() != 294930) {
        log << "Invalid USec!" << testTime.USec();
        return 1;
    }
    return 0;
}

int ReadFromTimestamp(testLogger& log) {
    Time testTime(reftime);
    return CheckTime(log,testTime);
}

int Copy(testLogger& log) {
    Time testTime(reftime);
    Time testTime2(testTime);
    return CheckTime(log,testTime2);
}

int DiffSeconds ( testLogger& log ) {
   const string start = "20140403 10:11:02.394930";
   const string end = "20150504 11:11:03.294930";

   long noDiff = Time(start).DiffSecs(Time(start));
   if ( noDiff !=0 ) {
       log << "expected no diff, but got: " << noDiff << endl;
       return 1;
   }

   long yearDiff = Time("20170403 10:11:02.194930").DiffSecs(Time("20140403 10:11:02.194930"));
   if ( yearDiff != 94694400 ) {
       log << "Invalid year diff!" << endl;
       log << "Expected: " << 94694400 << endl;
       log << "Got: "      << yearDiff << endl;
       return 1;
   }

   long monthDiff = Time("20140703 10:11:02.194930").DiffSecs(Time("20140403 10:11:02.194930"));
   if ( monthDiff != 7862400 ) {
       log << "Invalid month diff!" << endl;
       log << "Expected: " << 7862400 << endl;
       log << "Got: "      << monthDiff << endl;
       log << Time("20140703 10:11:02.194930").Month() << endl;
       log << Time("20140403 10:11:02.194930").Month();
       return 1;
   }

   long diffsecs = 1430737863 - 1396519862 -1;
   long diff = Time(end).DiffSecs(Time(start));
   if ( diff != diffsecs ) {
      log << "Wrong diff!" <<endl;
      log << "Expected: " << diffsecs << endl;
      log << "Got: " << diff << endl;
      return 1;
   }
   return 0;
}

int DiffUSeconds ( testLogger& log ) {
   const string start = "20140403 10:11:02.394930";
   const string end = "20150504 11:11:03.294934";

   long diffusecs = (1430737863L - 1396519862L)*1000000L + (294934L - 394930L);
   long diff = Time(end).DiffUSecs(Time(start));
   if ( diff != diffusecs ) {
      log << "Wrong diff!" <<endl;
      log << "Expected: " << diffusecs << endl;
      log << "Got:      " << diff << endl;
      log << "Diff:     " << diffusecs - diff << endl;
      return 1;
   }
   return 0;
}

int USecsEpoch( testLogger& log ) {
    long secs = Time("20150504 11:11:03.294934").EpochSecs();
    long expected_s = (1430737863L); 
    if ( secs !=  expected_s ) {
        log << "Invalid epoch time (s)! " << secs << endl;
        log << "Expected: " << expected_s << endl;
        return 1;
    }

    long usecs = Time("20150504 11:11:03.294934").EpochUSecs();
    long expected = (1430737863L * 1e6L + 294934L); 
    if ( usecs !=  expected ) {
        log << "Invalid epoch time (us)! " << usecs << endl;
        log << "Expected: " << expected << endl;
        return 1;
    }

    Time time(1430737863L * 1e6L + 294934L);
    if ( time.Timestamp() != "20150504 11:11:03.294934" ) {
        log << "Failed to initialise from epoch usecs!" << endl; 
        log << "expected: " <<  "20150504 11:11:03.294934" << endl;
        log << "got: " << time.Timestamp() << endl;
    }

    return 0;
}
