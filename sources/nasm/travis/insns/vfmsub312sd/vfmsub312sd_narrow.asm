	vfmsub312sd xmm1, xmm2, xmm1
	vfmsub312sd xmm4, xmm3, xmm5

%ifdef ERROR
	vfmsub312sd xmm12, xmm11, xmm9
%endif
