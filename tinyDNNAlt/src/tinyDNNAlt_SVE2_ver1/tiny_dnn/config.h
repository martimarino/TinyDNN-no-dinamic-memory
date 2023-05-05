/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <cstddef>
#include <cstdint>
#ifdef CNN_USE_POSIT
//#define POSIT_CEREAL
#undef CNN_USE_AVX
#undef CNN_USE_AVX2
#undef CNN_USE_SSE
#include "interface/cereal/posit.h"
using namespace posit;
#endif
#define CNN_USE_EXCEPTIONS

#define CNN_USE_STDOUT

#ifdef USE_GEMMLOWP
#if !defined(_MSC_VER) && !defined(_WIN32) && !defined(WIN32)
#define CNN_USE_GEMMLOWP  // gemmlowp doesn't support MSVC/mingw
#endif
#endif  // USE_GEMMLOWP

#ifdef CNN_USE_OMP
#define CNN_TASK_SIZE 100
#else
#define CNN_TASK_SIZE 8
#endif

namespace tiny_dnn {

#ifdef CNN_USE_POSIT
#ifndef CNN_TAB_TYPE
	using X = Posit<CNN_POS_STORAGE, CNN_POS_BITS , CNN_EXP_BITS, CNN_POS_BACKEND, PositSpec::WithNan>;
#else
	using X = CNN_TAB_TYPE;
#endif
typedef X float_t;
#elif defined CNN_USE_DOUBLE
typedef double float_t;
#else
typedef float float_t;
#endif

typedef float float_accum_t;

}  // namespace tiny_dnn
