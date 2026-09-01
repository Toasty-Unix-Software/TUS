default rel
	vandnps xmm1, xmm1, oword [0x91f]
	vandnps xmm1, oword [0x91f]
	vandnps xmm2, xmm0, oword [0x5a0]
	vandnps ymm4, ymm7, ymm2
	vandnps ymm4, ymm2
	vandnps ymm6, ymm2, ymm2
	vandnps xmm5, xmm7, xmm6
	vandnps xmm5, xmm6
	vandnps xmm1, xmm4, xmm6
	vandnps ymm4, ymm2, ymm0
	vandnps ymm4, ymm0
	vandnps ymm0, ymm3, ymm3
	vandnps xmm10, xmm11, xmm13
	vandnps ymm10, ymm12, ymm9
	vandnps xmm13, xmm13, xmm12
	vandnps ymm12, ymm8, ymm11
	vandnps xmm21, xmm29, xmm16
	vandnps ymm31, ymm20, ymm31
	vandnps xmm28, xmm21, xmm17
	vandnps ymm25, ymm30, ymm18
	vandnps xmm5{k6}, xmm1, xmm6
	vandnps ymm5{k7}, ymm0, ymm6
	vandnps zmm5{k4}, zmm2, zword [0xe9a]
	vandnps xmm4{k1}{z}, xmm0, xmm4
	vandnps ymm3{k3}{z}, ymm3, ymm0
	vandnps zmm6{k1}{z}, zmm3, zmm2
	vandnps xmm5, xmm2, oword [eax+1]
	vandnps xmm3, xmm6, oword [eax+64]
	vandnps ymm4, ymm3, yword [eax+1]
	vandnps ymm1, ymm3, yword [eax+64]
	vandnps xmm3, xmm2, oword [eax+1]
	vandnps xmm3, xmm1, oword [eax+64]
	vandnps ymm2, ymm6, yword [eax+1]
	vandnps ymm1, ymm5, yword [eax+64]
	vandnps zmm7, zmm5, zword [eax+1]
	vandnps zmm5, zmm2, zword [eax+64]
	vandnps xmm2, xmm3, [0x7d4]
	vandnps ymm5, ymm3, [0x37f]
	vandnps xmm3, xmm4, [0xdd7]
	vandnps ymm6, ymm6, [0x6d1]
	vandnps zmm1, zmm5, [0xca6]
