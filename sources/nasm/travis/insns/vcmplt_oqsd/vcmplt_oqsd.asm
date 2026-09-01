default rel
	vcmplt_oqsd xmm4, xmm0, xmm1
	vcmplt_oqsd xmm4, xmm1
	vcmplt_oqsd xmm5, xmm3, qword [0x912]
	vcmplt_oqsd k3, xmm5, xmm7
	vcmplt_oqsd k1, xmm7, xmm2
	vcmplt_oqsd xmm9, xmm13, xmm13
	vcmplt_oqsd k4, xmm12, xmm12
	vcmplt_oqsd k6{k6}, xmm6, qword [0xc95]
	vcmplt_oqsd k4, xmm5, xmm6, {sae}
	vcmplt_oqsd xmm5, xmm4, qword [eax+1]
	vcmplt_oqsd xmm3, xmm6, qword [eax+64]
	vcmplt_oqsd k1, xmm3, qword [eax+1]
	vcmplt_oqsd k4, xmm3, qword [eax+64]
	vcmplt_oqsd xmm1, xmm0, [0x76c]
	vcmplt_oqsd k5, xmm1, [0xbff]
