default rel
	vfnmsub132sd xmm5, xmm2, xmm5
	vfnmsub132sd xmm5, xmm2, xmm5
	vfnmsub132sd xmm4, xmm7, xmm7
	vfnmsub132sd xmm6, xmm5, qword [0x8b0]
	vfnmsub132sd xmm12, xmm8, xmm9
	vfnmsub132sd xmm12, xmm11, xmm8
	vfnmsub132sd xmm24, xmm31, xmm26
	vfnmsub132sd xmm21, xmm22, xmm29
	vfnmsub132sd xmm2{k2}, xmm0, xmm6
	vfnmsub132sd xmm4{k1}{z}, xmm5, xmm1
	vfnmsub132sd xmm5, xmm1, xmm2, {rn-sae}
	vfnmsub132sd xmm0, xmm1, qword [eax+1]
	vfnmsub132sd xmm0, xmm3, qword [eax+64]
	vfnmsub132sd xmm0, xmm2, qword [eax+1]
	vfnmsub132sd xmm2, xmm6, qword [eax+64]
	vfnmsub132sd xmm3, xmm0, [0x4e1]
	vfnmsub132sd xmm7, xmm7, [0x9ae]
