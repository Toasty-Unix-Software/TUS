	vpinsrw xmm6, xmm1, word [0xc12], 0x49
	vpinsrw xmm6, word [0xc12], 0x49
	vpinsrw xmm6, xmm3, word [0x3dc], 0xe5
	vpinsrw xmm2, xmm5, dx, 0x8e
	vpinsrw xmm2, dx, 0x8e
	vpinsrw xmm4, xmm4, bx, 0xab
	vpinsrw xmm7, xmm1, eax, 0xd1
	vpinsrw xmm7, eax, 0xd1
	vpinsrw xmm6, xmm5, esi, 0x7a

%ifdef ERROR
	vpinsrw xmm10, xmm13, word [0xba4], 0x1e
	vpinsrw xmm14, xmm8, r12w, 0xc9
	vpinsrw xmm12, xmm9, r11d, 0x63
%endif
