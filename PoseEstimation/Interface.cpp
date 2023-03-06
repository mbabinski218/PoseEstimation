#include "Libs.hpp"
#include "Config.hpp"
#include "Gui.hpp"

int main()
{
    try
    {
        const auto gui = Gui(Config::Get());
    	gui.Render();
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