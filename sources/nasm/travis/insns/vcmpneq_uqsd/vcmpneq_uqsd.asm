default rel
	vcmpneq_uqsd xmm3, xmm7, xmm6
	vcmpneq_uqsd xmm3, xmm6
	vcmpneq_uqsd xmm2, xmm0, xmm2
	vcmpneq_uqsd k7, xmm0, xmm6
	vcmpneq_uqsd k1, xmm3, xmm4
	vcmpneq_uqsd xmm12, xmm8, xmm15
	vcmpneq_uqsd k4, xmm10, xmm10
	vcmpneq_uqsd k3{k6}, xmm6, xmm3
	vcmpneq_uqsd k1, xmm3, xmm5, {sae}
	vcmpneq_uqsd xmm0, xmm4, qword [eax+1]
	vcmpneq_uqsd xmm1, xmm0, qword [eax+64]
	vcmpneq_uqsd k4, xmm0, qword [eax+1]
	vcmpneq_uqsd k1, xmm5, qword [eax+64]
	vcmpneq_uqsd xmm7, xmm6, [0xaaf]
	vcmpneq_uqsd k1, xmm0, [0xf57]
