	vsm4rnds4 xmm2, xmm6, oword [0xb98]
	vsm4rnds4 xmm3, xmm4, xmm5
	vsm4rnds4 ymm6, ymm1, yword [0x7d3]
	vsm4rnds4 ymm5, ymm5, ymm1
	vsm4rnds4 zmm5, zmm5, zword [0xbdd]
	vsm4rnds4 zmm6, zmm5, zmm1

%ifdef ERROR
	vsm4rnds4 xmm14, xmm8, xmm15
	vsm4rnds4 ymm11, ymm14, ymm8
	vsm4rnds4 zmm8, zmm8, zmm10
	vsm4rnds4 xmm23, xmm24, xmm29
	vsm4rnds4 ymm21, ymm30, ymm20
	vsm4rnds4 zmm27, zmm21, zmm25
%endif
