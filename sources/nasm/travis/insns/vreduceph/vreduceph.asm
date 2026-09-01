default rel
	vreduceph xmm6, xmm3, 0x31
	vreduceph xmm2, xmm0, 0xde
	vreduceph ymm6, ymm6, 0x2c
	vreduceph ymm3, ymm4, 0x2e
	vreduceph zmm2, zmm0, 0xce
	vreduceph zmm7, zword [0x45c], 0xf8
	vreduceph xmm8, xmm15, 0x6d
	vreduceph ymm13, ymm11, 0x83
	vreduceph zmm8, zmm12, 0x4c
	vreduceph xmm22, xmm28, 0x69
	vreduceph ymm23, ymm22, 0x8e
	vreduceph zmm27, zmm25, 0x9b
	vreduceph xmm1{k6}, xmm5, 0x18
	vreduceph ymm5{k7}, ymm6, 0x9f
	vreduceph zmm0{k5}, zmm5, 0xa6
	vreduceph xmm6{k2}{z}, oword [0xf02], 0xd2
	vreduceph ymm0{k6}{z}, ymm6, 0x2
	vreduceph zmm7{k3}{z}, zmm4, 0x9c
	vreduceph xmm4, oword [eax+1], 0x7
	vreduceph xmm6, oword [eax+64], 0xde
	vreduceph ymm3, yword [eax+1], 0x5d
	vreduceph ymm7, yword [eax+64], 0xf0
	vreduceph zmm2, zword [eax+1], 0xa6
	vreduceph zmm3, zword [eax+64], 0x25
	vreduceph xmm6, [0xed0], 0xe0
	vreduceph ymm7, [0x686], 0xdb
	vreduceph zmm7, [0xfa4], 0x5b
