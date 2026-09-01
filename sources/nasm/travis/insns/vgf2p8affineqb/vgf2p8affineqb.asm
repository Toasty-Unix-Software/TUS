default rel
	vgf2p8affineqb xmm7, xmm6, xmm4, 0x90
	vgf2p8affineqb xmm7, xmm4, 0x90
	vgf2p8affineqb xmm5, xmm4, xmm3, 0x8a
	vgf2p8affineqb ymm7, ymm3, yword [0xce4], 0x59
	vgf2p8affineqb ymm7, yword [0xce4], 0x59
	vgf2p8affineqb ymm7, ymm3, ymm5, 0x20
	vgf2p8affineqb xmm6, xmm3, xmm6, 0x19
	vgf2p8affineqb xmm6, xmm6, 0x19
	vgf2p8affineqb xmm1, xmm5, oword [0x441], 0x2f
	vgf2p8affineqb ymm4, ymm2, yword [0x9e2], 0x20
	vgf2p8affineqb ymm4, yword [0x9e2], 0x20
	vgf2p8affineqb ymm0, ymm2, ymm7, 0x54
	vgf2p8affineqb xmm13, xmm9, xmm14, 0xea
	vgf2p8affineqb ymm14, ymm14, ymm12, 0x62
	vgf2p8affineqb xmm12, xmm12, xmm12, 0x7c
	vgf2p8affineqb ymm15, ymm11, ymm15, 0xea
	vgf2p8affineqb xmm20, xmm22, xmm18, 0x93
	vgf2p8affineqb ymm27, ymm29, ymm31, 0xd9
	vgf2p8affineqb xmm26, xmm21, xmm24, 0x3f
	vgf2p8affineqb ymm18, ymm30, ymm27, 0x39
	vgf2p8affineqb xmm0{k6}, xmm6, xmm5, 0x2b
	vgf2p8affineqb ymm1{k3}, ymm6, ymm6, 0x78
	vgf2p8affineqb zmm0{k1}, zmm1, zword [0xfde], 0x20
	vgf2p8affineqb xmm1{k4}{z}, xmm2, xmm0, 0xe3
	vgf2p8affineqb ymm6{k3}{z}, ymm2, yword [0x905], 0xb5
	vgf2p8affineqb zmm6{k6}{z}, zmm6, zmm7, 0xf1
	vgf2p8affineqb xmm7, xmm6, oword [eax+1], 0x96
	vgf2p8affineqb xmm0, xmm7, oword [eax+64], 0xdd
	vgf2p8affineqb ymm2, ymm7, yword [eax+1], 0xba
	vgf2p8affineqb ymm0, ymm2, yword [eax+64], 0xe2
	vgf2p8affineqb xmm4, xmm6, oword [eax+1], 0x15
	vgf2p8affineqb xmm5, xmm6, oword [eax+64], 0x30
	vgf2p8affineqb ymm5, ymm5, yword [eax+1], 0xb9
	vgf2p8affineqb ymm6, ymm6, yword [eax+64], 0xd
	vgf2p8affineqb zmm6, zmm6, zword [eax+1], 0x7b
	vgf2p8affineqb zmm3, zmm0, zword [eax+64], 0x75
	vgf2p8affineqb xmm5, xmm3, [0x50e], 0xa6
	vgf2p8affineqb ymm1, ymm3, [0x70a], 0x8b
	vgf2p8affineqb xmm0, xmm7, [0xf53], 0x3
	vgf2p8affineqb ymm7, ymm1, [0xae5], 0xdb
	vgf2p8affineqb zmm0, zmm5, [0x249], 0xc4
