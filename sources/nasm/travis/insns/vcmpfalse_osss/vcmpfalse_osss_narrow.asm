	vcmpfalse_osss xmm3, xmm5, xmm2
	vcmpfalse_osss xmm3, xmm2
	vcmpfalse_osss xmm5, xmm7, qword [0x65b]
	vcmpfalse_osss k7, xmm3, dword [0xa9a]
	vcmpfalse_osss k5, xmm1, xmm7

%ifdef ERROR
	vcmpfalse_osss xmm14, xmm11, xmm12
	vcmpfalse_osss k3, xmm9, xmm9
%endif
