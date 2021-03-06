/* Copyright 2015-16.  Los Alamos National Security, LLC. This material was produced
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
 * specific language governing permissions and limitations under the License.
 *
 * Under this license, it is required to include a reference to this work.
 *
 * This is LANL Copyright Disclosure C16017/LA-CC-15-102
 *
 * Authors: Bob Robey         XCP-2   brobey@lanl.gov
 *          Gerald Collom     XCP-2   gcollom@lanl.gov
 *          Colin Redman      XCP-2   credman@lanl.gov 
 */

#include "meshgen/meshgen.h"
#include "genmalloc/genmalloc.h"
#include "full_perfect_remap.h"

void full_perfect_remap (cell_list icells, cell_list ocells) {

    // Allocate a hash table the size of the finest level of the grid
    uint i_max = icells.ibasesize*two_to_the(icells.levmax);
    uint j_max = icells.jbasesize*two_to_the(icells.levmax);
    int **hash = (int **)genmatrix(j_max, i_max, sizeof(int));

    // Fill Hash Table from Input mesh
    for (uint ic = 0; ic < icells.ncells; ic++){
        uint lev = icells.level[ic];
        uint i = icells.i[ic];
        uint j = icells.j[ic];
        // If at the maximum level just set the one cell
        if (lev == icells.levmax) {
            hash[j][i] = ic;
        } else {
            // Set the square block of cells at the finest level
            // to the index number
            uint lev_mod = two_to_the(icells.levmax - lev);
            for (uint jj = j*lev_mod; jj < (j+1)*lev_mod; jj++) {
                for (uint ii = i*lev_mod; ii < (i+1)*lev_mod; ii++) {
                    hash[jj][ii] = ic;
                }
            }
        }
    }

    // Use Hash Table to Perform Remap
    for (uint ic = 0; ic < ocells.ncells; ic++){
        uint lev = ocells.level[ic];
        uint i = ocells.i[ic];
        uint j = ocells.j[ic];

        // If at the finest level, get the index number and
        // get the value of the input mesh at that index
        if (lev == ocells.levmax) {
            ocells.values[ic] = icells.values[hash[j][i]];
        } else {
            // Sum up the values in the underlying block of
            // cells at the finest level and average
            uint lev_mod = two_to_the(ocells.levmax - lev);
            ocells.values[ic] = 0.0;
            for (uint jj = j*lev_mod; jj < (j+1)*lev_mod; jj++) {
                for (uint ii = i*lev_mod; ii < (i+1)*lev_mod; ii++) {
                    ocells.values[ic] += icells.values[hash[jj][ii]];
                }
            }
            // Get average by dividing by number of cells
            ocells.values[ic] /= (double)(lev_mod*lev_mod);
        }
    }

    // Deallocate hash table
    genmatrixfree((void **)hash);
}

#ifdef _OPENMP
void full_perfect_remap_openMP (cell_list icells, cell_list ocells) {

    // Allocate a hash table the size of the finest level of the grid
    uint i_max = icells.ibasesize*two_to_the(icells.levmax);
    uint j_max = icells.jbasesize*two_to_the(icells.levmax);
    int **hash = (int **)genmatrix(j_max, i_max, sizeof(int));

#pragma omp parallel default(none) shared(icells, ocells, hash)
    {
        uint ilength = icells.ncells;
        uint olength = ocells.ncells;
        uint max_lev = icells.levmax;

        // Fill Hash Table from Input mesh
#pragma omp for
        for (uint ic = 0; ic < ilength; ic++){
            uint lev = icells.level[ic];
            uint i = icells.i[ic];
            uint j = icells.j[ic];
            // If at the maximum level just set the one cell
            if (lev == max_lev) {
                hash[j][i] = ic;
            } else {
                // Set the square block of cells at the finest level
                // to the index number
                uint lev_mod = two_to_the(max_lev - lev);
                for (uint jj = j*lev_mod; jj < (j+1)*lev_mod; jj++) {
                    for (uint ii = i*lev_mod; ii < (i+1)*lev_mod; ii++) {
                        hash[jj][ii] = ic;
                    }
                }
            }
        }

    // Use Hash Table to Perform Remap
#pragma omp for
        for (uint ic = 0; ic < olength; ic++){
            uint lev = ocells.level[ic];
            uint i = ocells.i[ic];
            uint j = ocells.j[ic];

            // If at the finest level, get the index number and
            // get the value of the input mesh at that index
            if (lev == max_lev) {
                ocells.values[ic] = icells.values[hash[j][i]];
            } else {
                // Sum up the values in the underlying block of
                // cells at the finest level and average
                uint lev_mod = two_to_the(max_lev - lev);
                ocells.values[ic] = 0.0;
                for (uint jj = j*lev_mod; jj < (j+1)*lev_mod; jj++) {
                    for (uint ii = i*lev_mod; ii < (i+1)*lev_mod; ii++) {
                        ocells.values[ic] += icells.values[hash[jj][ii]];
                    }
                }
                // Get average by dividing by number of cells
                ocells.values[ic] /= (double)(lev_mod*lev_mod);
            }
        }
    }

    // Deallocate hash table
    genmatrixfree((void **)hash);
}
#endif
