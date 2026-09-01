	vrcp14ss xmm4, xmm7, xmm4
	vrcp14ss xmm4, xmm4
	vrcp14ss xmm4, xmm2, xmm0

%ifdef ERROR
	vrcp14ss xmm14, xmm13, xmm15
	vrcp14ss xmm29, xmm28, xmm22
%endif
