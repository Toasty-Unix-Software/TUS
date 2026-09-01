	vsubbf16 xmm3, xmm3, xmm0
	vsubbf16 xmm5, xmm5, oword [0x63c]
	vsubbf16 ymm5, ymm0, yword [0x86e]
	vsubbf16 ymm1, ymm3, ymm0
	vsubbf16 zmm5, zmm1, zmm7
	vsubbf16 zmm0, zmm3, zmm7

%ifdef ERROR
	vsubbf16 xmm15, xmm8, xmm9
	vsubbf16 ymm14, ymm10, ymm9
	vsubbf16 zmm10, zmm13, zmm12
	vsubbf16 xmm27, xmm20, xmm23
	vsubbf16 ymm17, ymm26, ymm24
	vsubbf16 zmm18, zmm30, zmm16
%endif
