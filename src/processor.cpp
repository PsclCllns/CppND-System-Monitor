#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization()
{
    if(prevActive == 0 || prevIdle == 0)
    {
        prevActive = LinuxParser::ActiveJiffies();
        prevIdle   = LinuxParser::IdleJiffies();
        return 0.0;
    }
    else
    {
        const long active       = LinuxParser::ActiveJiffies();
        const long idle         = LinuxParser::IdleJiffies();
        const long total        = active + idle;
        const long prevTotal    = prevActive + prevIdle;
        const long totalDiff    = total - prevTotal;
        const float load {float(totalDiff - idle + prevIdle) / float(totalDiff)};

        return load;
    }
    
}