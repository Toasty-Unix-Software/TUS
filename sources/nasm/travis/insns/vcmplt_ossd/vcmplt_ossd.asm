default rel
	vcmplt_ossd xmm6, xmm7, qword [0xb27]
	vcmplt_ossd xmm6, qword [0xb27]
	vcmplt_ossd xmm7, xmm7, xmm1
	vcmplt_ossd k3, xmm2, xmm7
	vcmplt_ossd k2, xmm5, qword [0x18f]
	vcmplt_ossd xmm10, xmm15, xmm13
	vcmplt_ossd k3, xmm14, xmm15
	vcmplt_ossd k2{k3}, xmm1, xmm3
	vcmplt_ossd k6, xmm5, xmm0, {sae}
	vcmplt_ossd xmm1, xmm4, qword [eax+1]
	vcmplt_ossd xmm2, xmm0, qword [eax+64]
	vcmplt_ossd k5, xmm4, qword [eax+1]
	vcmplt_ossd k2, xmm5, qword [eax+64]
	vcmplt_ossd xmm4, xmm0, [0x663]
	vcmplt_ossd k7, xmm0, [0x49f]
