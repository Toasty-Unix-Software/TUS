	vmovaps xmm3, xmm5
	vmovaps xmm5, xmm0
	vmovaps xmm0, xmm4
	vmovaps xmm5, xmm2
	vmovaps ymm6, ymm2
	vmovaps ymm6, yword [0x852]
	vmovaps yword [0xebc], ymm7
	vmovaps ymm4, ymm4

%ifdef ERROR
	vmovaps xmm14, xmm15
	vmovaps xmm15, xmm12
	vmovaps ymm14, ymm11
	vmovaps ymm12, ymm11
	vmovaps xmm21, xmm28
	vmovaps xmm24, xmm19
	vmovaps ymm29, ymm23
	vmovaps ymm26, ymm27
%endif
