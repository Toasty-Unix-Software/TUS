default rel
	vaddpd xmm7, xmm5, xmm6
	vaddpd xmm7, xmm6
	vaddpd xmm1, xmm0, xmm7
	vaddpd ymm7, ymm2, ymm2
	vaddpd ymm7, ymm2
	vaddpd ymm4, ymm0, yword [0xf04]
	vaddpd xmm6, xmm6, xmm4
	vaddpd xmm6, xmm4
	vaddpd xmm0, xmm4, xmm5
	vaddpd ymm5, ymm3, yword [0xfe1]
	vaddpd ymm5, yword [0xfe1]
	vaddpd ymm5, ymm6, ymm5
	vaddpd xmm8, xmm8, xmm14
	vaddpd ymm9, ymm14, ymm8
	vaddpd xmm10, xmm9, xmm14
	vaddpd ymm11, ymm11, ymm11
	vaddpd xmm28, xmm25, xmm28
	vaddpd ymm27, ymm30, ymm25
	vaddpd xmm18, xmm22, xmm17
	vaddpd ymm21, ymm31, ymm22
	vaddpd xmm7{k5}, xmm7, xmm7
	vaddpd ymm4{k5}, ymm2, yword [0x641]
	vaddpd zmm0{k4}, zmm0, zmm5
	vaddpd xmm0{k3}{z}, xmm5, xmm7
	vaddpd ymm3{k1}{z}, ymm0, ymm3
	vaddpd zmm4{k7}{z}, zmm3, zword [0x28e]
	vaddpd zmm3, zmm1, zmm4, {rn-sae}
	vaddpd xmm7, xmm7, oword [eax+1]
	vaddpd xmm1, xmm2, oword [eax+64]
	vaddpd ymm0, ymm5, yword [eax+1]
	vaddpd ymm6, ymm7, yword [eax+64]
	vaddpd xmm4, xmm7, oword [eax+1]
	vaddpd xmm4, xmm2, oword [eax+64]
	vaddpd ymm6, ymm0, yword [eax+1]
	vaddpd ymm5, ymm1, yword [eax+64]
	vaddpd zmm3, zmm7, zword [eax+1]
	vaddpd zmm4, zmm1, zword [eax+64]
	vaddpd xmm7, xmm1, [0xf06]
	vaddpd ymm0, ymm5, [0xd97]
	vaddpd xmm0, xmm3, [0x399]
	vaddpd ymm5, ymm5, [0x516]
	vaddpd zmm6, zmm2, [0x793]
