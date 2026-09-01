	vcmpnge_usss xmm4, xmm3, xmm5
	vcmpnge_usss xmm4, xmm5
	vcmpnge_usss xmm1, xmm0, xmm0
	vcmpnge_usss k5, xmm4, xmm7
	vcmpnge_usss k6, xmm3, xmm2

%ifdef ERROR
	vcmpnge_usss xmm11, xmm10, xmm12
	vcmpnge_usss k4, xmm15, xmm13
%endif
