#include <format> 
#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

#include "InputData.h"
#include "VagonCore.h"

int main(int argc, char** argv)
{
    // Create a daily logger - a new file is created every day on 1:10am
    auto logger = spdlog::daily_logger_mt("dl", "logs/vagon.txt", 1, 10);
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
    spdlog::set_default_logger(logger);

    spdlog::info("Application 'vagon' - starting (argc = {}).", argc);

    bool b_start_flag = false;
    InputData inp;  
    if (argc != 6)
    {
        std::cout << "Usage: ./vagon L N T A V" << "\r\n";
        std::cout << "Where:" << "\r\n";
        std::cout << "L - length of road (km)" << "\r\n";
        std::cout << "N - stop points (total quantity)" << "\r\n";
        std::cout << "T - duration of waiting at point (sec)" << "\r\n";
        std::cout << "A - acceleration (m/sec^2)" << "\r\n";
        std::cout << "V - velocity (km/h)" << "\r\n";
    }
    else if (argc == 6)
    {
        inp.db_Len = atof(argv[1]);
        inp.n_N = atoi(argv[2]);
        inp.n_T = atoi(argv[3]);
        inp.db_A = atof(argv[4]);
        inp.db_V = atof(argv[5]);
        
        std::cout << std::format("{:>4}{:>13}\n", "Param", "Value"); // headers        
        std::cout << std::format("{:>4}{:>13.3}\n", "L", inp.db_Len)
          << std::format("{:>4}{:>13}\n", "N", inp.n_N)
          << std::format("{:>4}{:>13}\n", "T", inp.n_T)
          << std::format("{:>4}{:>13.3}\n", "A", inp.db_A)
          << std::format("{:>4}{:>13.3}\n", "V", inp.db_V);

        spdlog::info("Input Parameters:");
        spdlog::info("   L = {0}", inp.db_Len);
        spdlog::info("   N = {0}", inp.n_N);
        spdlog::info("   T = {0}", inp.n_T);
        spdlog::info("   A = {0}", inp.db_A);
        spdlog::info("   V = {0}", inp.db_V);
        
        b_start_flag = ((inp.n_N >= 2) && (inp.n_T > 0));
    }
    
    if (!b_start_flag)
    {
        if (argc == 6)
        {
            std::cout << std::format("INCORRECT!!!\n")
              << std::format("Valid values:\n")
              << std::format("  for N: VALID_VALUE >=2\n")
              << std::format("  for T: VALID_VALUE > 0\n");

            spdlog::info("Application 'vagon' - incorrect inputing of data - exit!");
        }
        return 1;
    }
       
    SP_VAGON_CORE spVagonCore(new VagonCore(inp));
    bool bSuccess = spVagonCore->CalculatePath();
    if (bSuccess)
        spVagonCore->ViewResults();
        
    return 0;
}
