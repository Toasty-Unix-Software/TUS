default rel
	vcvtqq2ps xmm1, xmm6
	vcvtqq2ps xmm0, xmm3
	vcvtqq2ps xmm4, ymm1
	vcvtqq2ps xmm2, yword [0xb08]
	vcvtqq2ps ymm0, zmm2
	vcvtqq2ps ymm7, zmm0
	vcvtqq2ps xmm13, xmm15
	vcvtqq2ps xmm10, ymm10
	vcvtqq2ps ymm12, zmm13
	vcvtqq2ps xmm27, xmm21
	vcvtqq2ps xmm20, ymm29
	vcvtqq2ps ymm29, zmm26
	vcvtqq2ps xmm5{k4}, xmm4
	vcvtqq2ps xmm1{k6}, ymm4
	vcvtqq2ps ymm6{k6}, zmm3
	vcvtqq2ps xmm6{k5}{z}, xmm3
	vcvtqq2ps xmm7{k3}{z}, yword [0x3a8]
	vcvtqq2ps ymm7{k4}{z}, zmm6
	vcvtqq2ps ymm4, zmm4, {rz-sae}
	vcvtqq2ps xmm6, oword [eax+1]
	vcvtqq2ps xmm1, oword [eax+64]
	vcvtqq2ps xmm7, yword [eax+1]
	vcvtqq2ps xmm2, yword [eax+64]
	vcvtqq2ps ymm2, zword [eax+1]
	vcvtqq2ps ymm3, zword [eax+64]
	vcvtqq2ps xmm1, [0x678]
	vcvtqq2ps xmm1, [0xc71]
	vcvtqq2ps ymm6, [0xf07]
