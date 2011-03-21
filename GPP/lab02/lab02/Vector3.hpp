#pragma once

#ifdef FORCELINE
#undef FORCELINE
#endif

#if defined (_MSC_VER) || defined (__INTEL_COMPILER)
#	define FORCELINE __forceinline
#elif defined(__GNUC__) || defined(__SUNPRO_C)
#	define FORCELINE __inline
#else
#	error Unrecognized compiler
#endif

//#undef FORCELINE
//#define FORCELINE 

#include "Vector3.h"

#include "Vector3_Scalar.hpp"