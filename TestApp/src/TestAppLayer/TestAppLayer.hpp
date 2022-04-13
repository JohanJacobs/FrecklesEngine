#pragma once 
#include "FE/FrecklesEngine.hpp"

class TestApp: public FE::CORE::Layer
{
    public:
        TestApp() 
        {
            LOG_INFO("TestAppLayer::TestApp()");
        }

        TestApp(const TestApp& other) 
        {
            LOG_INFO("TestAppLayer::TestApp(const TestApp& other)");
        }

        TestApp(TestApp&& other)  noexcept
        {
            LOG_INFO("TestAppLayer:: TestApp(TestApp&& other)  noexcept");
        }

        ~TestApp()
        {
            LOG_INFO("TestAppLayer::~TestApp()");
        }

        void OnUpdate() override
        {
            LOG_INFO("TestAppLayer::OnUpdate()");
        }

        void OnAttach()override
        {
            LOG_INFO("TestAppLayer::OnAttach()");
        }

        void OnDetach() override
        {
            LOG_INFO("TestAppLayer::OnDetach()");
        }

    private:
};