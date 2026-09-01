	vcmptrue_uqsd xmm6, xmm7, qword [0xc64]
	vcmptrue_uqsd xmm6, qword [0xc64]
	vcmptrue_uqsd xmm4, xmm4, xmm2
	vcmptrue_uqsd k2, xmm3, xmm5
	vcmptrue_uqsd k4, xmm4, xmm2

%ifdef ERROR
	vcmptrue_uqsd xmm10, xmm9, xmm10
	vcmptrue_uqsd k2, xmm12, xmm13
%endif
