	vmovups xmm4, xmm7
	vmovups xmm2, xmm4
	vmovups xmm2, xmm3
	vmovups xmm2, xmm1
	vmovups ymm1, yword [0x69d]
	vmovups ymm4, yword [0xa8e]
	vmovups ymm2, ymm0
	vmovups yword [0x93b], ymm6

%ifdef ERROR
	vmovups xmm10, xmm13
	vmovups xmm10, xmm8
	vmovups ymm11, ymm11
	vmovups ymm9, ymm14
	vmovups xmm29, xmm20
	vmovups xmm24, xmm20
	vmovups ymm17, ymm18
	vmovups ymm17, ymm29
%endif
