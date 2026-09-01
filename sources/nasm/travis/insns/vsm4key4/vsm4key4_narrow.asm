	vsm4key4 xmm7, xmm1, xmm4
	vsm4key4 xmm0, xmm3, xmm0
	vsm4key4 ymm3, ymm6, ymm0
	vsm4key4 ymm3, ymm2, ymm5
	vsm4key4 zmm7, zmm0, zword [0x8b2]
	vsm4key4 zmm0, zmm5, zmm4

%ifdef ERROR
	vsm4key4 xmm8, xmm13, xmm12
	vsm4key4 ymm11, ymm11, ymm11
	vsm4key4 zmm14, zmm11, zmm9
	vsm4key4 xmm17, xmm20, xmm26
	vsm4key4 ymm28, ymm17, ymm23
	vsm4key4 zmm29, zmm25, zmm29
%endif
