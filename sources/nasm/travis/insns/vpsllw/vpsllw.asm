default rel
	vpsllw xmm1, xmm3, oword [0xdb9]
	vpsllw xmm1, oword [0xdb9]
	vpsllw xmm3, xmm7, xmm2
	vpsllw xmm5, xmm3, 0x3e
	vpsllw xmm5, 0x3e
	vpsllw xmm5, xmm3, 0x4b
	vpsllw ymm7, ymm6, xmm1
	vpsllw ymm7, xmm1
	vpsllw ymm5, ymm0, oword [0xbf4]
	vpsllw ymm5, ymm2, 0x38
	vpsllw ymm5, 0x38
	vpsllw ymm5, ymm1, 0x21
	vpsllw xmm10, xmm14, xmm15
	vpsllw xmm11, xmm14, 0x90
	vpsllw ymm14, ymm14, xmm12
	vpsllw ymm10, ymm10, 0x9f
	vpsllw xmm31, xmm19, xmm28
	vpsllw xmm30, xmm25, 0xa0
	vpsllw ymm17, ymm27, xmm16
	vpsllw ymm16, ymm30, 0xf1
	vpsllw xmm0{k2}, xmm1, oword [0x934]
	vpsllw ymm7{k7}, ymm3, xmm0
	vpsllw zmm0{k6}, zmm2, xmm4
	vpsllw xmm4{k3}, xmm5, 0xd4
	vpsllw ymm7{k7}, ymm5, 0xbd
	vpsllw zmm2{k2}, zmm5, 0xb
	vpsllw xmm4{k6}{z}, xmm7, xmm2
	vpsllw ymm5{k4}{z}, ymm6, xmm6
	vpsllw zmm7{k6}{z}, zmm3, oword [0x2c3]
	vpsllw xmm3{k4}{z}, oword [0xc53], 0x82
	vpsllw ymm4{k6}{z}, yword [0xf1e], 0x9b
	vpsllw zmm6{k5}{z}, zmm4, 0x4d
	vpsllw xmm3, xmm1, oword [eax+1]
	vpsllw xmm5, xmm1, oword [eax+64]
	vpsllw ymm7, ymm6, oword [eax+1]
	vpsllw ymm1, ymm3, oword [eax+64]
	vpsllw xmm2, xmm3, oword [eax+1]
	vpsllw xmm4, xmm6, oword [eax+64]
	vpsllw ymm0, ymm4, oword [eax+1]
	vpsllw ymm7, ymm2, oword [eax+64]
	vpsllw zmm6, zmm1, oword [eax+1]
	vpsllw zmm0, zmm3, oword [eax+64]
	vpsllw xmm2, oword [eax+1], 0xa6
	vpsllw xmm7, oword [eax+64], 0x4a
	vpsllw ymm2, yword [eax+1], 0xdb
	vpsllw ymm4, yword [eax+64], 0x59
	vpsllw zmm7, zword [eax+1], 0x1c
	vpsllw zmm5, zword [eax+64], 0xd8
	vpsllw xmm1, xmm6, [0xde0]
	vpsllw ymm6, ymm3, [0xd9f]
	vpsllw xmm7, xmm7, [0xff7]
	vpsllw ymm0, ymm7, [0x6ba]
	vpsllw zmm1, zmm2, [0x979]
	vpsllw xmm6, [0x993], 0x5b
	vpsllw ymm0, [0xb90], 0x69
	vpsllw zmm3, [0xd98], 0x2a
