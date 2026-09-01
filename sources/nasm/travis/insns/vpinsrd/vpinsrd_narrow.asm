	vpinsrd xmm3, xmm1, dword [0xa24], 0xa0
	vpinsrd xmm3, dword [0xa24], 0xa0
	vpinsrd xmm5, xmm0, dword [0x1ef], 0xf1
	vpinsrd xmm0, xmm1, eax, 0x41
	vpinsrd xmm0, eax, 0x41
	vpinsrd xmm2, xmm7, esi, 0xfe

%ifdef ERROR
	vpinsrd xmm9, xmm8, dword [0x2a9], 0x8b
	vpinsrd xmm15, xmm9, r14d, 0x1c
	vpinsrd xmm23, xmm31, dword [0x863], 0xc4
	vpinsrd xmm31, xmm18, r29d, 0xbd
%endif
