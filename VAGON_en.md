```
            VAGON traffic graphics calculation program


         1.	This program is a Command Line Interface (CLI) command line application.
         2.	All source codes of the program are in high-level language C++ (C++20).
         3. Compilation and assembly were carried out using the CMake utility and the VSCode toolkit. Operating system - Ubuntu Linux. The version control system is git.
         4. All messages in the VAGON program are in English. Also in English and all comments in the source codes.
         5. Logging of the program's operation (and output of detailed calculation results) is provided by the freely distributed spdlog library: https://github.com/gabime/spdlog (this library is used in the git repository as a submodule).
         6. Calculation parameters are set on the command line. Here they are: L - length (km), N - number of stopping points, T - time at each stopping point (sec), A - acceleration module during acceleration and braking (m/sec^2), V - maximum speed on the section of path (km/h).
         7. If the specified maximum speed is unreachable (the track section is less than the smaller acceleration and braking sections), the program displays a message about the fact that the selected speed is unreachable and calculates an alternative speed.


 
