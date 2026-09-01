default rel
	vcmpgt_ossd xmm5, xmm5, xmm5
	vcmpgt_ossd xmm5, xmm5
	vcmpgt_ossd xmm2, xmm6, qword [0x51b]
	vcmpgt_ossd k5, xmm6, qword [0x9bb]
	vcmpgt_ossd k4, xmm6, xmm2
	vcmpgt_ossd xmm11, xmm8, xmm12
	vcmpgt_ossd k2, xmm11, xmm14
	vcmpgt_ossd k5{k1}, xmm0, qword [0x834]
	vcmpgt_ossd k6, xmm5, xmm6, {sae}
	vcmpgt_ossd xmm0, xmm3, qword [eax+1]
	vcmpgt_ossd xmm0, xmm1, qword [eax+64]
	vcmpgt_ossd k7, xmm6, qword [eax+1]
	vcmpgt_ossd k4, xmm2, qword [eax+64]
	vcmpgt_ossd xmm3, xmm3, [0xfc4]
	vcmpgt_ossd k2, xmm2, [0x11a]
