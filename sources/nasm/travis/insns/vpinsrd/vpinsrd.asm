default rel
	vpinsrd xmm3, xmm1, dword [0xa24], 0xa0
	vpinsrd xmm3, dword [0xa24], 0xa0
	vpinsrd xmm5, xmm0, dword [0x1ef], 0xf1
	vpinsrd xmm0, xmm1, eax, 0x41
	vpinsrd xmm0, eax, 0x41
	vpinsrd xmm2, xmm7, esi, 0xfe
	vpinsrd xmm9, xmm8, dword [0x2a9], 0x8b
	vpinsrd xmm15, xmm9, r14d, 0x1c
	vpinsrd xmm23, xmm31, dword [0x863], 0xc4
	vpinsrd xmm31, xmm18, r29d, 0xbd
	vpinsrd xmm2, xmm1, dword [eax+1], 0x8b
	vpinsrd xmm6, xmm2, dword [eax+64], 0x3d
	vpinsrd xmm0, xmm0, dword [eax+1], 0x9e
	vpinsrd xmm7, xmm0, dword [eax+64], 0x30
	vpinsrd xmm7, xmm3, [0x820], 0xcc
	vpinsrd xmm5, xmm6, [0xcf5], 0xbe
