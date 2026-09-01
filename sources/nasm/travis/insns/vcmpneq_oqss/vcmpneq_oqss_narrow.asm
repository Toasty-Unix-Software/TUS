	vcmpneq_oqss xmm2, xmm6, xmm5
	vcmpneq_oqss xmm2, xmm5
	vcmpneq_oqss xmm6, xmm4, xmm4
	vcmpneq_oqss k4, xmm6, dword [0xfaa]
	vcmpneq_oqss k2, xmm2, xmm0

%ifdef ERROR
	vcmpneq_oqss xmm12, xmm12, xmm14
	vcmpneq_oqss k7, xmm8, xmm15
%endif
