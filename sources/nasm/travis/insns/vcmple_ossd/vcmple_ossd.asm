default rel
	vcmple_ossd xmm3, xmm2, xmm2
	vcmple_ossd xmm3, xmm2
	vcmple_ossd xmm7, xmm1, xmm3
	vcmple_ossd k6, xmm2, xmm7
	vcmple_ossd k5, xmm4, xmm0
	vcmple_ossd xmm10, xmm13, xmm8
	vcmple_ossd k2, xmm9, xmm13
	vcmple_ossd k4{k5}, xmm2, qword [0x781]
	vcmple_ossd k4, xmm6, xmm1, {sae}
	vcmple_ossd xmm7, xmm6, qword [eax+1]
	vcmple_ossd xmm5, xmm5, qword [eax+64]
	vcmple_ossd k5, xmm3, qword [eax+1]
	vcmple_ossd k2, xmm5, qword [eax+64]
	vcmple_ossd xmm2, xmm3, [0x20b]
	vcmple_ossd k3, xmm5, [0xd53]
