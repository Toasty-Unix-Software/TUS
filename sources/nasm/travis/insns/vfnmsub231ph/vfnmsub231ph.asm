default rel
	vfnmsub231ph xmm4, xmm5, oword [0x25c]
	vfnmsub231ph xmm4, oword [0x25c]
	vfnmsub231ph xmm5, xmm6, xmm3
	vfnmsub231ph ymm4, ymm3, yword [0xfd5]
	vfnmsub231ph ymm4, yword [0xfd5]
	vfnmsub231ph ymm2, ymm7, yword [0x1cb]
	vfnmsub231ph zmm3, zmm5, zword [0xc12]
	vfnmsub231ph zmm3, zword [0xc12]
	vfnmsub231ph zmm0, zmm1, zmm4
	vfnmsub231ph xmm15, xmm10, xmm15
	vfnmsub231ph ymm8, ymm13, ymm9
	vfnmsub231ph zmm15, zmm14, zmm14
	vfnmsub231ph xmm26, xmm24, xmm27
	vfnmsub231ph ymm27, ymm29, ymm27
	vfnmsub231ph zmm21, zmm31, zmm31
	vfnmsub231ph xmm7{k1}, xmm2, oword [0xda8]
	vfnmsub231ph ymm3{k7}, ymm0, yword [0x599]
	vfnmsub231ph zmm2{k5}, zmm4, zword [0x95a]
	vfnmsub231ph xmm5{k7}{z}, xmm1, xmm0
	vfnmsub231ph ymm2{k4}{z}, ymm0, ymm4
	vfnmsub231ph zmm5{k6}{z}, zmm4, zword [0x606]
	vfnmsub231ph zmm7, zmm7, zmm4, {rd-sae}
	vfnmsub231ph xmm1, xmm6, oword [eax+1]
	vfnmsub231ph xmm7, xmm7, oword [eax+64]
	vfnmsub231ph ymm4, ymm2, yword [eax+1]
	vfnmsub231ph ymm1, ymm1, yword [eax+64]
	vfnmsub231ph zmm3, zmm4, zword [eax+1]
	vfnmsub231ph zmm4, zmm2, zword [eax+64]
	vfnmsub231ph xmm3, xmm3, [0x362]
	vfnmsub231ph ymm5, ymm5, [0x4b8]
	vfnmsub231ph zmm5, zmm4, [0x826]
