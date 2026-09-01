default rel
	vunpcklps xmm3, xmm2, xmm6
	vunpcklps xmm3, xmm6
	vunpcklps xmm5, xmm6, xmm4
	vunpcklps ymm0, ymm5, yword [0x5f3]
	vunpcklps ymm0, yword [0x5f3]
	vunpcklps ymm1, ymm4, ymm5
	vunpcklps xmm3, xmm5, xmm3
	vunpcklps xmm3, xmm3
	vunpcklps xmm1, xmm5, xmm5
	vunpcklps ymm3, ymm5, yword [0x3fc]
	vunpcklps ymm3, yword [0x3fc]
	vunpcklps ymm7, ymm3, yword [0xe8c]
	vunpcklps xmm12, xmm15, xmm11
	vunpcklps ymm12, ymm9, ymm9
	vunpcklps xmm12, xmm13, xmm11
	vunpcklps ymm13, ymm14, ymm11
	vunpcklps xmm26, xmm19, xmm16
	vunpcklps ymm30, ymm31, ymm22
	vunpcklps xmm16, xmm28, xmm18
	vunpcklps ymm21, ymm21, ymm21
	vunpcklps xmm2{k7}, xmm5, oword [0xfff]
	vunpcklps ymm3{k5}, ymm2, yword [0x8e9]
	vunpcklps zmm6{k3}, zmm4, zmm4
	vunpcklps xmm3{k2}{z}, xmm7, xmm3
	vunpcklps ymm3{k6}{z}, ymm2, yword [0xf0d]
	vunpcklps zmm2{k2}{z}, zmm1, zmm1
	vunpcklps xmm3, xmm3, oword [eax+1]
	vunpcklps xmm4, xmm3, oword [eax+64]
	vunpcklps ymm7, ymm5, yword [eax+1]
	vunpcklps ymm1, ymm0, yword [eax+64]
	vunpcklps xmm4, xmm0, oword [eax+1]
	vunpcklps xmm0, xmm0, oword [eax+64]
	vunpcklps ymm2, ymm4, yword [eax+1]
	vunpcklps ymm5, ymm6, yword [eax+64]
	vunpcklps zmm0, zmm2, zword [eax+1]
	vunpcklps zmm1, zmm7, zword [eax+64]
	vunpcklps xmm5, xmm7, [0x937]
	vunpcklps ymm5, ymm1, [0x6ba]
	vunpcklps xmm6, xmm1, [0x54c]
	vunpcklps ymm7, ymm7, [0x9d8]
	vunpcklps zmm7, zmm5, [0xce9]
