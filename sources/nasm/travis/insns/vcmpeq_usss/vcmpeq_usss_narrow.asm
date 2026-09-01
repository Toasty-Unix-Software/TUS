	vcmpeq_usss xmm7, xmm2, xmm0
	vcmpeq_usss xmm7, xmm0
	vcmpeq_usss xmm5, xmm3, xmm4
	vcmpeq_usss k2, xmm6, xmm1
	vcmpeq_usss k1, xmm5, xmm2

%ifdef ERROR
	vcmpeq_usss xmm9, xmm8, xmm11
	vcmpeq_usss k4, xmm12, xmm9
%endif
