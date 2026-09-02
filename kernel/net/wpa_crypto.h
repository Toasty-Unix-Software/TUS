/*
 * wpa_crypto.h - the WPA2-PSK key hierarchy (IEEE 802.11i)
 *
 * This is the crypto core only: PSK derivation from a passphrase,
 * pairwise transient key (PTK) derivation from the 4-way handshake
 * nonces, and EAPOL-Key MIC computation/verification. It has no
 * dependency on any NIC.
 *
 * What is NOT here, and why: actually joining a network needs 802.11
 * management frames (probe/auth/assoc) and the ath9k-htc USB command
 * protocol to drive them, neither of which kernel/drivers/ath9k/
 * implements yet (it only loads firmware - see its header). There is
 * also no way to boot-test a real handshake in this environment: QEMU
 * has no ath9k-htc emulation and no virtual AP, so any "association"
 * code path here would be unverifiable by construction. wpa_selftest()
 * below is the part that genuinely can be checked - against the
 * published IEEE 802.11i-2004 Annex H.4 test vectors - and is.
 */

#ifndef TUS_NET_WPA_CRYPTO_H
#define TUS_NET_WPA_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

/* PBKDF2-HMAC-SHA1(passphrase, ssid, 4096, 256 bits) - IEEE 802.11i
 * 8.5.1.2. `psk_out` must hold 32 bytes. */
void wpa_psk_from_passphrase(const char *passphrase, const char *ssid,
                             size_t ssid_len, uint8_t psk_out[32]);

/* PTK = PRF-512(PMK, "Pairwise key expansion",
 *               Min(AA,SA) || Max(AA,SA) || Min(ANonce,SNonce) || Max(ANonce,SNonce))
 * aa/sa are 6-byte MACs, anonce/snonce are 32 bytes each.
 * ptk_out must hold 64 bytes: KCK[0:16] KEK[16:32] TK[32:48] (+ 16
 * unused bytes for WPA2's 512-bit PRF output, matching hostapd/
 * wpa_supplicant's own PTK layout). */
void wpa_derive_ptk(const uint8_t pmk[32], const uint8_t aa[6],
                    const uint8_t sa[6], const uint8_t anonce[32],
                    const uint8_t snonce[32], uint8_t ptk_out[64]);

/* HMAC-SHA1-128 MIC over an EAPOL-Key frame, using KCK (ptk[0:16]).
 * `frame` must have its MIC field already zeroed. mic_out gets 16
 * bytes, of which the low 16 are used for WPA2 (HMAC-SHA1-128). */
void wpa_eapol_mic(const uint8_t kck[16], const uint8_t *frame, size_t len,
                   uint8_t mic_out[16]);

/* Runs the crypto core against the fixed IEEE 802.11i-2004 Annex H.4
 * test vectors (SSID "IEEE", passphrase "password" -> a known PSK;
 * and a full worked 4-way-handshake example -> a known PTK/MIC).
 * Returns 0 if every check matches, -1 otherwise (prints which). */
int wpa_selftest(void);

#endif /* TUS_NET_WPA_CRYPTO_H */
