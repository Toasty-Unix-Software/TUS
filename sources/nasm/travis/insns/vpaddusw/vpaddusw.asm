default rel
	vpaddusw xmm6, xmm2, oword [0x490]
	vpaddusw xmm6, oword [0x490]
	vpaddusw xmm0, xmm3, xmm0
	vpaddusw ymm7, ymm6, ymm7
	vpaddusw ymm7, ymm7
	vpaddusw ymm3, ymm5, yword [0x961]
	vpaddusw xmm3, xmm4, oword [0xfac]
	vpaddusw xmm3, oword [0xfac]
	vpaddusw xmm4, xmm3, oword [0x89c]
	vpaddusw ymm0, ymm6, yword [0x7cc]
	vpaddusw ymm0, yword [0x7cc]
	vpaddusw ymm6, ymm7, ymm4
	vpaddusw xmm15, xmm14, xmm12
	vpaddusw ymm12, ymm9, ymm9
	vpaddusw xmm15, xmm12, xmm15
	vpaddusw ymm15, ymm10, ymm8
	vpaddusw xmm29, xmm19, xmm17
	vpaddusw ymm30, ymm19, ymm31
	vpaddusw xmm21, xmm26, xmm24
	vpaddusw ymm28, ymm25, ymm26
	vpaddusw xmm5{k3}, xmm7, xmm6
	vpaddusw ymm1{k3}, ymm6, yword [0xe60]
	vpaddusw zmm5{k3}, zmm6, zword [0x552]
	vpaddusw xmm4{k4}{z}, xmm0, oword [0x7a4]
	vpaddusw ymm2{k7}{z}, ymm6, yword [0x9db]
	vpaddusw zmm7{k7}{z}, zmm3, zmm0
	vpaddusw xmm1, xmm0, oword [eax+1]
	vpaddusw xmm3, xmm6, oword [eax+64]
	vpaddusw ymm0, ymm2, yword [eax+1]
	vpaddusw ymm6, ymm6, yword [eax+64]
	vpaddusw xmm5, xmm7, oword [eax+1]
	vpaddusw xmm6, xmm1, oword [eax+64]
	vpaddusw ymm3, ymm3, yword [eax+1]
	vpaddusw ymm2, ymm3, yword [eax+64]
	vpaddusw zmm4, zmm5, zword [eax+1]
	vpaddusw zmm1, zmm1, zword [eax+64]
	vpaddusw xmm1, xmm0, [0xa02]
	vpaddusw ymm2, ymm7, [0x529]
	vpaddusw xmm7, xmm7, [0x2d5]
	vpaddusw ymm2, ymm6, [0xdbf]
	vpaddusw zmm5, zmm2, [0x2b3]
