default rel
	vpxord xmm5, xmm1, xmm4
	vpxord xmm5, xmm4
	vpxord xmm5, xmm1, oword [0x8b6]
	vpxord ymm6, ymm4, yword [0x97e]
	vpxord ymm6, yword [0x97e]
	vpxord ymm5, ymm2, yword [0xeb5]
	vpxord zmm3, zmm6, zmm4
	vpxord zmm3, zmm4
	vpxord zmm0, zmm0, zmm4
	vpxord xmm9, xmm9, xmm10
	vpxord ymm9, ymm11, ymm14
	vpxord zmm15, zmm12, zmm8
	vpxord xmm23, xmm18, xmm16
	vpxord ymm20, ymm25, ymm20
	vpxord zmm19, zmm19, zmm27
	vpxord xmm1{k4}, xmm6, xmm1
	vpxord ymm3{k2}, ymm1, yword [0x76e]
	vpxord zmm5{k3}, zmm6, zword [0xca4]
	vpxord xmm5{k5}{z}, xmm7, xmm0
	vpxord ymm1{k7}{z}, ymm0, ymm3
	vpxord zmm1{k4}{z}, zmm5, zmm6
	vpxord xmm2, xmm0, oword [eax+1]
	vpxord xmm7, xmm0, oword [eax+64]
	vpxord ymm6, ymm5, yword [eax+1]
	vpxord ymm3, ymm6, yword [eax+64]
	vpxord zmm4, zmm7, zword [eax+1]
	vpxord zmm5, zmm4, zword [eax+64]
	vpxord xmm5, xmm1, [0x82f]
	vpxord ymm6, ymm6, [0xdf1]
	vpxord zmm6, zmm0, [0x61f]
