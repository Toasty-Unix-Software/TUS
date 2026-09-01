	vcmpfalse_ossd xmm2, xmm2, qword [0xa65]
	vcmpfalse_ossd xmm2, qword [0xa65]
	vcmpfalse_ossd xmm7, xmm2, xmm2
	vcmpfalse_ossd k2, xmm0, qword [0xf0a]
	vcmpfalse_ossd k6, xmm0, qword [0x660]

%ifdef ERROR
	vcmpfalse_ossd xmm11, xmm14, xmm14
	vcmpfalse_ossd k3, xmm13, xmm12
%endif
