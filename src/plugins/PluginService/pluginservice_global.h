#ifndef PLUGINSERVICE_GLOBAL_H
#define PLUGINSERVICE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGINSERVICE_LIBRARY)
#  define PLUGINSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGINSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINSERVICE_GLOBAL_H
