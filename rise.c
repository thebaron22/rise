#include "sweodef.h"
#include "swephexp.h"

int main()
{
  char serr[AS_MAXCH];
  double epheflag = SEFLG_SWIEPH;
  int gregflag = SE_GREG_CAL;

  int year = 2020;
  int month = 1;
  int day = 11;

  double geo_longitude = 150.995180; // positive for east, negative for west of Greenwich
  double geo_latitude = -33.818650;
  double geo_altitude = 87.0;

  double rhour;
  double shour;

  // array for atmospheric conditions
  double datm[2];
  datm[0] = 1013.25; // atmospheric pressure;
                     // irrelevant with Hindu method, can be set to 0
  datm[1] = 15;      // atmospheric temperature;
                     // irrelevant with Hindu method, can be set to 0

  double geopos[3];
  geopos[0] = geo_longitude;
  geopos[1] = geo_latitude;
  geopos[2] = geo_altitude;

  swe_set_topo(geopos[0], geopos[1], geopos[2]);

  int ipl = SE_SUN; // object whose rising is wanted
  char starname[255]; // name of star, if a star's rising is wanted
                      // is "" or NULL, if Sun, Moon, or planet is calculated
  double trise; // for rising time
  double tset;  // for setting time
  
  // calculate the julian day number of the date at 0:00 UT:
  double tjd = swe_julday(year,month,day,22,gregflag);

  // convert geographic longitude to time (day fraction) and subtract it from tjd
  // this method should be good for all geographic latitudes except near in
  // polar regions
  double dt = geo_longitude / 360.0;
  tjd = tjd - dt;

  int rsmi = SE_CALC_RISE;
  int return_code = swe_rise_trans(tjd, ipl, starname, epheflag, rsmi, geopos, datm[0], datm[1], &trise, serr);

  if (return_code == ERR) {
    printf("%s\n", serr);
  }

  swe_revjul(trise, gregflag, &year, &month, &day, &rhour);


  rsmi = SE_CALC_SET;
  return_code = swe_rise_trans(trise, ipl, starname, epheflag, rsmi, geopos, datm[0], datm[1], &tset, serr);

  if (return_code == ERR) {
    printf("%s\n", serr);
  }

  swe_revjul(tset, gregflag, &year, &month, &day, &shour);
  printf("Day length: %f\tJulian days\nDay length: %f\tHours\n\n", (tset - trise), (tset - trise) * 24 );

  // Turn Julian day into first UTC then localtime
  int iyear_utc;
  int imonth_utc;
  int iday_utc;
  int ihour_utc;
  int imin_utc;
  double dsec_utc;

  double d_timezone = 11;
  int iyear;
  int imonth;
  int iday;
  int ihour;
  int imin;
  double dsec;

  swe_jdet_to_utc(trise, gregflag, &iyear_utc, &imonth_utc, &iday_utc, &ihour_utc, &imin_utc, &dsec_utc);
  printf("UTC   sunrise : date=%i/%i/%i, time=%02i:%02i:%05.2f\n", iyear_utc, imonth_utc, iday_utc, ihour_utc, imin_utc, dsec_utc);

  swe_utc_time_zone(iyear_utc, imonth_utc, iday_utc, ihour_utc, imin_utc, dsec_utc, -d_timezone, &iyear, &imonth, &iday, &ihour, &imin, &dsec);
  printf("Local sunrise  : date=%i/%i/%i, time=%02i:%02i:%05.2f\n\n", iyear, imonth, iday, ihour, imin, dsec);

  swe_jdet_to_utc(tset, gregflag, &iyear_utc, &imonth_utc, &iday_utc, &ihour_utc, &imin_utc, &dsec_utc);
  printf("UTC   sunset  : date=%i/%i/%i, time=%02i:%02i:%05.2f\n", iyear_utc, imonth_utc, iday_utc, ihour_utc, imin_utc, dsec_utc);

  swe_utc_time_zone(iyear_utc, imonth_utc, iday_utc, ihour_utc, imin_utc, dsec_utc, -d_timezone, &iyear, &imonth, &iday, &ihour, &imin, &dsec);
  printf("Local sunset  : date=%i/%i/%i, time=%02i:%02i:%05.2f\n\n", iyear, imonth, iday, ihour, imin, dsec);

  //  Calculate Nocturnal Tides
  int i;
  int j;
  double nocday = ( tset - trise ) / 30 ;
  printf("nocday: %f\n\n", nocday);
  for ( i=0; i<30; i++) {

    j = i % 5;

    switch(j){
      case 0 :
        printf("Spirit:\t");
        break;
      case 1 :
        printf("Air:\t");
        break;
      case 2 :
        printf("Fire:\t");
        break;
      case 3 :
        printf("Water:\t");
        break;
      case 4 :
        printf("Earth:\t");
        break;
    }

    printf("%f\t", trise + i * nocday);

    swe_jdet_to_utc(trise + i * nocday, gregflag, &iyear_utc, &imonth_utc, &iday_utc, &ihour_utc, &imin_utc, &dsec_utc);
    swe_utc_time_zone(iyear_utc, imonth_utc, iday_utc, ihour_utc, imin_utc, dsec_utc, -d_timezone, &iyear, &imonth, &iday, &ihour, &imin, &dsec);
    printf(": date=%i/%i/%i, time=%02i:%02i:%05.2f\n", iyear, imonth, iday, ihour, imin, dsec);

  }
  printf("\nCheck: %f\n\n", trise + 29 * nocday );

  // Calculate moon stuff. Needs more work
  double attr[20];
  return_code = swe_pheno_ut( tjd, SE_MOON, 0, attr, serr);

  if (return_code == ERR) {
    printf("%s\n", serr);
  }

  printf("Moon phase: %.2f%%  ", attr[1] * 100);

  double today = attr[1];
  return_code = swe_pheno_ut( tjd - 1, SE_MOON, 0, attr, serr);

  if (return_code == ERR) {
    // error action
    printf("%s\n", serr);
  }

  if ( today > attr[1] ) {
    printf("Waxing\n");
  }
  else {
    printf("Waning\n");
  }

  printf("Moon phase angle: %f\n", attr[0]);

  printf("\n");

}
