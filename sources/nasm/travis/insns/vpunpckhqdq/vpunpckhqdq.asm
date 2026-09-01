default rel
	vpunpckhqdq xmm5, xmm7, xmm2
	vpunpckhqdq xmm5, xmm2
	vpunpckhqdq xmm5, xmm0, xmm6
	vpunpckhqdq ymm0, ymm6, yword [0x6ad]
	vpunpckhqdq ymm0, yword [0x6ad]
	vpunpckhqdq ymm3, ymm2, yword [0x40b]
	vpunpckhqdq xmm0, xmm7, xmm4
	vpunpckhqdq xmm0, xmm4
	vpunpckhqdq xmm3, xmm7, xmm2
	vpunpckhqdq ymm5, ymm5, yword [0x292]
	vpunpckhqdq ymm5, yword [0x292]
	vpunpckhqdq ymm5, ymm3, ymm0
	vpunpckhqdq xmm8, xmm10, xmm13
	vpunpckhqdq ymm10, ymm8, ymm14
	vpunpckhqdq xmm10, xmm14, xmm10
	vpunpckhqdq ymm13, ymm15, ymm15
	vpunpckhqdq xmm17, xmm16, xmm23
	vpunpckhqdq ymm17, ymm19, ymm23
	vpunpckhqdq xmm26, xmm21, xmm17
	vpunpckhqdq ymm31, ymm31, ymm21
	vpunpckhqdq xmm1{k1}, xmm3, oword [0x6f6]
	vpunpckhqdq ymm4{k2}, ymm2, ymm1
	vpunpckhqdq zmm1{k6}, zmm5, zword [0xb02]
	vpunpckhqdq xmm5{k4}{z}, xmm5, xmm1
	vpunpckhqdq ymm4{k6}{z}, ymm6, ymm6
	vpunpckhqdq zmm1{k6}{z}, zmm6, zword [0x243]
	vpunpckhqdq xmm3, xmm0, oword [eax+1]
	vpunpckhqdq xmm5, xmm3, oword [eax+64]
	vpunpckhqdq ymm0, ymm0, yword [eax+1]
	vpunpckhqdq ymm3, ymm2, yword [eax+64]
	vpunpckhqdq xmm2, xmm5, oword [eax+1]
	vpunpckhqdq xmm2, xmm1, oword [eax+64]
	vpunpckhqdq ymm5, ymm1, yword [eax+1]
	vpunpckhqdq ymm5, ymm4, yword [eax+64]
	vpunpckhqdq zmm7, zmm6, zword [eax+1]
	vpunpckhqdq zmm7, zmm2, zword [eax+64]
	vpunpckhqdq xmm3, xmm0, [0x108]
	vpunpckhqdq ymm0, ymm0, [0xc53]
	vpunpckhqdq xmm7, xmm4, [0x48b]
	vpunpckhqdq ymm5, ymm5, [0xe29]
	vpunpckhqdq zmm4, zmm5, [0xdfd]
