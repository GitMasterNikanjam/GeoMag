/*
Build:
  mkdir -p ./bin && g++ -std=c++11 -Wall -Wextra test_geomag.cpp ../GeoMag.cpp ../tables.cpp -o ./bin/test_geomag

Run:
  ./bin/test_geomag
*/

#include <cstdio>
#include "../GeoMag.h"

static void test_point(const char* name, float lat_deg, float lon_deg)
{
    float I = 0.0f;   // intensity (Gauss)
    float D = 0.0f;   // declination (deg, east of north)
    float Inc = 0.0f; // inclination (deg, positive Down)

    const bool inside = AP_Declination::get_mag_field_ef(lat_deg, lon_deg, I, D, Inc);

    std::printf("[%s] lat=%.4f lon=%.4f | Intensity=%.5f G  Decl[deg]=%.3f  Incl[deg]=%.3f  inside:%s\n",
                name, lat_deg, lon_deg, I, D, Inc, inside ? "true" : "false");

    Location loc{};
    loc.lat = static_cast<int32_t>(lat_deg * 1e7f);
    loc.lng = static_cast<int32_t>(lon_deg * 1e7f);

    const Vector3f B = AP_Declination::get_earth_field_ga(loc);
    std::printf("    B_ef (N,E,D) = [%.6f, %.6f, %.6f] Gauss\n\n", B.x, B.y, B.z);
}

int main()
{
    test_point("Berlin",   52.5200f,   13.4050f);
    test_point("Tehran",   35.6892f,   51.3890f);
    test_point("Sydney",  -33.8688f,  151.2093f);
    test_point("Quito",     0.1807f,  -78.4678f);

    // Optional: quick edge checks near bounds of the table
    test_point("NorthPole-ish",  89.0f,   0.0f);
    test_point("SouthPole-ish", -89.0f,   0.0f);
    test_point("Dateline",        0.0f, 179.9f);

    return 0;
}
