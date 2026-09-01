	vminbf16 xmm1, xmm4, xmm7
	vminbf16 xmm3, xmm5, xmm6
	vminbf16 ymm5, ymm3, yword [0xb5a]
	vminbf16 ymm6, ymm2, ymm4
	vminbf16 zmm4, zmm0, zmm1
	vminbf16 zmm2, zmm6, zmm2

%ifdef ERROR
	vminbf16 xmm8, xmm10, xmm13
	vminbf16 ymm15, ymm12, ymm8
	vminbf16 zmm10, zmm8, zmm13
	vminbf16 xmm20, xmm22, xmm24
	vminbf16 ymm20, ymm17, ymm29
	vminbf16 zmm17, zmm28, zmm31
%endif
