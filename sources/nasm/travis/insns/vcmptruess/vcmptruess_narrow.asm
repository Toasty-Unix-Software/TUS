	vcmptruess xmm6, xmm1, xmm0
	vcmptruess xmm6, xmm0
	vcmptruess xmm2, xmm5, xmm0
	vcmptruess k6, xmm5, xmm5
	vcmptruess k1, xmm6, dword [0xac5]

%ifdef ERROR
	vcmptruess xmm12, xmm12, xmm11
	vcmptruess k3, xmm8, xmm13
%endif
