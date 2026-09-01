	vroundsd xmm1, xmm7, xmm6, 0xfd
	vroundsd xmm1, xmm6, 0xfd
	vroundsd xmm5, xmm0, xmm3, 0x80

%ifdef ERROR
	vroundsd xmm10, xmm11, xmm11, 0xfe
%endif
