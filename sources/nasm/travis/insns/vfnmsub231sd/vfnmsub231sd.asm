default rel
	vfnmsub231sd xmm4, xmm1, xmm4
	vfnmsub231sd xmm4, xmm5, qword [0x748]
	vfnmsub231sd xmm4, xmm1, xmm1
	vfnmsub231sd xmm2, xmm6, qword [0xaed]
	vfnmsub231sd xmm8, xmm15, xmm12
	vfnmsub231sd xmm12, xmm10, xmm10
	vfnmsub231sd xmm18, xmm16, xmm24
	vfnmsub231sd xmm29, xmm26, xmm22
	vfnmsub231sd xmm2{k2}, xmm5, qword [0x318]
	vfnmsub231sd xmm1{k5}{z}, xmm4, xmm6
	vfnmsub231sd xmm3, xmm2, xmm0, {rz-sae}
	vfnmsub231sd xmm6, xmm0, qword [eax+1]
	vfnmsub231sd xmm4, xmm0, qword [eax+64]
	vfnmsub231sd xmm7, xmm6, qword [eax+1]
	vfnmsub231sd xmm5, xmm3, qword [eax+64]
	vfnmsub231sd xmm6, xmm1, [0x880]
	vfnmsub231sd xmm1, xmm4, [0x390]
