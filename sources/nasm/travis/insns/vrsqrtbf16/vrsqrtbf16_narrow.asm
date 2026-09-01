	vrsqrtbf16 xmm6, xmm6
	vrsqrtbf16 xmm1, oword [0x7fd]
	vrsqrtbf16 ymm3, yword [0x1b2]
	vrsqrtbf16 ymm3, ymm4
	vrsqrtbf16 zmm0, zmm5
	vrsqrtbf16 zmm1, zmm5

%ifdef ERROR
	vrsqrtbf16 xmm11, xmm10
	vrsqrtbf16 ymm13, ymm15
	vrsqrtbf16 zmm12, zmm11
	vrsqrtbf16 xmm19, xmm28
	vrsqrtbf16 ymm23, ymm18
	vrsqrtbf16 zmm29, zmm22
%endif
