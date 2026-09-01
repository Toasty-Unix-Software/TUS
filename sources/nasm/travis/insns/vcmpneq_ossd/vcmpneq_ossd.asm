default rel
	vcmpneq_ossd xmm5, xmm1, xmm3
	vcmpneq_ossd xmm5, xmm3
	vcmpneq_ossd xmm4, xmm4, qword [0xb67]
	vcmpneq_ossd k7, xmm6, xmm1
	vcmpneq_ossd k4, xmm7, xmm7
	vcmpneq_ossd xmm14, xmm10, xmm14
	vcmpneq_ossd k5, xmm9, xmm12
	vcmpneq_ossd k4{k4}, xmm4, xmm2
	vcmpneq_ossd k4, xmm1, xmm0, {sae}
	vcmpneq_ossd xmm1, xmm2, qword [eax+1]
	vcmpneq_ossd xmm1, xmm6, qword [eax+64]
	vcmpneq_ossd k3, xmm7, qword [eax+1]
	vcmpneq_ossd k5, xmm6, qword [eax+64]
	vcmpneq_ossd xmm5, xmm7, [0x5f2]
	vcmpneq_ossd k6, xmm3, [0xba3]
