	vpshldd xmm1, xmm7, xmm5, 0x34
	vpshldd xmm1, xmm5, 0x34
	vpshldd xmm3, xmm5, xmm0, 0xc6
	vpshldd ymm3, ymm4, yword [0xcd6], 0xca
	vpshldd ymm3, yword [0xcd6], 0xca
	vpshldd ymm2, ymm5, ymm3, 0x53
	vpshldd zmm4, zmm3, zmm6, 0x12
	vpshldd zmm4, zmm6, 0x12
	vpshldd zmm3, zmm1, zmm7, 0xc5

%ifdef ERROR
	vpshldd xmm15, xmm14, xmm11, 0xae
	vpshldd ymm12, ymm12, ymm11, 0x90
	vpshldd zmm10, zmm9, zmm14, 0x62
	vpshldd xmm23, xmm20, xmm23, 0xc3
	vpshldd ymm28, ymm23, ymm28, 0x2d
	vpshldd zmm22, zmm31, zmm22, 0x5d
%endif
