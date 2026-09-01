	vmovsldup xmm4, xmm4
	vmovsldup xmm4, oword [0xbcb]
	vmovsldup ymm4, ymm1
	vmovsldup ymm4, ymm7
	vmovsldup xmm5, xmm6
	vmovsldup xmm0, xmm2
	vmovsldup ymm1, ymm3
	vmovsldup ymm2, ymm2

%ifdef ERROR
	vmovsldup xmm15, xmm8
	vmovsldup ymm12, ymm14
	vmovsldup xmm12, xmm11
	vmovsldup ymm15, ymm8
	vmovsldup xmm28, xmm22
	vmovsldup ymm26, ymm23
	vmovsldup xmm26, xmm19
	vmovsldup ymm30, ymm30
%endif
