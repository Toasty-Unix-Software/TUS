default rel
	vpclmullqlqdq xmm7, xmm3, xmm1
	vpclmullqlqdq xmm7, xmm1
	vpclmullqlqdq xmm6, xmm2, xmm3
	vpclmullqlqdq ymm6, ymm1, ymm3
	vpclmullqlqdq ymm6, ymm3
	vpclmullqlqdq ymm6, ymm5, ymm4
	vpclmullqlqdq zmm7, zmm0, zmm2
	vpclmullqlqdq zmm7, zmm2
	vpclmullqlqdq zmm0, zmm3, zmm0
	vpclmullqlqdq xmm9, xmm11, xmm9
	vpclmullqlqdq ymm14, ymm13, ymm11
	vpclmullqlqdq zmm9, zmm14, zmm15
	vpclmullqlqdq xmm21, xmm25, xmm29
	vpclmullqlqdq ymm26, ymm29, ymm30
	vpclmullqlqdq zmm31, zmm16, zmm19
	vpclmullqlqdq xmm1, xmm5, oword [eax+1]
	vpclmullqlqdq xmm4, xmm3, oword [eax+64]
	vpclmullqlqdq ymm7, ymm3, yword [eax+1]
	vpclmullqlqdq ymm7, ymm6, yword [eax+64]
	vpclmullqlqdq zmm3, zmm3, zword [eax+1]
	vpclmullqlqdq zmm4, zmm7, zword [eax+64]
	vpclmullqlqdq xmm3, xmm1, [0xfad]
	vpclmullqlqdq ymm3, ymm4, [0x564]
	vpclmullqlqdq zmm6, zmm1, [0x71e]
