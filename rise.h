// Rise header file

void setloc(double lng, double lat, double alt);
int sunriseset(int year, int month, int day);

// Location
double geopos[3];

// Timezone
double d_timezone = -11;

// Sunrise UTC results
int rise_iyear_utc;
int rise_imonth_utc;
int rise_iday_utc;
int rise_ihour_utc;
int rise_imin_utc;
double rise_dsec_utc;

// Sunrise localtime results
int rise_iyear;
int rise_imonth;
int rise_iday;
int rise_ihour;
int rise_imin;
double rise_dsec;

// Sunset UTC results
int set_iyear_utc;
int set_imonth_utc;
int set_iday_utc;
int set_ihour_utc;
int set_imin_utc;
double set_dsec_utc;

// Sunset localtime results
int set_iyear;
int set_imonth;
int set_iday;
int set_ihour;
int set_imin;
double set_dsec;

