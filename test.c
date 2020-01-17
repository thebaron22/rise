#include "rise.h"

int main()
{

  struct srs psrs;

  psrs.lng = 150.995180;
  psrs.lat = -33.818650;
  psrs.alt = 87.0;

  sunriseset(&psrs, 2020, 1, 11);

  return 0;

}
