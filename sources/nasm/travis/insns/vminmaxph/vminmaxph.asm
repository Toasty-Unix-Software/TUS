default rel
	vminmaxph xmm2, xmm4, oword [0xc19], 0x6d
	vminmaxph xmm3, xmm2, xmm0, 0xfd
	vminmaxph ymm4, ymm2, ymm7, 0x54
	vminmaxph ymm1, ymm6, ymm4, 0x8d
	vminmaxph zmm5, zmm7, zmm5, 0xab
	vminmaxph zmm1, zmm0, zword [0x3a8], 0x51
	vminmaxph xmm15, xmm13, xmm8, 0x39
	vminmaxph ymm8, ymm15, ymm14, 0x3
	vminmaxph zmm14, zmm9, zmm8, 0x46
	vminmaxph xmm19, xmm30, xmm30, 0xaf
	vminmaxph ymm19, ymm22, ymm31, 0x33
	vminmaxph zmm30, zmm19, zmm19, 0x53
	vminmaxph xmm5{k6}, xmm6, oword [0xa46], 0x2
	vminmaxph ymm0{k1}, ymm3, yword [0x91a], 0x4a
	vminmaxph zmm0{k6}, zmm5, zmm4, 0x3a
	vminmaxph xmm5{k2}{z}, xmm0, xmm7, 0x80
	vminmaxph ymm5{k1}{z}, ymm4, ymm2, 0x57
	vminmaxph zmm3{k2}{z}, zmm0, zmm4, 0x4a
	vminmaxph xmm7, xmm4, oword [eax+1], 0x63
	vminmaxph xmm4, xmm2, oword [eax+64], 0xb6
	vminmaxph ymm6, ymm3, yword [eax+1], 0x6
	vminmaxph ymm7, ymm0, yword [eax+64], 0x11
	vminmaxph zmm4, zmm7, zword [eax+1], 0xc6
	vminmaxph zmm5, zmm1, zword [eax+64], 0xa7
	vminmaxph xmm7, xmm5, [0x144], 0x44
	vminmaxph ymm7, ymm0, [0x794], 0xf9
	vminmaxph zmm1, zmm0, [0xa4e], 0x7c
