/* Copyright 2012.  Los Alamos National Security, LLC. This material was produced
 * under U.S. Government contract DE-AC52-06NA25396 for Los Alamos National 
 * Laboratory (LANL), which is operated by Los Alamos National Security, LLC
 * for the U.S. Department of Energy. The U.S. Government has rights to use,
 * reproduce, and distribute this software.  NEITHER THE GOVERNMENT NOR LOS
 * ALAMOS NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified
 * to produce derivative works, such modified software should be clearly marked,
 * so as not to confuse it with the version available from LANL.   
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.”
 *
 * Under this license, it is required to include a reference to this work. We
 * request that each derivative work contain a reference to LANL Copyright 
 * Disclosure C13002/LA-CC-12-022 so that this work’s impact can be roughly
 * measured. In addition, it is requested that a modifier is included as in
 * the following example:
 *
 * //<Uses | improves on | modified from> LANL Copyright Disclosure C13002/LA-CC-12-022
 *
 * This is LANL Copyright Disclosure C13002/LA-CC-12-022
 */

#ifndef _Globals2d_
#define _Globals2d_


#ifdef __cplusplus
extern "C"
{
#endif
   
//#define NDEBUG 1
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ENTITY_COINCIDENCE_TOLERANCE      ((double)1.0E-5)

#define KDTREE_ELEMENT_BLOCKING_SIZE      ((long)1024)

#define POSITIVE_INFINITY (+1.0E+64)
#define NEGATIVE_INFINITY (-1.0E+64)

#define XAXIS ((unsigned long)0)
#define YAXIS ((unsigned long)1)

typedef struct {
   double x, y;
} TVector2d;

#ifndef _BOOLEAN
#define _BOOLEAN
typedef unsigned char boolean;
#ifndef true
#define true  ((boolean)1)
#endif
#ifndef false
#define false ((boolean)0)
#endif
#endif

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#ifndef SWAP
#define SWAP(a,b,t) {t h; h = a; a = b; b = h; }
#endif

#define MALLOC(n,t) ((t*)(malloc(n * sizeof(t))))
#define REALLOC(p,n,t) ((t*)(realloc((void*)p, n * sizeof(t))))
#define FREE(p) { if (p) free(p); }
#define MEMCPY(s,d,n,t) {memcpy((void*)d, (void*)s, n * sizeof(t)); }

#ifdef __cplusplus
}
#endif
   
#endif
