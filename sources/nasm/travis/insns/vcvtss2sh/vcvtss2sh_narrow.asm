	vcvtss2sh xmm4, xmm4, xmm2
	vcvtss2sh xmm4, xmm2
	vcvtss2sh xmm0, xmm4, xmm3

%ifdef ERROR
	vcvtss2sh xmm8, xmm15, xmm11
	vcvtss2sh xmm21, xmm25, xmm29
%endif
