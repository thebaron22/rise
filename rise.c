#include <sweodef.h>
#include <swephexp.h>
#include "rise.h"

int sunriseset(struct srs *psrs, int year, int month, int day)
{
  char serr[AS_MAXCH];
  double epheflag = SEFLG_SWIEPH;
  int gregflag = SE_GREG_CAL;

  double rhour;
  double shour;

  // array for atmospheric conditions
  double datm[2];
  datm[0] = 1013.25; // atmospheric pressure;
                     // irrelevant with Hindu method, can be set to 0
  datm[1] = 15;      // atmospheric temperature;
                     // irrelevant with Hindu method, can be set to 0

  double geopos[3];
  geopos[0] = psrs->lng;
  geopos[1] = psrs->lat;
  geopos[2] = psrs->alt;

  swe_set_topo(geopos[0], geopos[1], geopos[2]);

  int ipl = SE_SUN; // object whose rising is wanted
  char starname[255] = ""; // name of star, if a star's rising is wanted
                      // is "" or NULL, if Sun, Moon, or planet is calculated
  double trise; // for rising time
  double tset;  // for setting time
  
  // calculate the julian day number of the date at 0:00 UT:
  double tjd = swe_julday(year,month,day,22,gregflag);

  // convert geographic longitude to time (day fraction) and subtract it from tjd
  // this method should be good for all geographic latitudes except near in
  // polar regions
  double dt =  geopos[0] / 360.0;
  tjd = tjd - dt;

  int rsmi = SE_CALC_RISE;
  int return_code = swe_rise_trans(tjd, ipl, starname, epheflag, rsmi, geopos, datm[0], datm[1], &trise, serr);

  if (return_code == ERR) {
    printf("%s\n", serr);
    return 1;
  }

  swe_revjul(trise, gregflag, &year, &month, &day, &rhour);


  rsmi = SE_CALC_SET;
  return_code = swe_rise_trans(trise, ipl, starname, epheflag, rsmi, geopos, datm[0], datm[1], &tset, serr);

  if (return_code == ERR) {
    printf("%s\n", serr);
    return 1;
  }

  swe_revjul(tset, gregflag, &year, &month, &day, &shour);
  printf("Day length: %f\tJulian days\nDay length: %f\tHours\n\n", (tset - trise), (tset - trise) * 24 );

  // Turn Julian day into first UTC then localtime
  swe_jdet_to_utc(trise, gregflag, &psrs->rise_year, &psrs->rise_month, &psrs->rise_day, &psrs->rise_hour, &psrs->rise_min, &psrs->rise_sec);
  printf("UTC   sunrise : date=%i/%i/%i, time=%02i:%02i:%05.2f\n", psrs->rise_year, psrs->rise_month, psrs->rise_day, psrs->rise_hour, psrs->rise_min, psrs->rise_sec);

  swe_jdet_to_utc(tset, gregflag, &psrs->set_year, &psrs->set_month, &psrs->set_day, &psrs->set_hour, &psrs->set_min, &psrs->set_sec);
  printf("UTC   sunset  : date=%i/%i/%i, time=%02i:%02i:%05.2f\n", psrs->set_year, psrs->set_month, psrs->set_day, psrs->set_hour, psrs->set_min, psrs->set_sec);

  return 0;
}
