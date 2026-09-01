default rel
	vcvtbf162ibs xmm3, xmm3
	vcvtbf162ibs xmm7, oword [0xb80]
	vcvtbf162ibs ymm4, yword [0xa72]
	vcvtbf162ibs ymm4, ymm2
	vcvtbf162ibs zmm4, zmm5
	vcvtbf162ibs zmm2, zmm2
	vcvtbf162ibs xmm10, xmm10
	vcvtbf162ibs ymm15, ymm15
	vcvtbf162ibs zmm11, zmm14
	vcvtbf162ibs xmm16, xmm22
	vcvtbf162ibs ymm25, ymm20
	vcvtbf162ibs zmm31, zmm26
	vcvtbf162ibs xmm2{k7}, oword [0xfe9]
	vcvtbf162ibs ymm4{k6}, ymm1
	vcvtbf162ibs zmm0{k5}, zmm2
	vcvtbf162ibs xmm7{k4}{z}, oword [0xa6e]
	vcvtbf162ibs ymm2{k6}{z}, ymm7
	vcvtbf162ibs zmm7{k5}{z}, zmm2
	vcvtbf162ibs xmm6, oword [eax+1]
	vcvtbf162ibs xmm4, oword [eax+64]
	vcvtbf162ibs ymm5, yword [eax+1]
	vcvtbf162ibs ymm7, yword [eax+64]
	vcvtbf162ibs zmm6, zword [eax+1]
	vcvtbf162ibs zmm4, zword [eax+64]
	vcvtbf162ibs xmm1, [0xb8a]
	vcvtbf162ibs ymm5, [0xdec]
	vcvtbf162ibs zmm6, [0x1fc]
