default rel
	vunpckhps xmm2, xmm5, oword [0xd20]
	vunpckhps xmm2, oword [0xd20]
	vunpckhps xmm3, xmm1, oword [0xa36]
	vunpckhps ymm7, ymm0, yword [0x898]
	vunpckhps ymm7, yword [0x898]
	vunpckhps ymm3, ymm6, ymm0
	vunpckhps xmm7, xmm3, xmm4
	vunpckhps xmm7, xmm4
	vunpckhps xmm2, xmm5, oword [0xbd3]
	vunpckhps ymm5, ymm1, ymm4
	vunpckhps ymm5, ymm4
	vunpckhps ymm5, ymm3, ymm2
	vunpckhps xmm15, xmm14, xmm14
	vunpckhps ymm11, ymm14, ymm12
	vunpckhps xmm13, xmm15, xmm11
	vunpckhps ymm12, ymm11, ymm12
	vunpckhps xmm21, xmm21, xmm17
	vunpckhps ymm22, ymm24, ymm21
	vunpckhps xmm25, xmm26, xmm20
	vunpckhps ymm29, ymm28, ymm27
	vunpckhps xmm1{k5}, xmm6, xmm2
	vunpckhps ymm7{k3}, ymm2, ymm6
	vunpckhps zmm6{k2}, zmm0, zmm4
	vunpckhps xmm0{k6}{z}, xmm2, xmm7
	vunpckhps ymm1{k1}{z}, ymm4, yword [0x155]
	vunpckhps zmm2{k1}{z}, zmm5, zmm0
	vunpckhps xmm4, xmm0, oword [eax+1]
	vunpckhps xmm5, xmm3, oword [eax+64]
	vunpckhps ymm5, ymm7, yword [eax+1]
	vunpckhps ymm1, ymm3, yword [eax+64]
	vunpckhps xmm5, xmm6, oword [eax+1]
	vunpckhps xmm7, xmm3, oword [eax+64]
	vunpckhps ymm2, ymm5, yword [eax+1]
	vunpckhps ymm1, ymm6, yword [eax+64]
	vunpckhps zmm7, zmm2, zword [eax+1]
	vunpckhps zmm0, zmm7, zword [eax+64]
	vunpckhps xmm4, xmm0, [0x6e6]
	vunpckhps ymm7, ymm3, [0x539]
	vunpckhps xmm2, xmm7, [0x8db]
	vunpckhps ymm2, ymm1, [0xb2c]
	vunpckhps zmm5, zmm5, [0x4be]
