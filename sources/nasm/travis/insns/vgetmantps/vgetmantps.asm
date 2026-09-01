default rel
	vgetmantps xmm0, xmm1, 0x7d
	vgetmantps xmm2, xmm1, 0x5b
	vgetmantps ymm4, ymm7, 0x36
	vgetmantps ymm4, yword [0xbbe], 0xad
	vgetmantps zmm2, zmm3, 0x13
	vgetmantps zmm6, zmm5, 0x32
	vgetmantps xmm10, xmm15, 0xe0
	vgetmantps ymm13, ymm12, 0xa9
	vgetmantps zmm10, zmm8, 0xe5
	vgetmantps xmm30, xmm22, 0xf5
	vgetmantps ymm23, ymm18, 0x85
	vgetmantps zmm28, zmm30, 0x89
	vgetmantps xmm5{k3}, xmm6, 0xdb
	vgetmantps ymm1{k3}, ymm1, 0x3b
	vgetmantps zmm2{k6}, zword [0xb4b], 0xee
	vgetmantps xmm2{k2}{z}, oword [0xb7a], 0xed
	vgetmantps ymm3{k2}{z}, ymm1, 0xb5
	vgetmantps zmm2{k7}{z}, zmm0, 0xda
	vgetmantps xmm3, oword [eax+1], 0x73
	vgetmantps xmm3, oword [eax+64], 0x20
	vgetmantps ymm5, yword [eax+1], 0x25
	vgetmantps ymm4, yword [eax+64], 0x73
	vgetmantps zmm0, zword [eax+1], 0x3c
	vgetmantps zmm7, zword [eax+64], 0x9a
	vgetmantps xmm7, [0x20c], 0xd9
	vgetmantps ymm0, [0x74d], 0xf9
	vgetmantps zmm5, [0x8d0], 0xae
