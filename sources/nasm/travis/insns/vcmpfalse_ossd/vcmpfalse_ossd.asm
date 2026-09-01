default rel
	vcmpfalse_ossd xmm2, xmm2, qword [0xa65]
	vcmpfalse_ossd xmm2, qword [0xa65]
	vcmpfalse_ossd xmm7, xmm2, xmm2
	vcmpfalse_ossd k2, xmm0, qword [0xf0a]
	vcmpfalse_ossd k6, xmm0, qword [0x660]
	vcmpfalse_ossd xmm11, xmm14, xmm14
	vcmpfalse_ossd k3, xmm13, xmm12
	vcmpfalse_ossd k2{k4}, xmm7, qword [0x1f5]
	vcmpfalse_ossd k1, xmm2, xmm6, {sae}
	vcmpfalse_ossd xmm1, xmm2, qword [eax+1]
	vcmpfalse_ossd xmm4, xmm5, qword [eax+64]
	vcmpfalse_ossd k1, xmm2, qword [eax+1]
	vcmpfalse_ossd k5, xmm3, qword [eax+64]
	vcmpfalse_ossd xmm7, xmm1, [0x5bd]
	vcmpfalse_ossd k4, xmm4, [0xd5f]
