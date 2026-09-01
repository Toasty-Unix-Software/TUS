	vbroadcasti32x2 xmm7, xmm0
	vbroadcasti32x2 xmm0, qword [0x3b1]
	vbroadcasti32x2 ymm1, qword [0x6c3]
	vbroadcasti32x2 ymm3, xmm4
	vbroadcasti32x2 zmm3, xmm6
	vbroadcasti32x2 zmm2, xmm5

%ifdef ERROR
	vbroadcasti32x2 xmm15, xmm8
	vbroadcasti32x2 ymm15, xmm13
	vbroadcasti32x2 zmm8, xmm9
	vbroadcasti32x2 xmm18, xmm24
	vbroadcasti32x2 ymm23, xmm27
	vbroadcasti32x2 zmm24, xmm28
%endif
