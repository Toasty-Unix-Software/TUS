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

%ifdef ERROR
	vgf2p8affineinvqb xmm13, xmm15, xmm14, 0xec
	vgf2p8affineinvqb ymm8, ymm11, ymm13, 0x5d
	vgf2p8affineinvqb xmm8, xmm13, xmm10, 0x30
	vgf2p8affineinvqb ymm14, ymm12, ymm15, 0x5d
	vgf2p8affineinvqb xmm28, xmm17, xmm27, 0x8
	vgf2p8affineinvqb ymm23, ymm16, ymm23, 0xc3
	vgf2p8affineinvqb xmm30, xmm18, xmm21, 0x53
	vgf2p8affineinvqb ymm29, ymm18, ymm24, 0xa5
%endif
