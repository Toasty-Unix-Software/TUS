	vcmpnle_usss xmm3, xmm3, xmm7
	vcmpnle_usss xmm3, xmm7
	vcmpnle_usss xmm6, xmm5, xmm2
	vcmpnle_usss k5, xmm0, xmm5
	vcmpnle_usss k6, xmm4, dword [0x62d]

%ifdef ERROR
	vcmpnle_usss xmm13, xmm10, xmm13
	vcmpnle_usss k7, xmm9, xmm13
%endif
