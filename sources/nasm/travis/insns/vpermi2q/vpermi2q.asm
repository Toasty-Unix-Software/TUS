default rel
	vpermi2q xmm2, xmm5, xmm2
	vpermi2q xmm5, xmm6, oword [0x4eb]
	vpermi2q ymm7, ymm5, ymm5
	vpermi2q ymm0, ymm2, yword [0x2f1]
	vpermi2q zmm7, zmm6, zmm4
	vpermi2q zmm6, zmm5, zmm5
	vpermi2q xmm9, xmm14, xmm13
	vpermi2q ymm13, ymm15, ymm14
	vpermi2q zmm14, zmm12, zmm9
	vpermi2q xmm30, xmm25, xmm26
	vpermi2q ymm30, ymm30, ymm26
	vpermi2q zmm22, zmm22, zmm16
	vpermi2q xmm5{k1}, xmm1, xmm1
	vpermi2q ymm7{k2}, ymm5, ymm3
	vpermi2q zmm1{k5}, zmm6, zword [0xfa3]
	vpermi2q xmm4{k5}{z}, xmm3, xmm7
	vpermi2q ymm5{k1}{z}, ymm5, ymm0
	vpermi2q zmm7{k7}{z}, zmm5, zword [0xd0f]
	vpermi2q xmm2, xmm7, oword [eax+1]
	vpermi2q xmm4, xmm5, oword [eax+64]
	vpermi2q ymm6, ymm5, yword [eax+1]
	vpermi2q ymm5, ymm5, yword [eax+64]
	vpermi2q zmm4, zmm7, zword [eax+1]
	vpermi2q zmm1, zmm4, zword [eax+64]
	vpermi2q xmm0, xmm0, [0xad5]
	vpermi2q ymm2, ymm6, [0xd72]
	vpermi2q zmm2, zmm3, [0xd92]
