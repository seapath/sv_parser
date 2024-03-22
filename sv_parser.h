/**
 * \file sv_parser.h
 * \brief Tool to parse IEC 61850 Sampled Values from the packet payload
 *
 * \copyright Copyright (C) 2024 Savoir-faire Linux, Inc
 * \license SPDX-License-Identifier: Apache-2.0
 *
 */

#include <stdint.h>

/**
 * \struct SV_ASDU
 * \brief Sample Value Application Specific Data Unit
 * */
typedef struct {
    char* svId; /**< User-defined unique string identifier used
                  for subscription */
    uint8_t* smpCnt; /**< Index of the SV message */
    uint8_t* confRef; /**< Configuration revision */
    uint8_t* smpSynch; /**< Synchronization mechanism of the clock used
                         for sending the SV */

    int seqDataLength; /**< Length of the sequence of data */
    uint8_t* seqDataAddr; /**< Sequence of measured voltage and
                            current values */
} SV_ASDU;

/**
 * \struct SV_payload
 * \brief Sample Value payload
 * */
typedef struct {
   uint8_t* APPID; /**< Application ID */
   uint8_t* length; /**< Message length */

   uint8_t noASDU; /**< Number of ASDU in the sequence */
   SV_ASDU* seqASDU; /**< Sequence of ASDU */
} SV_payload;
