#include "Alternative.h"

#include <cmath>

Alternative::Alternative(const AlternativeCalculateData& acd)
  : m_acd(acd)
{
}

void Alternative::PrepareAlternativeParameters()
{
    // The length of stopping distance is equal to "m_acd.db_accel_dist":
    double db_len_two = 2.0 * m_acd.db_accel_dist; // Two parts: acceleration and stopping
    double db_len_temp = std::abs(m_acd.db_len - db_len_two);
    double db_len_temp_2 = db_len_temp / 2.0;
    double db_len_new = m_acd.db_accel_dist - db_len_temp_2;
    // Update results of calculations:
    m_dbAlternativeDeltaTime = std::sqrt(2.0 * db_len_new / m_acd.db_accel);
    m_dbAlternativeVelocity = m_dbAlternativeDeltaTime * m_acd.db_accel;
    m_bIsResultReady = true;
}

double Alternative::GetAlternativeVelocity() const
{
    return m_dbAlternativeVelocity;
}

double Alternative::GetAlternativeDeltaTime() const
{
    return m_dbAlternativeDeltaTime;
}

bool Alternative::IsResultReady() const
{
    return m_bIsResultReady;
}
