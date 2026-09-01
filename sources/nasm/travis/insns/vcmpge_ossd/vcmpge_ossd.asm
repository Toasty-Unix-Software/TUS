default rel
	vcmpge_ossd xmm3, xmm2, xmm3
	vcmpge_ossd xmm3, xmm3
	vcmpge_ossd xmm6, xmm5, xmm6
	vcmpge_ossd k4, xmm0, xmm2
	vcmpge_ossd k7, xmm6, qword [0x8a4]
	vcmpge_ossd xmm11, xmm8, xmm12
	vcmpge_ossd k3, xmm11, xmm10
	vcmpge_ossd k4{k3}, xmm6, xmm5
	vcmpge_ossd k4, xmm0, xmm0, {sae}
	vcmpge_ossd xmm0, xmm7, qword [eax+1]
	vcmpge_ossd xmm6, xmm6, qword [eax+64]
	vcmpge_ossd k2, xmm0, qword [eax+1]
	vcmpge_ossd k1, xmm2, qword [eax+64]
	vcmpge_ossd xmm6, xmm5, [0x91b]
	vcmpge_ossd k3, xmm5, [0xdde]
