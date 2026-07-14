#ifndef WIDGET_DIALECT_H
#define WIDGET_DIALECT_H

// cl only reports the real dialect in _MSVC_LANG unless /Zc:__cplusplus is on.
#ifdef _MSVC_LANG
#define WIDGET_DIALECT _MSVC_LANG
#else
#define WIDGET_DIALECT __cplusplus
#endif

#endif
