/*  Config file for Samraksh use: Same as the picocoin config
 *  Using "Minimal configuration for TLS 1.2 with PSK and AES-CCM ciphersuites" as base
 *  and then adding PKCS to it for
 *-------------------------------------------------------------------
 *  Minimal configuration for TLS 1.2 with PSK and AES-CCM ciphersuites
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
/*   Reduced configuration used by Picocoin.
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Reduced configuration used by Picocoin.
 *
 * See README.txt for usage instructions.
 *
 * Distinguishing features:
 * - no SSL/TLS;
 * - no X.509;
 * - ECDSA/PK and some other chosen crypto bits.
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* System support */
//#define MBEDTLS_HAVE_ASM  //commented out by Mukundan
//#define MBEDTLS_HAVE_TIME //commented out by Mukundan

/* mbed TLS feature support */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_PK_PARSE_EC_EXTENDED
#define MBEDTLS_ERROR_STRERROR_DUMMY
#define MBEDTLS_FS_IO

/* mbed TLS modules */
//#define MBEDTLS_AESNI_C //commented Mukundan
#define MBEDTLS_AES_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CIPHER_C //added by mukundan
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_MD_C
#define MBEDTLS_OID_C
//#define MBEDTLS_PADLOCK_C //commented by Mukundan
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_RIPEMD160_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA256_C

//Everything below copied from the ccm-psk-tls1_2 config by Mukundan

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/* Save some RAM by adjusting to your exact needs */
#define MBEDTLS_PSK_MAX_LEN    16 /* 128-bits keys are generally enough */

/*
 * You should adjust this to the exact number of sources you're using: default
 * is the "platform_entropy_poll" source, but you may want to add other ones
 * Minimum is 2 for the entropy test suite.
 */
#define MBEDTLS_ENTROPY_MAX_SOURCES 2


#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */
