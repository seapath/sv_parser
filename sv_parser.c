/**
 * \file sv_parser.c
 * \brief Tool to parse IEC 61850 Sampled Values from the packet payload
 *
 * \copyright Copyright (C) 2024 Savoir-faire Linux, Inc
 * \license SPDX-License-Identifier: Apache-2.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sv_parser.h"

/**
 * \brief SV_payload constructor
 *
 * \param payload Raw data from payload
 * \return Allocated structure
 * */
struct SV_payload* create_SV()
{
        struct SV_payload *sv;

        sv = malloc(sizeof(struct SV_payload));

        /* allocate ASDU */
        sv->seqASDU = malloc(8 * sizeof(struct SV_ASDU)); // max 8 ASDU
        memset(sv->seqASDU, 0, 8*sizeof(struct SV_ASDU));
        for(int i=0; i<8; ++i) {
                sv->seqASDU[i].svID = malloc(34 * sizeof(char)); // svID max
                sv->seqASDU[i].datSet = malloc(130 * sizeof(char)); // datSet
                                                                    // max
                sv->seqASDU[i].seqData = malloc(64 * sizeof(uint8_t));
        }
        return sv;
}

/**
 * \brief SV_payload destructor
 *
 * \param sv Structure to free
 * */
void free_SV(struct SV_payload *sv)
{
        for(int i=0; i<8; ++i) {
                free(sv->seqASDU[i].svID);
                free(sv->seqASDU[i].datSet);
                free(sv->seqASDU[i].seqData);
        }
        free(sv->seqASDU);
        free(sv);
}
