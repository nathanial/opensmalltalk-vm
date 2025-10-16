/* Provide the sqImageFile* API for macOS builds by reusing the Unix
 * implementation that lives in sqImageFileAccess.h when INCLUDE_SIF_CODE is 1.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "sq.h"

#if defined(INCLUDE_SIF_CODE) && INCLUDE_SIF_CODE == 0
#undef INCLUDE_SIF_CODE
#endif
#ifndef INCLUDE_SIF_CODE
#define INCLUDE_SIF_CODE 1
#endif

#include "../vm/sqImageFileAccess.h"
