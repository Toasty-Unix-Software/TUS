	vdivbf16 xmm4, xmm7, xmm1
	vdivbf16 xmm1, xmm5, xmm0
	vdivbf16 ymm5, ymm4, yword [0xdee]
	vdivbf16 ymm2, ymm3, ymm2
	vdivbf16 zmm4, zmm7, zmm5
	vdivbf16 zmm7, zmm7, zmm6

%ifdef ERROR
	vdivbf16 xmm10, xmm14, xmm11
	vdivbf16 ymm11, ymm12, ymm12
	vdivbf16 zmm14, zmm13, zmm10
	vdivbf16 xmm23, xmm31, xmm29
	vdivbf16 ymm23, ymm18, ymm16
	vdivbf16 zmm28, zmm30, zmm20
%endif
