default rel
	vgetmantpd xmm3, xmm3, 0x35
	vgetmantpd xmm4, xmm4, 0x72
	vgetmantpd ymm3, ymm5, 0xef
	vgetmantpd ymm1, yword [0xf86], 0xa0
	vgetmantpd zmm4, zmm3, 0xd7
	vgetmantpd zmm1, zmm3, 0x25
	vgetmantpd xmm8, xmm8, 0x59
	vgetmantpd ymm13, ymm11, 0xd
	vgetmantpd zmm11, zmm15, 0xc1
	vgetmantpd xmm22, xmm18, 0x34
	vgetmantpd ymm16, ymm29, 0x79
	vgetmantpd zmm21, zmm22, 0xb5
	vgetmantpd xmm2{k7}, xmm0, 0x2d
	vgetmantpd ymm4{k1}, yword [0xa56], 0x28
	vgetmantpd zmm2{k1}, zmm0, 0xad
	vgetmantpd xmm3{k5}{z}, xmm2, 0xd4
	vgetmantpd ymm0{k2}{z}, ymm0, 0xca
	vgetmantpd zmm0{k2}{z}, zmm5, 0xc1
	vgetmantpd xmm2, oword [eax+1], 0x63
	vgetmantpd xmm6, oword [eax+64], 0xe1
	vgetmantpd ymm1, yword [eax+1], 0x4f
	vgetmantpd ymm2, yword [eax+64], 0x4c
	vgetmantpd zmm3, zword [eax+1], 0x23
	vgetmantpd zmm6, zword [eax+64], 0x6c
	vgetmantpd xmm4, [0x546], 0x67
	vgetmantpd ymm0, [0x102], 0x88
	vgetmantpd zmm2, [0xdc5], 0xd1
