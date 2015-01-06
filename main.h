#ifndef main_h

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>

#include "config.h"
#include "pindef.h"
#include "macros.h"

#ifdef USE_SERPORT
#include "ser.c"
#include "ser.h"
#endif

#include "wiimote.h"

#define main_h
#endif
