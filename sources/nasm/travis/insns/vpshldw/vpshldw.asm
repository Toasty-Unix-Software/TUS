default rel
	vpshldw xmm7, xmm4, oword [0xe49], 0x5d
	vpshldw xmm7, oword [0xe49], 0x5d
	vpshldw xmm1, xmm1, xmm1, 0xfd
	vpshldw ymm4, ymm2, ymm7, 0xaf
	vpshldw ymm4, ymm7, 0xaf
	vpshldw ymm2, ymm3, ymm4, 0x5f
	vpshldw zmm0, zmm1, zmm4, 0x1
	vpshldw zmm0, zmm4, 0x1
	vpshldw zmm1, zmm1, zword [0x7d1], 0x20
	vpshldw xmm10, xmm9, xmm8, 0xa2
	vpshldw ymm15, ymm9, ymm13, 0x64
	vpshldw zmm12, zmm11, zmm8, 0xa0
	vpshldw xmm24, xmm24, xmm16, 0x64
	vpshldw ymm30, ymm29, ymm29, 0xb4
	vpshldw zmm25, zmm30, zmm16, 0x53
	vpshldw xmm3{k5}, xmm1, xmm0, 0xfb
	vpshldw ymm0{k2}, ymm5, ymm1, 0x1
	vpshldw zmm6{k7}, zmm6, zmm5, 0x52
	vpshldw xmm0{k1}{z}, xmm5, oword [0x256], 0x3
	vpshldw ymm5{k1}{z}, ymm6, ymm7, 0xbf
	vpshldw zmm4{k2}{z}, zmm1, zmm5, 0xff
	vpshldw xmm7, xmm7, oword [eax+1], 0xc9
	vpshldw xmm7, xmm4, oword [eax+64], 0x18
	vpshldw ymm3, ymm4, yword [eax+1], 0x3d
	vpshldw ymm7, ymm2, yword [eax+64], 0x92
	vpshldw zmm4, zmm5, zword [eax+1], 0xa5
	vpshldw zmm1, zmm5, zword [eax+64], 0xd4
	vpshldw xmm1, xmm3, [0xcea], 0xb4
	vpshldw ymm5, ymm0, [0xcf6], 0xac
	vpshldw zmm3, zmm2, [0x74d], 0xab
