default rel
	vcvttph2qq xmm5, xmm4
	vcvttph2qq xmm3, xmm6
	vcvttph2qq ymm4, xmm1
	vcvttph2qq ymm3, xmm4
	vcvttph2qq zmm6, xmm6
	vcvttph2qq zmm5, oword [0x992]
	vcvttph2qq xmm11, xmm9
	vcvttph2qq ymm12, xmm13
	vcvttph2qq zmm13, xmm13
	vcvttph2qq xmm17, xmm16
	vcvttph2qq ymm22, xmm24
	vcvttph2qq zmm23, xmm29
	vcvttph2qq xmm2{k1}, xmm6
	vcvttph2qq ymm0{k6}, xmm0
	vcvttph2qq zmm5{k2}, xmm0
	vcvttph2qq xmm1{k1}{z}, dword [0xfa5]
	vcvttph2qq ymm6{k4}{z}, xmm5
	vcvttph2qq zmm5{k7}{z}, xmm4
	vcvttph2qq zmm3, xmm0, {sae}
	vcvttph2qq xmm3, dword [eax+1]
	vcvttph2qq xmm2, dword [eax+64]
	vcvttph2qq ymm6, qword [eax+1]
	vcvttph2qq ymm1, qword [eax+64]
	vcvttph2qq zmm3, oword [eax+1]
	vcvttph2qq zmm2, oword [eax+64]
	vcvttph2qq xmm6, [0x7b6]
	vcvttph2qq ymm6, [0xb6a]
	vcvttph2qq zmm1, [0x640]
