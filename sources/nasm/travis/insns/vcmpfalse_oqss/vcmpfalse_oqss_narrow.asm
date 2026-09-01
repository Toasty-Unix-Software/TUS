	vcmpfalse_oqss xmm5, xmm7, qword [0x5bf]
	vcmpfalse_oqss xmm5, qword [0x5bf]
	vcmpfalse_oqss xmm3, xmm7, qword [0x380]
	vcmpfalse_oqss k6, xmm7, xmm1
	vcmpfalse_oqss k2, xmm3, xmm5

%ifdef ERROR
	vcmpfalse_oqss xmm11, xmm14, xmm14
	vcmpfalse_oqss k5, xmm11, xmm11
%endif
