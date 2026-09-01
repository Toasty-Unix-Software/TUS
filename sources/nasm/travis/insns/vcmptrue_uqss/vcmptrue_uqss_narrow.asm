	vcmptrue_uqss xmm5, xmm6, xmm3
	vcmptrue_uqss xmm5, xmm3
	vcmptrue_uqss xmm0, xmm7, xmm1
	vcmptrue_uqss k5, xmm0, xmm7
	vcmptrue_uqss k4, xmm7, xmm0

%ifdef ERROR
	vcmptrue_uqss xmm8, xmm15, xmm12
	vcmptrue_uqss k4, xmm12, xmm10
%endif
