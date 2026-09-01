default rel
	vgf2p8affineinvqb xmm3, xmm6, xmm7, 0xe0
	vgf2p8affineinvqb xmm3, xmm7, 0xe0
	vgf2p8affineinvqb xmm6, xmm1, xmm1, 0x23
	vgf2p8affineinvqb ymm3, ymm4, ymm3, 0xd3
	vgf2p8affineinvqb ymm3, ymm3, 0xd3
	vgf2p8affineinvqb ymm0, ymm7, ymm3, 0xc4
	vgf2p8affineinvqb xmm5, xmm4, xmm3, 0xc0
	vgf2p8affineinvqb xmm5, xmm3, 0xc0
	vgf2p8affineinvqb xmm2, xmm0, oword [0x8ae], 0xf5
	vgf2p8affineinvqb ymm5, ymm2, yword [0x828], 0x57
	vgf2p8affineinvqb ymm5, yword [0x828], 0x57
	vgf2p8affineinvqb ymm6, ymm5, ymm0, 0x5
	vgf2p8affineinvqb xmm13, xmm15, xmm14, 0xec
	vgf2p8affineinvqb ymm8, ymm11, ymm13, 0x5d
	vgf2p8affineinvqb xmm8, xmm13, xmm10, 0x30
	vgf2p8affineinvqb ymm14, ymm12, ymm15, 0x5d
	vgf2p8affineinvqb xmm28, xmm17, xmm27, 0x8
	vgf2p8affineinvqb ymm23, ymm16, ymm23, 0xc3
	vgf2p8affineinvqb xmm30, xmm18, xmm21, 0x53
	vgf2p8affineinvqb ymm29, ymm18, ymm24, 0xa5
	vgf2p8affineinvqb xmm1{k7}, xmm5, oword [0x4f4], 0xf7
	vgf2p8affineinvqb ymm1{k2}, ymm2, yword [0x2ed], 0x39
	vgf2p8affineinvqb zmm6{k2}, zmm6, zmm2, 0x14
	vgf2p8affineinvqb xmm2{k4}{z}, xmm0, xmm0, 0xe2
	vgf2p8affineinvqb ymm7{k2}{z}, ymm7, ymm6, 0xd6
	vgf2p8affineinvqb zmm5{k4}{z}, zmm4, zmm3, 0x1f
	vgf2p8affineinvqb xmm5, xmm4, oword [eax+1], 0xcd
	vgf2p8affineinvqb xmm0, xmm7, oword [eax+64], 0x3e
	vgf2p8affineinvqb ymm1, ymm7, yword [eax+1], 0x6f
	vgf2p8affineinvqb ymm0, ymm3, yword [eax+64], 0x2
	vgf2p8affineinvqb xmm7, xmm2, oword [eax+1], 0x6e
	vgf2p8affineinvqb xmm0, xmm2, oword [eax+64], 0x28
	vgf2p8affineinvqb ymm5, ymm2, yword [eax+1], 0x4b
	vgf2p8affineinvqb ymm1, ymm5, yword [eax+64], 0xa2
	vgf2p8affineinvqb zmm0, zmm4, zword [eax+1], 0xc2
	vgf2p8affineinvqb zmm2, zmm6, zword [eax+64], 0xda
	vgf2p8affineinvqb xmm4, xmm1, [0x654], 0x7b
	vgf2p8affineinvqb ymm7, ymm0, [0x8db], 0x1a
	vgf2p8affineinvqb xmm0, xmm5, [0x305], 0x19
	vgf2p8affineinvqb ymm0, ymm6, [0xbd4], 0x55
	vgf2p8affineinvqb zmm3, zmm1, [0xc42], 0x53
