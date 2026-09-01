default rel
	vcmptrue_uqsd xmm6, xmm7, qword [0xc64]
	vcmptrue_uqsd xmm6, qword [0xc64]
	vcmptrue_uqsd xmm4, xmm4, xmm2
	vcmptrue_uqsd k2, xmm3, xmm5
	vcmptrue_uqsd k4, xmm4, xmm2
	vcmptrue_uqsd xmm10, xmm9, xmm10
	vcmptrue_uqsd k2, xmm12, xmm13
	vcmptrue_uqsd k1{k3}, xmm2, xmm3
	vcmptrue_uqsd k4, xmm0, xmm2, {sae}
	vcmptrue_uqsd xmm2, xmm6, qword [eax+1]
	vcmptrue_uqsd xmm2, xmm0, qword [eax+64]
	vcmptrue_uqsd k1, xmm7, qword [eax+1]
	vcmptrue_uqsd k4, xmm3, qword [eax+64]
	vcmptrue_uqsd xmm6, xmm7, [0x114]
	vcmptrue_uqsd k6, xmm5, [0x4cb]
