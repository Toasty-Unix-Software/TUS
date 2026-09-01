	vcmpneq_uqsd xmm3, xmm7, xmm6
	vcmpneq_uqsd xmm3, xmm6
	vcmpneq_uqsd xmm2, xmm0, xmm2
	vcmpneq_uqsd k7, xmm0, xmm6
	vcmpneq_uqsd k1, xmm3, xmm4

%ifdef ERROR
	vcmpneq_uqsd xmm12, xmm8, xmm15
	vcmpneq_uqsd k4, xmm10, xmm10
%endif
