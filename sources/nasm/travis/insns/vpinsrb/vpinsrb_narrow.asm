	vpinsrb xmm1, xmm0, byte [0x3f4], 0x5
	vpinsrb xmm1, byte [0x3f4], 0x5
	vpinsrb xmm5, xmm4, byte [0x5b8], 0xa4
	vpinsrb xmm7, xmm5, cl, 0x88
	vpinsrb xmm7, cl, 0x88
	vpinsrb xmm1, xmm4, byte [0x2e4], 0x9e
	vpinsrb xmm6, xmm0, edx, 0x2a
	vpinsrb xmm6, edx, 0x2a
	vpinsrb xmm5, xmm6, ecx, 0x6f

%ifdef ERROR
	vpinsrb xmm8, xmm13, byte [0x6ec], 0xe3
	vpinsrb xmm14, xmm12, r8b, 0x1f
	vpinsrb xmm15, xmm14, r14d, 0x9f
%endif
