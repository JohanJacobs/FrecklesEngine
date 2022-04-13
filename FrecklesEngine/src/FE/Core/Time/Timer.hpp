#pragma once 
#include <chrono>

namespace FE
{
    namespace CORE
    {
        
        class Timer
        {
        using TimeResolution = std::chrono::high_resolution_clock;
        public:
            Timer(){Reset();}
            ~Timer() = default;
            
            float GetMillis() {return Elapsed();}
            float GetSeconds() {return Elapsed() / 1000.0f;}
            float operator()() {return GetSeconds();}
            void Reset() { StartTime = TimeResolution::now(); }
        private:
            float Elapsed()
            {
                return std::chrono::duration_cast<std::chrono::microseconds>(TimeResolution::now() - StartTime).count()*0.001f;
            }    
        private:
            std::chrono::time_point<TimeResolution> StartTime;
        };
    }
}