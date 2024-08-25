#pragma once

#include <ctime>
#include <vector>
#include <memory>

using ulong64 = unsigned long long int;

struct PathItem // Current data on this point of path: 
{
    std::time_t m_time; // UNIX-style time in this point (sec)
    double db_velocity; // Velocity in this point (m/s); zero - if this item is a starting point
    double db_distance; // Distance from start of current section (m)
};
using VECT_PATH_ITEMS = std::vector<PathItem>;

struct PathSection
{
    // Time "m_time_of_starting" it is time of starting driving on this section 
    // Time "m_time_of_arrival" it is time of arrival on stopping point of path
    // Time "m_time_of_departure" it is time of departure to next section of path
    std::time_t m_time_of_starting;  // UNIX-style time - first timestamp of this section
    std::time_t m_time_of_arrival;   // It's arrival into ending point of this section of path
    std::time_t m_time_of_departure; // Zero - if this section is a last section of path
    VECT_PATH_ITEMS m_vect_path_items; // Collection of path-items (for this section of path)
};
using SP_SECTION = std::shared_ptr<PathSection>;
using VECT_PATH_SECTIONS = std::vector<std::shared_ptr<PathSection>>;
