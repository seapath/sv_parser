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

/**
 * \brief Parse raw data from ASDU payload part to an asdu structure.
 *
 * \param rawData Raw data from the ASDU part of the payload
 * \param rawDataLength Length of the raw data
 * \param asdu Structure where we we will write the parsed data
 *
 * \return 0 if success, BAD_FORMAT otherwise
 **/
int parse_SV_ASDU(const uint8_t *rawData, int rawDataLength, struct SV_ASDU *asdu)
{
        int cursor = 0;
        uint8_t tag;
        uint8_t length;
        int i;

        while(cursor < rawDataLength) {
                tag = rawData[cursor];
                length = rawData[cursor+1];
                switch(tag) {
                case 0x80: // svID
                        for (i = 0; i < length; ++i) {
                                // start from 3rd byte
                                asdu->svID[i] = rawData[cursor + i + 2];
                        }
                        asdu->svID[i] = '\0'; // EOS char
                        cursor += length + 2;
                        break;
                case 0x81: // datSet
                        for (i = 0; i < length; ++i) {
                                // start from 3rd byte
                                asdu->datSet[i] = rawData[cursor + i + 2];
                        }
                        asdu->datSet[i] = '\0'; // EOS char
                        cursor += length + 2;
                        break;
                case 0x82: // smpCnt
                        asdu->smpCnt = rawData[cursor+2] << 8
                                     | rawData[cursor+3];
                        cursor += length + 2;
                        break;
                case 0x83: // confRev
                        asdu->confRev = 0;
                        for (int i = 0; i < 4; ++i) {
                                asdu->confRev |= rawData[cursor + 2 + i]
                                        << (24 - 8 * i);
                        }
                        cursor += length + 2;
                        break;
                case 0x84: // refrTm
                        asdu->refrTm = 0;
                        for (int i = 0; i < 8; ++i) {
                                asdu->refrTm |= rawData[cursor + 2 + i]
                                        << (56 - 8 * i);
                        }
                        cursor += length + 2;
                        break;
                case 0x85: // smpSynch
                        asdu->smpSynch = rawData[cursor + 2];
                        cursor += length + 2;
                        break;
                case 0x86: // smpRate
                        asdu->smpRate = rawData[cursor+2] << 8
                                     | rawData[cursor+3];
                        cursor += length + 2;
                        break;
                case 0x87: // sample
                        for (i = 0; i < length; ++i) {
                                // start from 3rd byte
                                asdu->seqData[i] = rawData[cursor + i + 2];
                        }
                        asdu->seqDataLength = length;
                        cursor += length + 2;
                        break;
                case 0x88: // smpMod
                        asdu->smpMod = rawData[cursor + 2];
                        cursor += length + 2;
                        break;
                default:
                        return BAD_FORMAT;
                }
        }
        return 0;
}
