// Rise header file

struct srs { 
// Location
  double lat;
  double lng;
  double alt;

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
};

int sunriseset(struct srs *psrs, int year, int month, int day);

