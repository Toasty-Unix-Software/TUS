default rel
	vrangepd xmm6, xmm5, xmm5, 0xc2
	vrangepd xmm6, xmm5, 0xc2
	vrangepd xmm0, xmm6, oword [0x104], 0xd4
	vrangepd ymm7, ymm4, yword [0xd11], 0x2b
	vrangepd ymm7, yword [0xd11], 0x2b
	vrangepd ymm5, ymm7, yword [0x187], 0x45
	vrangepd zmm2, zmm1, zmm2, 0xfb
	vrangepd zmm2, zmm2, 0xfb
	vrangepd zmm3, zmm7, zmm1, 0x1e
	vrangepd xmm15, xmm15, xmm12, 0xab
	vrangepd ymm8, ymm9, ymm11, 0xb8
	vrangepd zmm13, zmm11, zmm11, 0x1b
	vrangepd xmm22, xmm21, xmm26, 0x83
	vrangepd ymm18, ymm31, ymm23, 0xea
	vrangepd zmm19, zmm20, zmm24, 0xd5
	vrangepd xmm0{k5}, xmm4, xmm6, 0x6e
	vrangepd ymm5{k4}, ymm1, ymm6, 0x34
	vrangepd zmm2{k3}, zmm7, zword [0x573], 0xca
	vrangepd xmm4{k5}{z}, xmm2, xmm0, 0x1a
	vrangepd ymm7{k1}{z}, ymm5, yword [0x353], 0x96
	vrangepd zmm1{k3}{z}, zmm3, zmm3, 0xd3
	vrangepd xmm2, xmm6, oword [eax+1], 0x3a
	vrangepd xmm6, xmm7, oword [eax+64], 0x30
	vrangepd ymm1, ymm1, yword [eax+1], 0x2f
	vrangepd ymm2, ymm1, yword [eax+64], 0x5c
	vrangepd zmm5, zmm0, zword [eax+1], 0xfb
	vrangepd zmm0, zmm0, zword [eax+64], 0x6d
	vrangepd xmm4, xmm0, [0x311], 0x7
	vrangepd ymm2, ymm1, [0x740], 0x88
	vrangepd zmm0, zmm2, [0xe7f], 0x1
