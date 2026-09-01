	vcmpfalse_oqsd xmm1, xmm7, xmm3
	vcmpfalse_oqsd xmm1, xmm3
	vcmpfalse_oqsd xmm1, xmm4, xmm6
	vcmpfalse_oqsd k1, xmm3, xmm4
	vcmpfalse_oqsd k4, xmm5, qword [0x68d]

%ifdef ERROR
	vcmpfalse_oqsd xmm9, xmm13, xmm9
	vcmpfalse_oqsd k3, xmm10, xmm8
%endif
