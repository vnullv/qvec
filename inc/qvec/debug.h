#pragma once
#ifndef QVEC_DEBUG_H
#define QVEC_DEBUG_H

#include "qvec/vector.h"

void qvec_debug_print(qvec_t* v, void (*cb)(void const*));

#endif
