	vpcomd xmm0, xmm2, xmm2, 0xb0
	vpcomd xmm0, xmm2, 0xb0
	vpcomd xmm0, xmm6, xmm6, 0xa

%ifdef ERROR
	vpcomd xmm15, xmm15, xmm15, 0x96
%endif
