	vpsravw xmm4, xmm6, xmm0
	vpsravw xmm4, xmm0
	vpsravw xmm7, xmm6, xmm7
	vpsravw ymm2, ymm3, ymm2
	vpsravw ymm2, ymm2
	vpsravw ymm1, ymm1, yword [0x115]
	vpsravw zmm2, zmm3, zword [0x180]
	vpsravw zmm2, zword [0x180]
	vpsravw zmm5, zmm3, zword [0x2b8]

%ifdef ERROR
	vpsravw xmm13, xmm15, xmm13
	vpsravw ymm8, ymm15, ymm8
	vpsravw zmm8, zmm14, zmm14
	vpsravw xmm19, xmm28, xmm18
	vpsravw ymm24, ymm28, ymm21
	vpsravw zmm20, zmm21, zmm26
%endif
