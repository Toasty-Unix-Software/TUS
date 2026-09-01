	vmovlhps xmm1, xmm1, xmm5
	vmovlhps xmm1, xmm5
	vmovlhps xmm2, xmm1, xmm5

%ifdef ERROR
	vmovlhps xmm11, xmm8, xmm11
	vmovlhps xmm29, xmm16, xmm30
%endif
