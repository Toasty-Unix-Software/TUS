default rel
	vaddsd xmm1, xmm5, qword [0x3d5]
	vaddsd xmm1, qword [0x3d5]
	vaddsd xmm0, xmm2, qword [0x237]
	vaddsd xmm2, xmm5, xmm4
	vaddsd xmm2, xmm4
	vaddsd xmm3, xmm0, xmm2
	vaddsd xmm11, xmm10, xmm9
	vaddsd xmm15, xmm8, xmm11
	vaddsd xmm29, xmm22, xmm16
	vaddsd xmm26, xmm26, xmm20
	vaddsd xmm1{k7}, xmm4, xmm2
	vaddsd xmm1{k2}{z}, xmm0, qword [0x8eb]
	vaddsd xmm0, xmm1, xmm6, {rn-sae}
	vaddsd xmm1, xmm1, qword [eax+1]
	vaddsd xmm7, xmm7, qword [eax+64]
	vaddsd xmm7, xmm4, qword [eax+1]
	vaddsd xmm1, xmm5, qword [eax+64]
	vaddsd xmm7, xmm2, [0x754]
	vaddsd xmm3, xmm4, [0x4fb]
