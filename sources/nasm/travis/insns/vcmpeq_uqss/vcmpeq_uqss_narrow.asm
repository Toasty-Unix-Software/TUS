	vcmpeq_uqss xmm2, xmm5, xmm3
	vcmpeq_uqss xmm2, xmm3
	vcmpeq_uqss xmm2, xmm5, xmm1
	vcmpeq_uqss k4, xmm2, xmm4
	vcmpeq_uqss k2, xmm3, xmm4

%ifdef ERROR
	vcmpeq_uqss xmm11, xmm12, xmm14
	vcmpeq_uqss k3, xmm15, xmm15
%endif
