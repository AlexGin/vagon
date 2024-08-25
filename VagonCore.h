#pragma once

#include "InputData.h"
#include "PathData.h"

class VagonCore
{
    public:
       VagonCore(const InputData& inp);
       ~VagonCore();
       bool CalculatePath();       
       void ViewResults();       
    private:       
       InputData m_inp;
       std::time_t m_start_time;
       VECT_PATH_SECTIONS m_vect_path_sections;
    public:
       static std::string TimeConvert(std::time_t t, 
           const std::string& s_format, bool bGMT = false);
};
using SP_VAGON_CORE = std::shared_ptr<VagonCore>;
