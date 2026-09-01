default rel
	vorpd xmm5, xmm1, oword [0x973]
	vorpd xmm5, oword [0x973]
	vorpd xmm6, xmm7, xmm2
	vorpd ymm5, ymm0, yword [0x4e0]
	vorpd ymm5, yword [0x4e0]
	vorpd ymm1, ymm4, ymm2
	vorpd xmm5, xmm4, oword [0xd0a]
	vorpd xmm5, oword [0xd0a]
	vorpd xmm6, xmm4, xmm1
	vorpd ymm1, ymm5, ymm3
	vorpd ymm1, ymm3
	vorpd ymm2, ymm7, ymm7
	vorpd xmm8, xmm10, xmm12
	vorpd ymm11, ymm15, ymm10
	vorpd xmm10, xmm9, xmm10
	vorpd ymm14, ymm8, ymm15
	vorpd xmm23, xmm20, xmm30
	vorpd ymm26, ymm18, ymm25
	vorpd xmm22, xmm26, xmm19
	vorpd ymm26, ymm28, ymm17
	vorpd xmm5{k5}, xmm0, xmm2
	vorpd ymm2{k5}, ymm1, ymm7
	vorpd zmm7{k4}, zmm7, zmm0
	vorpd xmm5{k2}{z}, xmm7, xmm3
	vorpd ymm7{k5}{z}, ymm3, yword [0xc45]
	vorpd zmm2{k1}{z}, zmm7, zword [0x7ae]
	vorpd xmm2, xmm6, oword [eax+1]
	vorpd xmm7, xmm2, oword [eax+64]
	vorpd ymm1, ymm6, yword [eax+1]
	vorpd ymm0, ymm5, yword [eax+64]
	vorpd xmm2, xmm7, oword [eax+1]
	vorpd xmm5, xmm7, oword [eax+64]
	vorpd ymm6, ymm0, yword [eax+1]
	vorpd ymm4, ymm0, yword [eax+64]
	vorpd zmm2, zmm3, zword [eax+1]
	vorpd zmm3, zmm4, zword [eax+64]
	vorpd xmm3, xmm7, [0xce9]
	vorpd ymm2, ymm7, [0x391]
	vorpd xmm2, xmm0, [0x7b3]
	vorpd ymm5, ymm0, [0x950]
	vorpd zmm1, zmm1, [0xed9]
