	vcvtsd2sh xmm4, xmm0, xmm1
	vcvtsd2sh xmm4, xmm1
	vcvtsd2sh xmm3, xmm3, xmm0

%ifdef ERROR
	vcvtsd2sh xmm8, xmm10, xmm9
	vcvtsd2sh xmm27, xmm22, xmm17
%endif
