default rel
	vgf2p8mulb xmm5, xmm1, xmm5
	vgf2p8mulb xmm5, xmm5
	vgf2p8mulb xmm6, xmm4, xmm2
	vgf2p8mulb ymm1, ymm5, yword [0x4e4]
	vgf2p8mulb ymm1, yword [0x4e4]
	vgf2p8mulb ymm2, ymm3, ymm4
	vgf2p8mulb xmm0, xmm4, xmm3
	vgf2p8mulb xmm0, xmm3
	vgf2p8mulb xmm3, xmm6, xmm5
	vgf2p8mulb ymm6, ymm0, ymm7
	vgf2p8mulb ymm6, ymm7
	vgf2p8mulb ymm5, ymm7, ymm1
	vgf2p8mulb xmm14, xmm9, xmm9
	vgf2p8mulb ymm13, ymm14, ymm9
	vgf2p8mulb xmm12, xmm13, xmm12
	vgf2p8mulb ymm14, ymm11, ymm9
	vgf2p8mulb xmm29, xmm24, xmm21
	vgf2p8mulb ymm29, ymm20, ymm20
	vgf2p8mulb xmm29, xmm28, xmm29
	vgf2p8mulb ymm21, ymm18, ymm18
	vgf2p8mulb xmm5{k3}, xmm1, xmm0
	vgf2p8mulb ymm7{k3}, ymm3, ymm6
	vgf2p8mulb zmm4{k5}, zmm3, zmm5
	vgf2p8mulb xmm4{k7}{z}, xmm1, xmm3
	vgf2p8mulb ymm6{k2}{z}, ymm0, ymm4
	vgf2p8mulb zmm1{k3}{z}, zmm0, zmm2
	vgf2p8mulb xmm1, xmm2, oword [eax+1]
	vgf2p8mulb xmm2, xmm1, oword [eax+64]
	vgf2p8mulb ymm3, ymm4, yword [eax+1]
	vgf2p8mulb ymm5, ymm5, yword [eax+64]
	vgf2p8mulb xmm0, xmm3, oword [eax+1]
	vgf2p8mulb xmm0, xmm0, oword [eax+64]
	vgf2p8mulb ymm5, ymm1, yword [eax+1]
	vgf2p8mulb ymm4, ymm6, yword [eax+64]
	vgf2p8mulb zmm7, zmm5, zword [eax+1]
	vgf2p8mulb zmm2, zmm5, zword [eax+64]
	vgf2p8mulb xmm6, xmm5, [0x6c9]
	vgf2p8mulb ymm3, ymm3, [0x6b6]
	vgf2p8mulb xmm2, xmm4, [0xb7c]
	vgf2p8mulb ymm4, ymm2, [0xf12]
	vgf2p8mulb zmm4, zmm1, [0xc45]
