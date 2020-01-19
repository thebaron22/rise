#include <sweodef.h>
#include <swephexp.h>

class csrs {

    char serr[AS_MAXCH];
    double epheflag = SEFLG_SWIEPH;
    int gregflag = SE_GREG_CAL;

    double rhour;
    double shour;

    // array for atmospheric conditions
    double datm[2];

    double geopos[3];

    int ipl = SE_SUN; // object whose rising is wanted
    char starname[255] = ""; // name of star, if a star's rising is wanted
                      // is "" or NULL, if Sun, Moon, or planet is calculated

    double trise; // for rising time
    double tset;  // for setting time

    int lng;
    int lat;
    int alt;

    int year;
    int month;
    int day;

    double tjd;
    double dt;

    // Sunrise UTC results
    int rise_year;
    int rise_month;
    int rise_day;
    int rise_hour;
    int rise_min;
    double rise_sec;

    // Sunset UTC results
    int set_year;
    int set_month;
    int set_day;
    int set_hour;
    int set_min;
    double set_sec;

  public:
    void init(int x, int y, int z) {
      lng = x;
      lat = y;
      alt = z;

      swe_set_topo(geopos[0], geopos[1], geopos[2]);
    };

    void setdate(int x, int y, int z) {
      year = x;
      month = y;
      day = z;
      
      datm[0] = 1013.25; // atmospheric pressure;
                     // irrelevant with Hindu method, can be set to 0
      datm[1] = 15;      // atmospheric temperature;
                     // irrelevant with Hindu method, can be set to 0

      // calculate the julian day number of the date at 0:00 UT:
      tjd = swe_julday(year,month,day,22,gregflag);

      // convert geographic longitude to time (day fraction) and subtract it from tjd
      // this method should be good for all geographic latitudes except near in
      // polar regions
      dt =  geopos[0] / 360.0;
      tjd = tjd - dt;
    };

    int calc(){

      int return_code;
      return_code = swe_rise_trans(tjd, ipl, starname, epheflag, SE_CALC_RISE, geopos, datm[0], datm[1], &trise, serr);

      if (return_code == ERR) {
        printf("%s\n", serr);
        return 1;
      }

      swe_jdet_to_utc(trise, gregflag, &rise_year, &rise_month, &rise_day, &rise_hour, &rise_min, &rise_sec);

      return_code = swe_rise_trans(trise, ipl, starname, epheflag, SE_CALC_SET, geopos, datm[0], datm[1], &tset, serr);

      if (return_code == ERR) {
        printf("%s\n", serr);
        return 1;
      }
    
      swe_jdet_to_utc(tset, gregflag, &set_year, &set_month, &set_day, &set_hour, &set_min, &set_sec);

      return 0;
    };
};
