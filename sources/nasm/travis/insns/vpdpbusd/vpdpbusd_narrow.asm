	vpdpbusd xmm7, xmm6, xmm1
	vpdpbusd xmm0, xmm4, xmm3
	vpdpbusd ymm3, ymm1, ymm4
	vpdpbusd ymm2, ymm7, ymm0
	vpdpbusd xmm2, xmm2, xmm7
	vpdpbusd xmm2, xmm7
	vpdpbusd xmm1, xmm2, xmm3
	vpdpbusd ymm6, ymm0, ymm6
	vpdpbusd ymm6, ymm6
	vpdpbusd ymm7, ymm1, ymm3

%ifdef ERROR
	vpdpbusd xmm11, xmm8, xmm8
	vpdpbusd ymm9, ymm10, ymm11
	vpdpbusd xmm15, xmm9, xmm8
	vpdpbusd ymm15, ymm11, ymm15
	vpdpbusd xmm26, xmm18, xmm30
	vpdpbusd ymm19, ymm16, ymm20
	vpdpbusd xmm25, xmm27, xmm18
	vpdpbusd ymm31, ymm29, ymm21
%endif
