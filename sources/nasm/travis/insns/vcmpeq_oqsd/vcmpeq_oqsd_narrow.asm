	vcmpeq_oqsd k2, xmm7, xmm1
	vcmpeq_oqsd k2, xmm1, xmm4

%ifdef ERROR
	vcmpeq_oqsd k4, xmm11, xmm9
	vcmpeq_oqsd k1, xmm21, xmm17
%endif
