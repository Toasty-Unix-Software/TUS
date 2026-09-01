default rel
	vcmpeq_ossd xmm7, xmm4, xmm3
	vcmpeq_ossd xmm7, xmm3
	vcmpeq_ossd xmm3, xmm4, qword [0x390]
	vcmpeq_ossd k1, xmm2, xmm6
	vcmpeq_ossd k5, xmm7, xmm6
	vcmpeq_ossd xmm14, xmm10, xmm13
	vcmpeq_ossd k3, xmm15, xmm10
	vcmpeq_ossd k3{k4}, xmm5, xmm1
	vcmpeq_ossd k1, xmm3, xmm1, {sae}
	vcmpeq_ossd xmm3, xmm6, qword [eax+1]
	vcmpeq_ossd xmm7, xmm4, qword [eax+64]
	vcmpeq_ossd k7, xmm4, qword [eax+1]
	vcmpeq_ossd k1, xmm1, qword [eax+64]
	vcmpeq_ossd xmm0, xmm2, [0xc2d]
	vcmpeq_ossd k5, xmm2, [0x894]
