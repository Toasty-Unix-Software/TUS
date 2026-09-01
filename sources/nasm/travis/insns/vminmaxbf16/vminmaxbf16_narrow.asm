	vminmaxbf16 xmm0, xmm3, xmm2, 0xa6
	vminmaxbf16 xmm3, xmm2, xmm6, 0x6e
	vminmaxbf16 ymm1, ymm1, yword [0x4ba], 0xf0
	vminmaxbf16 ymm3, ymm0, ymm1, 0x99
	vminmaxbf16 zmm3, zmm2, zmm4, 0x55
	vminmaxbf16 zmm3, zmm1, zmm4, 0x35

%ifdef ERROR
	vminmaxbf16 xmm12, xmm9, xmm10, 0x8e
	vminmaxbf16 ymm9, ymm10, ymm15, 0x53
	vminmaxbf16 zmm11, zmm10, zmm9, 0x72
	vminmaxbf16 xmm24, xmm30, xmm20, 0x79
	vminmaxbf16 ymm25, ymm29, ymm28, 0x10
	vminmaxbf16 zmm29, zmm16, zmm23, 0x4a
%endif
