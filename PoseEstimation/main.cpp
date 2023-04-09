#include "stdafx.h"
#include "Runnable.hpp"
#include "Demo.hpp"
#include "Gui.hpp"

int main()
{
    try    
    {
        Demo gui;
        gui.Render();

		//Gui gui;
    	//gui.Redner();
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
