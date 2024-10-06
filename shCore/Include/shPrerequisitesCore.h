#pragma once

#include "shPrerequisitesUtilities.h"

#if defined (SH_CORE_EXPORTS)
# define SH_CORE_EXPORT __declspec(dllexport)
#else
# define SH_CORE_EXPORT __declspec(dllimport)
#endif
