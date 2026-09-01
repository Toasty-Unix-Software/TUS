	vhsubpd xmm5, xmm1, xmm7
	vhsubpd xmm5, xmm7
	vhsubpd xmm2, xmm7, xmm7
	vhsubpd ymm3, ymm1, ymm5
	vhsubpd ymm3, ymm5
	vhsubpd ymm3, ymm6, ymm3

%ifdef ERROR
	vhsubpd xmm9, xmm10, xmm15
	vhsubpd ymm10, ymm10, ymm15
%endif
