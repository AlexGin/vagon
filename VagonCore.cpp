#include "VagonCore.h"
#include "Alternative.h"

#include <format> 
#include <iostream>

#include "spdlog/spdlog.h"

constexpr double VELOCITY_KOEFF_1 = 0.277777; // Velocity: from "km/h" to "m/sec"
constexpr double VELOCITY_KOEFF_2 = 3.6;      // Velocity: from "m/sec" to "km/h"

VagonCore::VagonCore(const InputData& inp)
  : m_start_time(0L), m_inp(inp)
{
}

VagonCore::~VagonCore()
{
    m_vect_path_sections.clear();
}

bool VagonCore::CalculatePath()
{
    double db_Lenm = m_inp.db_Len * 1000.0; // Length (in meters)
    double db_Velm = m_inp.db_V * VELOCITY_KOEFF_1; // Velocity (m/sec)

    int n_total_sections = m_inp.n_N - 1;
    double db_len = db_Lenm / n_total_sections; // Length of each section

    // Times values (sec):
    double db_acceleration_time = db_Velm / m_inp.db_A;
    double db_stoping_time = db_acceleration_time; 
    double db_plain_path_time = 0.0; // Pessimestic prognose

    // Distance values (m):
    double db_accel_dist = db_Velm * db_acceleration_time / 2.0;
    double db_stopp_dist = db_Velm * db_stoping_time / 2.0;
    double db_plain_path_dist = db_len - (db_accel_dist + db_stopp_dist);

    if (db_plain_path_dist > 0.0) // Correct plain path (drive with velocity V):
    {
        db_plain_path_time = db_plain_path_dist / db_Velm;        
        spdlog::info("VagonCore::CalculatePath: with correct plain path (Plain-distance = {} m).",
            db_plain_path_dist);
    }
    else if (db_plain_path_dist <= 0.0) // Selected Velocity (V) - unreachable:
    {
        spdlog::info("VagonCore::CalculatePath: WITHOUT plain path.");

        AlternativeCalculateData acd;
        acd.db_len = db_len;
        acd.db_accel_dist = db_accel_dist;
        acd.db_plain_path_dist = db_plain_path_dist;
        acd.db_accel = m_inp.db_A;
        
        SP_ALTERNATIVE spAlt(new Alternative(acd));
        spAlt->PrepareAlternativeParameters();
        if (spAlt->IsResultReady())
        {
            double db_max_valid_velocity = spAlt->GetAlternativeVelocity() * VELOCITY_KOEFF_2;
            double db_acceleration_time = spAlt->GetAlternativeDeltaTime();
            spdlog::warn("Selected Velocity (V) - unreachable. Maximum Valid Velocity: {0:.3f} km/h",
                  db_max_valid_velocity);
            spdlog::warn("If using this value Maximum Valid Velocity, Acceleration/Stopping time = {0:.3f} sec",
                  db_acceleration_time);
            std::cout << std::format("WARNING: Selected Velocity (V) - unreachable!!!\n")
                  << std::format("Maximum Valid Velocity: {:.1f} km/h.\n", 
                          db_max_valid_velocity)
                  << std::format("If using this value Maximum Valid Velocity, Acceleration/Stopping time = {:.1f} sec\n",
                          db_acceleration_time);
        }
        return false;
    }
    std::time_t t_curr = time(NULL);
    
    m_start_time = t_curr;
    std::time_t t_prev = t_curr; // The "t_prev" value - it's previous value of time

    double db_dist_prev = 0.0; // The "db_dist_prev" - it's previous value of distance
    for (int nSectionID = 0; nSectionID < n_total_sections; nSectionID++)
    {        
        std::time_t t_valid_time = t_prev;
        double db_valid_distance = db_dist_prev;

        SP_SECTION spSection(new PathSection());

        spSection->m_time_of_starting = t_valid_time;

        PathItem pi0;
        pi0.m_time = t_valid_time;
        pi0.db_velocity = 0.0;
        pi0.db_distance = db_valid_distance;
        spSection->m_vect_path_items.push_back(pi0);

        t_valid_time = t_prev + 
            static_cast<int>(db_acceleration_time);
        db_valid_distance = db_dist_prev + 
            db_accel_dist;

        PathItem pi1;
        pi1.m_time = t_valid_time;
        pi1.db_velocity = m_inp.db_V;
        pi1.db_distance = db_valid_distance;
        spSection->m_vect_path_items.push_back(pi1);

        t_valid_time = t_prev + 
            static_cast<int>(db_acceleration_time) +
            static_cast<int>(db_plain_path_time);
        db_valid_distance = db_dist_prev + 
            db_accel_dist + 
            db_plain_path_dist;

        PathItem pi2;
        pi2.m_time = t_valid_time;
        pi2.db_velocity = m_inp.db_V;
        pi2.db_distance = db_valid_distance;
        spSection->m_vect_path_items.push_back(pi2);

        t_valid_time = t_prev + 
            static_cast<int>(db_acceleration_time) +
            static_cast<int>(db_plain_path_time) +
            static_cast<int>(db_stoping_time);
        db_valid_distance = db_dist_prev + 
            db_accel_dist + 
            db_plain_path_dist + 
            db_stopp_dist;

        spSection->m_time_of_arrival = t_valid_time;

        PathItem pi3;
        pi3.m_time = t_valid_time;
        pi3.db_velocity = 0.0;
        pi3.db_distance = db_valid_distance;
        spSection->m_vect_path_items.push_back(pi3);

        t_valid_time = t_prev + 
            static_cast<int>(db_acceleration_time) +
            static_cast<int>(db_plain_path_time) +
            static_cast<int>(db_stoping_time) + m_inp.n_T;

        spSection->m_time_of_departure = t_valid_time;

        PathItem pi4;
        pi4.m_time = t_valid_time;
        pi4.db_velocity = 0.0;
        pi4.db_distance = db_valid_distance; // This value - from previous step
        spSection->m_vect_path_items.push_back(pi4);

        m_vect_path_sections.push_back(spSection);

        t_prev = t_valid_time;
        db_dist_prev = db_valid_distance;
    }
    return true;
}

std::string VagonCore::TimeConvert(std::time_t t, const std::string& s_format, bool bGMT)
{
    std::tm* ptm = (!bGMT) ? std::localtime(&t) : std::gmtime(&t);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), s_format.c_str(), ptm);
    std::string s_result{buffer};
    return s_result;
}
 
void VagonCore::ViewResults()
{
    int n_section = 1;
    std::string s_fmt{"%d.%m.%Y %H:%M:%S"};
    for (auto spSection : m_vect_path_sections)
    {
        std::time_t t0 = spSection->m_time_of_starting;
        std::string s0 = VagonCore::TimeConvert(t0, s_fmt);

        std::time_t t1 = spSection->m_time_of_arrival;
        std::string s1 = VagonCore::TimeConvert(t1, s_fmt);

        std::time_t t2 = spSection->m_time_of_departure;
        std::string s2 = VagonCore::TimeConvert(t2, s_fmt);

        if (m_inp.n_N - 1 > n_section)
        {
            std::cout << std::format("Section of path: {}\n", n_section++)
                << std::format("{:<20}| {:<20}| {}\n", "T-Start", "T-Arrival", "T-Departure")
                << std::format("{:<20}| {:<20}| {}\n",  s0, s1, s2)
                << "------------------------------------------------------------------------\n";
        }
        else
        {
            std::cout << std::format("Section of path: {}\n", n_section)
                << std::format("{:<20}| {:<20}\n", "T-Start", "T-Arrival")
                << std::format("{:<20}| {:<20}\n",  s0, s1)
                << "------------------------------------------------------------------------\n";
        }
    }
    size_t n_total_sections = m_vect_path_sections.size();
    size_t n_index_of_last  = n_total_sections - 1;
    std::time_t time_begin_of_path = m_vect_path_sections[0]->m_time_of_starting;
    std::time_t time_end_of_path = m_vect_path_sections[n_index_of_last]->m_time_of_arrival;
    std::time_t time_total = time_end_of_path - time_begin_of_path;
    std::string s_fmt_total{"%H:%M:%S"};
    std::string s_time_total = VagonCore::TimeConvert(time_total, s_fmt_total, true);
    std::cout << std::format("Total time of passing of path: {} (HH:MM:SS).\n", s_time_total);
}

