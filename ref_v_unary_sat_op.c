/*

Copyright (c) 2009, 2010, 2011, 2012 STMicroelectronics
Written by Christophe Lyon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

/* Template file for unary saturating operator validation */

#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#else
#include "stm-arm-neon.h"
#endif

#include "stm-arm-neon-ref.h"

#define FNNAME1(NAME) void exec_ ## NAME (void)
#define FNNAME(NAME) FNNAME1(NAME)

FNNAME (INSN_NAME)
{
  /* Basic test: y=OP(x), then store the result.  */
#define TEST_UNARY_SAT_OP1(INSN, Q, T1, T2, W, N)		\
  Set_Neon_Cumulative_Sat(0, VECT_VAR(vector_res, T1, W, N));	\
  VECT_VAR(vector_res, T1, W, N) =				\
    INSN##Q##_##T2##W(VECT_VAR(vector, T1, W, N));		\
  vst1##Q##_##T2##W(VECT_VAR(result, T1, W, N),			\
		    VECT_VAR(vector_res, T1, W, N));		\
  dump_neon_cumulative_sat(TEST_MSG, xSTR(INSN##Q##_##T2##W),	\
			   xSTR(T1), W, N)

#define TEST_UNARY_SAT_OP(INSN, Q, T1, T2, W, N)	\
  TEST_UNARY_SAT_OP1(INSN, Q, T1, T2, W, N)

    /* With ARM RVCT, we need to declare variables before any executable
       statement  */

  /* No need for 64 bits variants */
  DECL_VARIABLE(vector, int, 8, 8);
  DECL_VARIABLE(vector, int, 16, 4);
  DECL_VARIABLE(vector, int, 32, 2);
  DECL_VARIABLE(vector, int, 8, 16);
  DECL_VARIABLE(vector, int, 16, 8);
  DECL_VARIABLE(vector, int, 32, 4);

  DECL_VARIABLE(vector_res, int, 8, 8);
  DECL_VARIABLE(vector_res, int, 16, 4);
  DECL_VARIABLE(vector_res, int, 32, 2);
  DECL_VARIABLE(vector_res, int, 8, 16);
  DECL_VARIABLE(vector_res, int, 16, 8);
  DECL_VARIABLE(vector_res, int, 32, 4);

  clean_results ();

  /* Initialize input "vector" from "buffer"  */
  VLOAD(vector, buffer, , int, s, 8, 8);
  VLOAD(vector, buffer, , int, s, 16, 4);
  VLOAD(vector, buffer, , int, s, 32, 2);
  VLOAD(vector, buffer, q, int, s, 8, 16);
  VLOAD(vector, buffer, q, int, s, 16, 8);
  VLOAD(vector, buffer, q, int, s, 32, 4);

  /* Apply a unary operator named INSN_NAME  */
  fprintf(ref_file, "\n%s cumulative saturation output:\n", TEST_MSG);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 8, 8);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 16, 4);
  TEST_UNARY_SAT_OP(INSN_NAME, , int, s, 32, 2);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 8, 16);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 16, 8);
  TEST_UNARY_SAT_OP(INSN_NAME, q, int, s, 32, 4);

  dump_results_hex (TEST_MSG);

#ifdef EXTRA_TESTS
  EXTRA_TESTS();
#endif
}
