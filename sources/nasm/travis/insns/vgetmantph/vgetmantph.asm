default rel
	vgetmantph xmm1, xmm4, 0x47
	vgetmantph xmm1, xmm5, 0xb3
	vgetmantph ymm1, yword [0xccb], 0xe1
	vgetmantph ymm1, ymm6, 0x97
	vgetmantph zmm4, zmm2, 0xb5
	vgetmantph zmm1, zmm2, 0x4e
	vgetmantph xmm10, xmm12, 0x5c
	vgetmantph ymm13, ymm12, 0x49
	vgetmantph zmm13, zmm12, 0x32
	vgetmantph xmm18, xmm25, 0xc7
	vgetmantph ymm22, ymm17, 0xca
	vgetmantph zmm26, zmm31, 0xad
	vgetmantph xmm5{k6}, xmm6, 0xb1
	vgetmantph ymm7{k7}, ymm2, 0x7b
	vgetmantph zmm6{k1}, zword [0xb8c], 0x10
	vgetmantph xmm0{k7}{z}, xmm5, 0x40
	vgetmantph ymm7{k3}{z}, ymm3, 0xd
	vgetmantph zmm4{k6}{z}, zmm3, 0x36
	vgetmantph xmm2, oword [eax+1], 0xfe
	vgetmantph xmm3, oword [eax+64], 0x82
	vgetmantph ymm5, yword [eax+1], 0xa1
	vgetmantph ymm4, yword [eax+64], 0x4b
	vgetmantph zmm6, zword [eax+1], 0x3f
	vgetmantph zmm1, zword [eax+64], 0xb6
	vgetmantph xmm3, [0xc89], 0xfa
	vgetmantph ymm0, [0x11e], 0x77
	vgetmantph zmm7, [0x849], 0xea
