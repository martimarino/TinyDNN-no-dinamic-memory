/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once
#define CNN_SINGLE_THREAD
#include <cstddef>
#include <cstdint>
//#include "floats/softfloat.hpp"
#ifdef CNN_USE_POSIT
//#define POSIT_CEREAL
#undef CNN_USE_AVX
#undef CNN_USE_AVX2
#undef CNN_USE_SSE

#include "interface/cereal/posit.h"
#endif
#define CNN_USE_EXCEPTIONS

#ifdef CNN_USE_RVV
#define _GEMMX_
#define _GEMM_BLOCK_ 24
#include "posit_rvv.h"

#endif

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
	//using X = Posit<int32_t, CNN_POS_BITS , CNN_EXP_BITS, uint64_t, PositSpec::WithNan>;
#else
	using X = CNN_TAB_TYPE;
#endif
typedef X float_t;

#ifdef CNN_USE_RVV
#include "posit_rvv.h"
posit::vectorized::PositRVVBackend<tiny_dnn::float_t> pp_rvv;
#endif
#elif defined CNN_USE_DOUBLE
typedef float float_t;
#else
typedef float float_t;
#endif

typedef float float_accum_t;

}  // namespace tiny_dnn
