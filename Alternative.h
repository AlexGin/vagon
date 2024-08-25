#pragma once

#include <memory>

struct AlternativeCalculateData
{
    double db_len;             // Length of each section (m)
    double db_accel_dist;      // Distance, passing with acceleration A (m/sec^2)
    double db_plain_path_dist; // Distance, passing with velocity V (m/sec) without acceleration
    double db_accel;           // Acceleration A (m/sec^2) - for beginning and ending of section
};

class Alternative
{
  public:
    Alternative(const AlternativeCalculateData& acd);
    void PrepareAlternativeParameters();
    double GetAlternativeVelocity() const;
    double GetAlternativeDeltaTime() const;
    bool IsResultReady() const;
  private:
    AlternativeCalculateData m_acd;
    double m_dbAlternativeVelocity = 0.0; // Inactive value (m/sec)
    double m_dbAlternativeDeltaTime = 0.0; // Inactive value (sec)
    bool m_bIsResultReady = false;        // Starting value - result is not ready
};
using SP_ALTERNATIVE = std::shared_ptr<Alternative>;
