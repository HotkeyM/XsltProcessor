#ifndef SCHEMATRONPROCESSORS_GLOBAL_H
#define SCHEMATRONPROCESSORS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCHEMATRONPROCESSORS_LIBRARY)
#  define SCHEMATRONPROCESSORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCHEMATRONPROCESSORSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCHEMATRONPROCESSORS_GLOBAL_H