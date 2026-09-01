	vcmpngt_usss xmm0, xmm1, xmm6
	vcmpngt_usss xmm0, xmm6
	vcmpngt_usss xmm6, xmm6, xmm5
	vcmpngt_usss k6, xmm6, xmm0
	vcmpngt_usss k2, xmm0, dword [0x36d]

%ifdef ERROR
	vcmpngt_usss xmm15, xmm9, xmm12
	vcmpngt_usss k7, xmm15, xmm15
%endif
