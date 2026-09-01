default rel
	vandps xmm3, xmm4, xmm2
	vandps xmm3, xmm2
	vandps xmm3, xmm0, oword [0x75f]
	vandps ymm5, ymm3, ymm5
	vandps ymm5, ymm5
	vandps ymm4, ymm0, ymm1
	vandps xmm4, xmm6, xmm6
	vandps xmm4, xmm6
	vandps xmm2, xmm6, xmm7
	vandps ymm2, ymm5, yword [0xd78]
	vandps ymm2, yword [0xd78]
	vandps ymm3, ymm3, ymm2
	vandps xmm14, xmm11, xmm15
	vandps ymm10, ymm9, ymm12
	vandps xmm9, xmm15, xmm14
	vandps ymm9, ymm11, ymm11
	vandps xmm31, xmm16, xmm24
	vandps ymm27, ymm28, ymm24
	vandps xmm17, xmm28, xmm26
	vandps ymm19, ymm29, ymm20
	vandps xmm2{k2}, xmm2, xmm3
	vandps ymm0{k2}, ymm4, ymm0
	vandps zmm3{k5}, zmm6, zmm0
	vandps xmm3{k3}{z}, xmm3, oword [0xdaf]
	vandps ymm2{k4}{z}, ymm3, yword [0x54a]
	vandps zmm3{k6}{z}, zmm3, zmm3
	vandps xmm5, xmm4, oword [eax+1]
	vandps xmm7, xmm3, oword [eax+64]
	vandps ymm7, ymm0, yword [eax+1]
	vandps ymm1, ymm1, yword [eax+64]
	vandps xmm5, xmm5, oword [eax+1]
	vandps xmm3, xmm2, oword [eax+64]
	vandps ymm7, ymm2, yword [eax+1]
	vandps ymm6, ymm5, yword [eax+64]
	vandps zmm4, zmm2, zword [eax+1]
	vandps zmm1, zmm1, zword [eax+64]
	vandps xmm6, xmm4, [0x700]
	vandps ymm5, ymm4, [0xc56]
	vandps xmm0, xmm0, [0x76b]
	vandps ymm0, ymm2, [0xa13]
	vandps zmm2, zmm6, [0x704]
