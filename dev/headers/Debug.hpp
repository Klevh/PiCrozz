#ifndef DEBUG_PICROZZ_AUBIN_KLEVH_H
#define DEBUG_PICROZZ_AUBIN_KLEVH_H

#ifndef DEBUG
#define DEBUG
#endif

#ifdef XMLCheckResult
#undef XMLCheckResult
#endif

#if defined(DEBUG) && !defined(NDEBUG)
#  include <iostream>
#  define LOG_DEBUG(a) std::cout << a << std::endl;
#  define XMLCheckResult(a_eResult)					\
    if ((a_eResult) != tinyxml2::XML_SUCCESS) {				\
	std::cout << "Error: " << (a_eResult) << std::endl << std::flush; \
    }while(0)
#else
#  define LOG_DEBUG(a)
#  define XMLCheckResult(a)
#endif

#ifdef LOG_OUTPUT
#  define SHOW_WHERE(exception,err) LOG_OUTPUT << #exception " caught in file :\n\t" __FILE__ "\nCaught before line : " << __LINE__ << "\n\t"  << err.what() << std::endl;
#endif

#endif
