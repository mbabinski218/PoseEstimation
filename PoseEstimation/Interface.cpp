#include <boost/exception/diagnostic_information.hpp> 
#include <iostream>
#include "Gui.h"

#ifdef _WIN32
#include "Windows.h"
#endif

int main()
{
    try
    {
        //std::cout << cv::getBuildInformation() << "\n";
        Gui::Render();
    }
    catch(...)
    {
#ifdef _WIN32
        const auto h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 4);
        std::cout << "[EXCEPTION] " << boost::current_exception_diagnostic_information();
        SetConsoleTextAttribute(h, 7);
#else 
        std::cout << "[EXCEPTION] " << boost::current_exception_diagnostic_information();
#endif

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}