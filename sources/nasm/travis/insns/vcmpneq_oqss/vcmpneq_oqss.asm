default rel
	vcmpneq_oqss xmm2, xmm6, xmm5
	vcmpneq_oqss xmm2, xmm5
	vcmpneq_oqss xmm6, xmm4, xmm4
	vcmpneq_oqss k4, xmm6, dword [0xfaa]
	vcmpneq_oqss k2, xmm2, xmm0
	vcmpneq_oqss xmm12, xmm12, xmm14
	vcmpneq_oqss k7, xmm8, xmm15
	vcmpneq_oqss k2{k4}, xmm7, xmm2
	vcmpneq_oqss k6, xmm3, xmm0, {sae}
	vcmpneq_oqss xmm4, xmm4, qword [eax+1]
	vcmpneq_oqss xmm5, xmm4, qword [eax+64]
	vcmpneq_oqss k1, xmm5, dword [eax+1]
	vcmpneq_oqss k4, xmm6, dword [eax+64]
	vcmpneq_oqss xmm5, xmm7, [0x69f]
	vcmpneq_oqss k3, xmm3, [0xa6c]
