#pragma once 
namespace FE
{
    namespace CORE
    {
        class Timestep
        {
        public:
            Timestep(float time) :CurrentTime{time} {}
            Timestep() =default;

            float GetSeconds() {return CurrentTime;}
            
            operator float() {return CurrentTime;}            
        private:
            float CurrentTime;

        };
    }
}