default rel
	vminps xmm5, xmm3, oword [0x24a]
	vminps xmm5, oword [0x24a]
	vminps xmm0, xmm5, xmm5
	vminps ymm0, ymm2, yword [0xe71]
	vminps ymm0, yword [0xe71]
	vminps ymm4, ymm6, yword [0xe25]
	vminps xmm6, xmm1, xmm6
	vminps xmm6, xmm6
	vminps xmm6, xmm6, xmm7
	vminps ymm2, ymm4, yword [0x1fb]
	vminps ymm2, yword [0x1fb]
	vminps ymm0, ymm5, ymm5
	vminps xmm10, xmm12, xmm15
	vminps ymm15, ymm10, ymm8
	vminps xmm13, xmm11, xmm11
	vminps ymm13, ymm14, ymm8
	vminps xmm20, xmm21, xmm22
	vminps ymm25, ymm29, ymm19
	vminps xmm16, xmm23, xmm18
	vminps ymm26, ymm17, ymm22
	vminps xmm0{k1}, xmm7, xmm6
	vminps ymm5{k7}, ymm3, ymm1
	vminps zmm5{k2}, zmm5, zword [0x2e2]
	vminps xmm6{k3}{z}, xmm5, xmm7
	vminps ymm2{k2}{z}, ymm0, yword [0x4cf]
	vminps zmm7{k7}{z}, zmm5, zmm6
	vminps zmm2, zmm3, zmm3, {sae}
	vminps xmm2, xmm5, oword [eax+1]
	vminps xmm2, xmm7, oword [eax+64]
	vminps ymm6, ymm1, yword [eax+1]
	vminps ymm7, ymm6, yword [eax+64]
	vminps xmm2, xmm3, oword [eax+1]
	vminps xmm1, xmm7, oword [eax+64]
	vminps ymm1, ymm7, yword [eax+1]
	vminps ymm2, ymm6, yword [eax+64]
	vminps zmm3, zmm4, zword [eax+1]
	vminps zmm0, zmm1, zword [eax+64]
	vminps xmm3, xmm2, [0x345]
	vminps ymm5, ymm1, [0x444]
	vminps xmm5, xmm6, [0x4ff]
	vminps ymm7, ymm5, [0x1c3]
	vminps zmm6, zmm6, [0x9c5]
