	vinsertf32x4 ymm7, ymm1, xmm1, 0x45
	vinsertf32x4 ymm7, xmm1, 0x45
	vinsertf32x4 ymm0, ymm5, xmm0, 0x32
	vinsertf32x4 zmm7, zmm6, xmm5, 0xd7
	vinsertf32x4 zmm7, xmm5, 0xd7
	vinsertf32x4 zmm2, zmm2, oword [0xc5f], 0xda

%ifdef ERROR
	vinsertf32x4 ymm12, ymm13, xmm13, 0x38
	vinsertf32x4 zmm15, zmm9, xmm12, 0x5f
	vinsertf32x4 ymm22, ymm20, xmm27, 0xc8
	vinsertf32x4 zmm20, zmm16, xmm30, 0xd
%endif
