default rel
	vcmpeq_uqsd xmm1, xmm4, xmm2
	vcmpeq_uqsd xmm1, xmm2
	vcmpeq_uqsd xmm2, xmm5, qword [0xdca]
	vcmpeq_uqsd k7, xmm5, xmm7
	vcmpeq_uqsd k7, xmm5, xmm0
	vcmpeq_uqsd xmm10, xmm14, xmm9
	vcmpeq_uqsd k1, xmm15, xmm15
	vcmpeq_uqsd k1{k4}, xmm4, xmm3
	vcmpeq_uqsd k2, xmm2, xmm1, {sae}
	vcmpeq_uqsd xmm1, xmm7, qword [eax+1]
	vcmpeq_uqsd xmm1, xmm2, qword [eax+64]
	vcmpeq_uqsd k1, xmm6, qword [eax+1]
	vcmpeq_uqsd k6, xmm4, qword [eax+64]
	vcmpeq_uqsd xmm7, xmm5, [0x48d]
	vcmpeq_uqsd k6, xmm6, [0x60c]
