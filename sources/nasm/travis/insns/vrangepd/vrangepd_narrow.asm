	vrangepd xmm6, xmm5, xmm5, 0xc2
	vrangepd xmm6, xmm5, 0xc2
	vrangepd xmm0, xmm6, oword [0x104], 0xd4
	vrangepd ymm7, ymm4, yword [0xd11], 0x2b
	vrangepd ymm7, yword [0xd11], 0x2b
	vrangepd ymm5, ymm7, yword [0x187], 0x45
	vrangepd zmm2, zmm1, zmm2, 0xfb
	vrangepd zmm2, zmm2, 0xfb
	vrangepd zmm3, zmm7, zmm1, 0x1e

%ifdef ERROR
	vrangepd xmm15, xmm15, xmm12, 0xab
	vrangepd ymm8, ymm9, ymm11, 0xb8
	vrangepd zmm13, zmm11, zmm11, 0x1b
	vrangepd xmm22, xmm21, xmm26, 0x83
	vrangepd ymm18, ymm31, ymm23, 0xea
	vrangepd zmm19, zmm20, zmm24, 0xd5
%endif
