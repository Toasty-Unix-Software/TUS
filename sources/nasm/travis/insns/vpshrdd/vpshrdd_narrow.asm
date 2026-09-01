	vpshrdd xmm7, xmm3, xmm3, 0x96
	vpshrdd xmm7, xmm3, 0x96
	vpshrdd xmm1, xmm6, oword [0xabb], 0x66
	vpshrdd ymm4, ymm5, ymm0, 0x46
	vpshrdd ymm4, ymm0, 0x46
	vpshrdd ymm2, ymm3, ymm3, 0xb3
	vpshrdd zmm5, zmm7, zmm6, 0xa5
	vpshrdd zmm5, zmm6, 0xa5
	vpshrdd zmm5, zmm1, zword [0xfb2], 0xda

%ifdef ERROR
	vpshrdd xmm13, xmm11, xmm13, 0xd6
	vpshrdd ymm10, ymm12, ymm8, 0x54
	vpshrdd zmm11, zmm11, zmm13, 0xa2
	vpshrdd xmm25, xmm27, xmm30, 0x2c
	vpshrdd ymm20, ymm19, ymm31, 0x9c
	vpshrdd zmm18, zmm31, zmm20, 0xfe
%endif
