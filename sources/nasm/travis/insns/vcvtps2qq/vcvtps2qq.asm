default rel
	vcvtps2qq xmm7, qword [0xcac]
	vcvtps2qq xmm5, xmm0
	vcvtps2qq ymm6, xmm1
	vcvtps2qq ymm6, xmm1
	vcvtps2qq zmm2, ymm3
	vcvtps2qq zmm5, ymm0
	vcvtps2qq xmm14, xmm10
	vcvtps2qq ymm15, xmm8
	vcvtps2qq zmm8, ymm12
	vcvtps2qq xmm18, xmm17
	vcvtps2qq ymm30, xmm24
	vcvtps2qq zmm16, ymm26
	vcvtps2qq xmm1{k5}, qword [0xc57]
	vcvtps2qq ymm6{k7}, oword [0x74b]
	vcvtps2qq zmm4{k3}, ymm6
	vcvtps2qq xmm6{k7}{z}, qword [0x23d]
	vcvtps2qq ymm7{k4}{z}, xmm4
	vcvtps2qq zmm3{k3}{z}, ymm0
	vcvtps2qq zmm2, ymm0, {rn-sae}
	vcvtps2qq xmm0, qword [eax+1]
	vcvtps2qq xmm0, qword [eax+64]
	vcvtps2qq ymm6, oword [eax+1]
	vcvtps2qq ymm3, oword [eax+64]
	vcvtps2qq zmm5, yword [eax+1]
	vcvtps2qq zmm4, yword [eax+64]
	vcvtps2qq xmm4, [0xcbc]
	vcvtps2qq ymm1, [0x290]
	vcvtps2qq zmm3, [0xdaa]
