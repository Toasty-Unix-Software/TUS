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

%ifdef ERROR
	vgf2p8affineqb xmm13, xmm9, xmm14, 0xea
	vgf2p8affineqb ymm14, ymm14, ymm12, 0x62
	vgf2p8affineqb xmm12, xmm12, xmm12, 0x7c
	vgf2p8affineqb ymm15, ymm11, ymm15, 0xea
	vgf2p8affineqb xmm20, xmm22, xmm18, 0x93
	vgf2p8affineqb ymm27, ymm29, ymm31, 0xd9
	vgf2p8affineqb xmm26, xmm21, xmm24, 0x3f
	vgf2p8affineqb ymm18, ymm30, ymm27, 0x39
%endif
