default rel
	vcvtph2qq xmm5, xmm1
	vcvtph2qq xmm2, xmm1
	vcvtph2qq ymm6, qword [0x8b1]
	vcvtph2qq ymm7, xmm1
	vcvtph2qq zmm3, oword [0x195]
	vcvtph2qq zmm2, oword [0x378]
	vcvtph2qq xmm13, xmm15
	vcvtph2qq ymm9, xmm9
	vcvtph2qq zmm12, xmm11
	vcvtph2qq xmm20, xmm22
	vcvtph2qq ymm22, xmm29
	vcvtph2qq zmm30, xmm31
	vcvtph2qq xmm0{k7}, xmm5
	vcvtph2qq ymm4{k3}, xmm1
	vcvtph2qq zmm2{k4}, xmm5
	vcvtph2qq xmm2{k6}{z}, xmm0
	vcvtph2qq ymm7{k4}{z}, xmm3
	vcvtph2qq zmm2{k2}{z}, xmm0
	vcvtph2qq zmm0, xmm4, {ru-sae}
	vcvtph2qq xmm3, dword [eax+1]
	vcvtph2qq xmm1, dword [eax+64]
	vcvtph2qq ymm6, qword [eax+1]
	vcvtph2qq ymm7, qword [eax+64]
	vcvtph2qq zmm2, oword [eax+1]
	vcvtph2qq zmm1, oword [eax+64]
	vcvtph2qq xmm1, [0x362]
	vcvtph2qq ymm3, [0x99c]
	vcvtph2qq zmm0, [0x3c5]
