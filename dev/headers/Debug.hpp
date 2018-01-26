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
	std::cout << "Error: " << (a_eResult) << std::endl;		\
    }while(0)
#else
#  define LOG_DEBUG(a)
#  define XMLCheckResult(a)
#endif

#endif
