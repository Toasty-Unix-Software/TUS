	vcmpeq_oqss k7, xmm5, xmm5
	vcmpeq_oqss k5, xmm5, xmm5

%ifdef ERROR
	vcmpeq_oqss k1, xmm11, xmm8
	vcmpeq_oqss k5, xmm27, xmm22
%endif
