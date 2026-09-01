default rel
	vcmpnlt_uqss xmm3, xmm2, qword [0x660]
	vcmpnlt_uqss xmm3, qword [0x660]
	vcmpnlt_uqss xmm3, xmm2, qword [0xa14]
	vcmpnlt_uqss k3, xmm1, xmm6
	vcmpnlt_uqss k6, xmm2, xmm1
	vcmpnlt_uqss xmm14, xmm12, xmm9
	vcmpnlt_uqss k5, xmm9, xmm10
	vcmpnlt_uqss k4{k3}, xmm6, xmm3
	vcmpnlt_uqss k1, xmm0, xmm4, {sae}
	vcmpnlt_uqss xmm2, xmm0, qword [eax+1]
	vcmpnlt_uqss xmm2, xmm7, qword [eax+64]
	vcmpnlt_uqss k3, xmm6, dword [eax+1]
	vcmpnlt_uqss k6, xmm0, dword [eax+64]
	vcmpnlt_uqss xmm3, xmm4, [0x106]
	vcmpnlt_uqss k3, xmm0, [0xc9f]
