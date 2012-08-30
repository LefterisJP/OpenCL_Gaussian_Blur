/**
** This file contains the options that the refu library got built with by the Refu Builder
** It is included from inside many files in the library so that the options can
** get included and that both the library and the using
** program can know which modules are built and which are not, along with other options
** Note that options which are used only in the source files are given to the compiler as flags
** during compiling and are not located in here
**/

#ifndef RF_IAMHERE_FOR_DOXYGEN //I don't want doxygen's preprocessor to have these includes when it runs

//determine the OS
#ifdef _WIN32
	#define REFU_WIN32_VERSION
#else
	#define REFU_LINUX_VERSION
#endif

#define _FILE_OFFSET_BITS 64
#ifndef RF_OPTION_THREADX_MSGQUEUE
	#define RF_OPTION_THREADX_MSGQUEUE 10
#endif
#ifndef RF_OPTION_FGETS_READBYTESN
	#define RF_OPTION_FGETS_READBYTESN 512
#endif
#ifndef RF_OPTION_DEFAULT_ARGUMENTS
	#define RF_OPTION_DEFAULT_ARGUMENTS
#endif
#define RF_OPTION_LOCALSTACKMEMORY_SIZE 1048576
#ifndef RF_MODULE_STRINGS
	#define RF_MODULE_STRINGS
#endif
#ifndef RF_MODULE_TIME_TIMER
	#define RF_MODULE_TIME_TIMER
#endif

#endif //closing the doxygen ifndef