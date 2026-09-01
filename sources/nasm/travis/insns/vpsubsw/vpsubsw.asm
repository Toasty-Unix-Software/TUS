default rel
	vpsubsw xmm1, xmm5, xmm5
	vpsubsw xmm1, xmm5
	vpsubsw xmm4, xmm0, xmm5
	vpsubsw ymm3, ymm1, yword [0x14a]
	vpsubsw ymm3, yword [0x14a]
	vpsubsw ymm3, ymm7, ymm7
	vpsubsw xmm7, xmm0, xmm5
	vpsubsw xmm7, xmm5
	vpsubsw xmm5, xmm6, xmm4
	vpsubsw ymm6, ymm4, ymm6
	vpsubsw ymm6, ymm6
	vpsubsw ymm1, ymm1, ymm7
	vpsubsw xmm10, xmm8, xmm13
	vpsubsw ymm8, ymm14, ymm9
	vpsubsw xmm10, xmm8, xmm11
	vpsubsw ymm10, ymm8, ymm10
	vpsubsw xmm25, xmm23, xmm25
	vpsubsw ymm31, ymm22, ymm16
	vpsubsw xmm22, xmm27, xmm23
	vpsubsw ymm24, ymm16, ymm25
	vpsubsw xmm5{k4}, xmm0, xmm2
	vpsubsw ymm3{k2}, ymm4, ymm1
	vpsubsw zmm5{k3}, zmm5, zmm6
	vpsubsw xmm6{k1}{z}, xmm2, xmm7
	vpsubsw ymm6{k7}{z}, ymm0, yword [0x46e]
	vpsubsw zmm0{k7}{z}, zmm2, zmm4
	vpsubsw xmm5, xmm7, oword [eax+1]
	vpsubsw xmm4, xmm4, oword [eax+64]
	vpsubsw ymm4, ymm3, yword [eax+1]
	vpsubsw ymm2, ymm2, yword [eax+64]
	vpsubsw xmm7, xmm6, oword [eax+1]
	vpsubsw xmm4, xmm2, oword [eax+64]
	vpsubsw ymm2, ymm4, yword [eax+1]
	vpsubsw ymm4, ymm3, yword [eax+64]
	vpsubsw zmm4, zmm5, zword [eax+1]
	vpsubsw zmm5, zmm3, zword [eax+64]
	vpsubsw xmm3, xmm1, [0xc96]
	vpsubsw ymm5, ymm5, [0xa80]
	vpsubsw xmm5, xmm7, [0x29e]
	vpsubsw ymm5, ymm1, [0x74f]
	vpsubsw zmm7, zmm4, [0x5f0]
