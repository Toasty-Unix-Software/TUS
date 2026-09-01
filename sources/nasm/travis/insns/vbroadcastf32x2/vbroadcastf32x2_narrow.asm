	vbroadcastf32x2 ymm7, xmm6
	vbroadcastf32x2 ymm4, qword [0x5ba]
	vbroadcastf32x2 zmm3, xmm0
	vbroadcastf32x2 zmm3, xmm4

%ifdef ERROR
	vbroadcastf32x2 ymm14, xmm13
	vbroadcastf32x2 zmm15, xmm10
	vbroadcastf32x2 ymm16, xmm30
	vbroadcastf32x2 zmm26, xmm26
%endif
