/**
 * \file sv_parser.h
 * \brief Tool to parse IEC 61850 Sampled Values from the packet payload
 *
 * \copyright Copyright (C) 2024 Savoir-faire Linux, Inc
 * \license SPDX-License-Identifier: Apache-2.0
 *
 */

#include <stdint.h>

#define BAD_FORMAT -1 /**< Error code when using a wrong sv payload */

/**
 * \struct SV_ASDU
 * \brief Sample Value Application Specific Data Unit
 * */
struct SV_ASDU {
        char *svID; /**< User-defined unique string identifier used
                  for subscription */
        char *datSet; /**< The name of the data set being sent */

        uint16_t smpCnt; /**< Index of the SV message */
        uint32_t confRev; /**< Configuration revision */
        uint8_t smpSynch; /**< Synchronization mechanism of the clock used
                         for sending the SV */
        uint8_t smpMod; /**< Sample Mode :samples/period, sampl/sec,
                          sec/sampl. */
        uint16_t smpRate; /**< Sample rate */
        uint64_t refrTm; /**< Time of the first sample */

        int seqDataLength; /**< Length of the sequence of data */
        uint8_t *seqData; /**< Sequence of measured voltage and
                            current values */
};

/**
 * \struct SV_payload
 * \brief Sample Value payload
 * */
struct SV_payload {
        uint16_t APPID; /**< Application ID */
        uint16_t length; /**< Message length */

        uint8_t noASDU; /**< Number of ASDU in the sequence */
        struct SV_ASDU *seqASDU; /**< Sequence of ASDU */
};

// constructor & destructor
struct SV_payload* create_SV();
void free_SV(struct SV_payload *sv);

// constructor & destructor
struct SV_payload* create_SV();
void free_SV(struct SV_payload *sv);

// parsing functions
int parse_SV_ASDU(const uint8_t *rawData, int rawDataLength,
                  struct SV_ASDU *asdu);
int parse_SV_payload(const uint8_t *payload, struct SV_payload *sv);
