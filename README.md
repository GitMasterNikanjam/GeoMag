# GeoMag

Lightweight, dependency-free C++11 library for **Earth’s magnetic field** lookup using a compact, embedded-friendly grid with bilinear interpolation.

* **Outputs**

  * Scalar **intensity** |B| (Gauss)
  * **Declination** D (degrees, east of true north)
  * **Inclination** I (degrees, positive downward)
  * Full **vector** **B<sub>NED</sub> = (N, E, D)** in **Gauss** (North–East–Down)
* **Zero external deps** — uses only the C++ standard library.
* **Portable** — works on Linux, Windows/MinGW, and microcontrollers (C++ runtime required).

---

## Contents

* [`GeoMag.h`](GeoMag.h) — public API + minimal `Location` (lat/lon in 1e-7 deg) and `Vector3f` types
* [`GeoMag.cpp`](GeoMag.cpp) — interpolation and NED vector math
* [`tables.cpp`](tables.cpp) — precomputed grid (10° world coverage)
* [`test_geomag.cpp`](test_geomag.cpp) — minimal example

---

## Quick Start

### Build & run (Linux/WSL/MinGW)

```bash
mkdir -p ./bin
g++ -std=c++11 -O2 -Wall -Wextra \
  test_geomag.cpp GeoMag.cpp tables.cpp \
  -o ./bin/test_geomag
./bin/test_geomag
```

### Embedded (ARM GCC example)

```bash
arm-none-eabi-g++ -std=gnu++11 -O2 -ffunction-sections -fdata-sections \
  -c GeoMag.cpp tables.cpp
# Link into your firmware as usual (consider -Wl,--gc-sections)
```

> The code avoids heap allocation and system calls; it only needs the C++ runtime.

---

## API

```cpp
// Types (from GeoMag.h)
struct Location { int32_t lat; int32_t lng; };   // degrees * 1e-7 (WGS-84)
struct Vector3f { float x, y, z; };              // NED (North, East, Down), Gauss

namespace AP_Declination {

// Full scalar field (Gauss, deg)
bool  get_mag_field_ef(float latitude_deg, float longitude_deg,
                       float &intensity_gauss,
                       float &declination_deg,
                       float &inclination_deg);

// Convenience: declination only (deg)
float get_declination(float latitude_deg, float longitude_deg);

// Full vector field in NED (Gauss)
Vector3f get_earth_field_ga(const Location &loc);

} // namespace AP_Declination
```

---

## Example

```cpp
#include <iostream>
#include "GeoMag.h"

int main() {
  // Tehran
  float I = 0.0f, D = 0.0f, Inc = 0.0f;
  AP_Declination::get_mag_field_ef(35.6892f, 51.3890f, I, D, Inc);

  Location loc{};
  loc.lat = static_cast<int32_t>(35.6892f * 1e7f);
  loc.lng = static_cast<int32_t>(51.3890f * 1e7f);

  Vector3f B = AP_Declination::get_earth_field_ga(loc);

  std::cout << "Intensity: " << I << " G, Decl: " << D << " deg, Incl: " << Inc << " deg\n";
  std::cout << "B_NED [G]: N=" << B.x << "  E=" << B.y << "  D=" << B.z << "\n";

  // Convert to microtesla if desired: 1 G = 100 µT
  float Bx_uT = B.x * 100.0f;
  (void)Bx_uT;

  return 0;
}
```

---

## Math, Frames, Units

* **Frame**: NED — **N**orth (x), **E**ast (y), **D**own (z).
* **Angles**: API uses **degrees**; library converts to radians internally.
* **Units**: **Gauss** (G). Convert to **µT** with `µT = G × 100`.
* **Vector construction** (from scalar outputs):

  ```
  D = radians(declination_deg)
  I = radians(inclination_deg)
  N = |B| * cos(I) * cos(D)
  E = |B| * cos(I) * sin(D)
  D = |B| * sin(I)
  ```

---

## Data & Interpolation

* World grid at **10°** resolution (lat −90..+90, lon −180..+180) baked into `tables.cpp`.
* Queries clamp to the nearest grid cell when outside (function returns `false` to signal the clamp; values are still provided).
* Bilinear interpolation within the cell for smooth outputs.

---

## Limitations

* **Altitude** not modeled (assumes sea level over the ellipsoid).
* **Epoch** tied to the precomputed table (accuracy depends on when tables were generated).
* Outputs are **Gauss**, not µT (convert if needed).

---

## Testing

* Run the sample app: `./bin/test_geomag`
* Sanity check: `sqrt(N^2 + E^2 + D^2)` should closely match the reported intensity |B| (small differences from interpolation/precision).

---

## License

This library is distributed under **GPLv3**. See headers for details.

---

## Acknowledgements

Based on a compact precomputed geomagnetic grid with standard bilinear interpolation; refactored for portability (no external project includes).

---
