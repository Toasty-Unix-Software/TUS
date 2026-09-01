default rel
	vcvtps2uqq xmm7, xmm4
	vcvtps2uqq xmm2, xmm4
	vcvtps2uqq ymm7, xmm4
	vcvtps2uqq ymm7, xmm7
	vcvtps2uqq zmm0, ymm7
	vcvtps2uqq zmm5, ymm6
	vcvtps2uqq xmm11, xmm10
	vcvtps2uqq ymm14, xmm11
	vcvtps2uqq zmm12, ymm12
	vcvtps2uqq xmm30, xmm17
	vcvtps2uqq ymm17, xmm27
	vcvtps2uqq zmm16, ymm22
	vcvtps2uqq xmm1{k7}, xmm1
	vcvtps2uqq ymm3{k4}, xmm3
	vcvtps2uqq zmm5{k6}, ymm7
	vcvtps2uqq xmm3{k3}{z}, xmm5
	vcvtps2uqq ymm1{k3}{z}, xmm6
	vcvtps2uqq zmm1{k4}{z}, ymm2
	vcvtps2uqq zmm4, ymm0, {rn-sae}
	vcvtps2uqq xmm7, qword [eax+1]
	vcvtps2uqq xmm7, qword [eax+64]
	vcvtps2uqq ymm0, oword [eax+1]
	vcvtps2uqq ymm5, oword [eax+64]
	vcvtps2uqq zmm2, yword [eax+1]
	vcvtps2uqq zmm0, yword [eax+64]
	vcvtps2uqq xmm2, [0x96c]
	vcvtps2uqq ymm6, [0x1b9]
	vcvtps2uqq zmm4, [0x72f]
