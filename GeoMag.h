#pragma once

#include <cstdint>
struct Location {
    int32_t lat;  ///< latitude  in 1e-7 degrees (WGS-84)
    int32_t lng;  ///< longitude in 1e-7 degrees (WGS-84)
};

/// Minimal 3D vector type (Gauss components, NED frame: x=North, y=East, z=Down)
struct Vector3f {
    float x{0}, y{0}, z{0};
};

/**
 * @class AP_Declination
 * @brief Provides access to geomagnetic field data derived from the 
 *        World Magnetic Model (WMM). 
 *
 * This class allows calculation of the Earth's magnetic field properties 
 * (declination, inclination, intensity) at a given geographic location 
 * based on precomputed WMM tables. Data is sampled at fixed resolution 
 * and bilinear interpolation is used between table points.
 */
class AP_Declination
{
public:
    /**
     * @brief Compute Earth's magnetic field parameters at a given latitude/longitude.
     *
     * @param latitude_deg     Latitude in degrees (WGS-84).
     * @param longitude_deg    Longitude in degrees (WGS-84).
     * @param[out] intensity_gauss   Magnetic field intensity (Gauss).
     * @param[out] declination_deg   Magnetic declination (degrees).
     * @param[out] inclination_deg   Magnetic inclination (degrees).
     *
     * @return true if input is inside valid range, false otherwise.
     *
     * The function assumes zero altitude above WGS-84 ellipsoid and 
     * uses bilinear interpolation on the precomputed tables. 
     * Valid input ranges are approximately ±90° latitude and ±180° longitude.
     */  
    static bool get_mag_field_ef(float latitude_deg, float longitude_deg, float &intensity_gauss, float &declination_deg, float &inclination_deg);

    /**
     * @brief Get Earth's magnetic field vector in Gauss at a given Location.
     *
     * @param loc Location in WGS-84 coordinates (latitude, longitude in 1e-7 degrees).
     * @return Vector3f Magnetic field vector expressed in Earth frame (Gauss).
     *
     * The vector is constructed by rotating the field intensity using 
     * the computed inclination and declination.
     */
    static Vector3f get_earth_field_ga(const Location &loc);

    /**
     * @brief Get magnetic declination at a given latitude/longitude.
     *
     * @param latitude_deg  Latitude in degrees (WGS-84).
     * @param longitude_deg Longitude in degrees (WGS-84).
     * @return Declination in degrees.
     *
     * Convenience function that wraps get_mag_field_ef() and returns only the declination.
     */
    static float get_declination(float latitude_deg, float longitude_deg);
    
private:
    /// Sampling resolution in degrees for the geomagnetic tables.
    static const float SAMPLING_RES;

    /// Minimum latitude supported by the tables (degrees).
    static const float SAMPLING_MIN_LAT;

    /// Maximum latitude supported by the tables (degrees).
    static const float SAMPLING_MAX_LAT;

    /// Minimum longitude supported by the tables (degrees).
    static const float SAMPLING_MIN_LON;

    /// Maximum longitude supported by the tables (degrees).
    static const float SAMPLING_MAX_LON;

    /// Number of table entries in latitude direction.
    static const uint32_t LAT_TABLE_SIZE = 19;

    /// Number of table entries in longitude direction.
    static const uint32_t LON_TABLE_SIZE = 37;

    /// Precomputed table of magnetic declination values [deg].
    static const float declination_table[LAT_TABLE_SIZE][LON_TABLE_SIZE];

    /// Precomputed table of magnetic inclination values [deg].
    static const float inclination_table[LAT_TABLE_SIZE][LON_TABLE_SIZE];

    /// Precomputed table of magnetic intensity values [Gauss].
    static const float intensity_table[LAT_TABLE_SIZE][LON_TABLE_SIZE];
};
