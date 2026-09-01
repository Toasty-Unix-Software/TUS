default rel
	vmulps xmm5, xmm2, xmm4
	vmulps xmm5, xmm4
	vmulps xmm2, xmm6, oword [0x234]
	vmulps ymm2, ymm1, ymm2
	vmulps ymm2, ymm2
	vmulps ymm4, ymm7, ymm6
	vmulps xmm0, xmm2, xmm0
	vmulps xmm0, xmm0
	vmulps xmm4, xmm1, xmm6
	vmulps ymm3, ymm5, ymm3
	vmulps ymm3, ymm3
	vmulps ymm3, ymm0, ymm2
	vmulps xmm15, xmm13, xmm10
	vmulps ymm13, ymm14, ymm15
	vmulps xmm12, xmm10, xmm9
	vmulps ymm11, ymm10, ymm9
	vmulps xmm21, xmm29, xmm31
	vmulps ymm18, ymm16, ymm21
	vmulps xmm19, xmm28, xmm20
	vmulps ymm31, ymm18, ymm31
	vmulps xmm7{k5}, xmm3, xmm5
	vmulps ymm4{k1}, ymm3, ymm3
	vmulps zmm6{k3}, zmm7, zmm4
	vmulps xmm0{k7}{z}, xmm7, xmm1
	vmulps ymm0{k7}{z}, ymm3, ymm6
	vmulps zmm0{k3}{z}, zmm0, zmm0
	vmulps zmm1, zmm0, zmm5, {rd-sae}
	vmulps xmm4, xmm4, oword [eax+1]
	vmulps xmm3, xmm7, oword [eax+64]
	vmulps ymm1, ymm4, yword [eax+1]
	vmulps ymm4, ymm2, yword [eax+64]
	vmulps xmm0, xmm5, oword [eax+1]
	vmulps xmm0, xmm4, oword [eax+64]
	vmulps ymm1, ymm3, yword [eax+1]
	vmulps ymm5, ymm5, yword [eax+64]
	vmulps zmm6, zmm6, zword [eax+1]
	vmulps zmm0, zmm6, zword [eax+64]
	vmulps xmm3, xmm7, [0x88d]
	vmulps ymm7, ymm4, [0xce3]
	vmulps xmm0, xmm0, [0x3d9]
	vmulps ymm6, ymm7, [0x92d]
	vmulps zmm1, zmm3, [0xccb]
