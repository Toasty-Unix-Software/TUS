default rel
	vpunpcklqdq xmm0, xmm0, xmm6
	vpunpcklqdq xmm0, xmm6
	vpunpcklqdq xmm6, xmm7, xmm7
	vpunpcklqdq ymm7, ymm4, ymm4
	vpunpcklqdq ymm7, ymm4
	vpunpcklqdq ymm5, ymm6, ymm1
	vpunpcklqdq xmm7, xmm7, xmm2
	vpunpcklqdq xmm7, xmm2
	vpunpcklqdq xmm1, xmm2, xmm0
	vpunpcklqdq ymm1, ymm1, ymm7
	vpunpcklqdq ymm1, ymm7
	vpunpcklqdq ymm5, ymm7, ymm7
	vpunpcklqdq xmm12, xmm10, xmm10
	vpunpcklqdq ymm15, ymm11, ymm9
	vpunpcklqdq xmm14, xmm14, xmm13
	vpunpcklqdq ymm9, ymm11, ymm9
	vpunpcklqdq xmm22, xmm31, xmm20
	vpunpcklqdq ymm24, ymm20, ymm24
	vpunpcklqdq xmm25, xmm17, xmm24
	vpunpcklqdq ymm18, ymm17, ymm20
	vpunpcklqdq xmm6{k4}, xmm7, oword [0x6b8]
	vpunpcklqdq ymm7{k1}, ymm5, ymm4
	vpunpcklqdq zmm7{k2}, zmm1, zmm1
	vpunpcklqdq xmm1{k6}{z}, xmm3, xmm4
	vpunpcklqdq ymm6{k1}{z}, ymm3, yword [0xbde]
	vpunpcklqdq zmm0{k6}{z}, zmm4, zmm4
	vpunpcklqdq xmm3, xmm3, oword [eax+1]
	vpunpcklqdq xmm0, xmm2, oword [eax+64]
	vpunpcklqdq ymm0, ymm7, yword [eax+1]
	vpunpcklqdq ymm1, ymm5, yword [eax+64]
	vpunpcklqdq xmm3, xmm5, oword [eax+1]
	vpunpcklqdq xmm7, xmm7, oword [eax+64]
	vpunpcklqdq ymm0, ymm6, yword [eax+1]
	vpunpcklqdq ymm0, ymm6, yword [eax+64]
	vpunpcklqdq zmm0, zmm1, zword [eax+1]
	vpunpcklqdq zmm1, zmm0, zword [eax+64]
	vpunpcklqdq xmm3, xmm7, [0x7a2]
	vpunpcklqdq ymm4, ymm2, [0x4a2]
	vpunpcklqdq xmm7, xmm5, [0x488]
	vpunpcklqdq ymm1, ymm7, [0xaf9]
	vpunpcklqdq zmm5, zmm1, [0xc60]
