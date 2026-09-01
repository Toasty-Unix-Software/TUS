default rel
	vminmaxpd xmm7, xmm7, oword [0x69e], 0x15
	vminmaxpd xmm6, xmm0, xmm2, 0x34
	vminmaxpd ymm0, ymm0, yword [0xc16], 0x9d
	vminmaxpd ymm7, ymm7, yword [0x897], 0x7a
	vminmaxpd zmm4, zmm5, zmm4, 0xc3
	vminmaxpd zmm7, zmm7, zword [0xdcd], 0xfa
	vminmaxpd xmm11, xmm8, xmm12, 0xa2
	vminmaxpd ymm11, ymm10, ymm15, 0x5a
	vminmaxpd zmm8, zmm13, zmm9, 0x1
	vminmaxpd xmm30, xmm26, xmm18, 0x1d
	vminmaxpd ymm22, ymm19, ymm21, 0x1a
	vminmaxpd zmm20, zmm28, zmm23, 0xc6
	vminmaxpd xmm5{k1}, xmm7, oword [0xc91], 0xaf
	vminmaxpd ymm3{k5}, ymm2, yword [0x5ab], 0x93
	vminmaxpd zmm2{k6}, zmm5, zmm3, 0xad
	vminmaxpd xmm2{k3}{z}, xmm1, oword [0x9b2], 0xf9
	vminmaxpd ymm3{k5}{z}, ymm0, yword [0x302], 0x27
	vminmaxpd zmm5{k5}{z}, zmm0, zmm3, 0x4e
	vminmaxpd xmm5, xmm1, oword [eax+1], 0x8b
	vminmaxpd xmm7, xmm7, oword [eax+64], 0x9e
	vminmaxpd ymm7, ymm2, yword [eax+1], 0x70
	vminmaxpd ymm3, ymm0, yword [eax+64], 0xa6
	vminmaxpd zmm4, zmm0, zword [eax+1], 0xa4
	vminmaxpd zmm4, zmm1, zword [eax+64], 0xf4
	vminmaxpd xmm5, xmm4, [0xc1b], 0xe8
	vminmaxpd ymm5, ymm2, [0x443], 0x5e
	vminmaxpd zmm2, zmm0, [0x5a0], 0x89
