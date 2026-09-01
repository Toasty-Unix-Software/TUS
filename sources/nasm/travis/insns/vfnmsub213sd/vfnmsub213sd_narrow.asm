	vfnmsub213sd xmm1, xmm0, qword [0xee0]
	vfnmsub213sd xmm2, xmm4, xmm4
	vfnmsub213sd xmm3, xmm0, xmm5
	vfnmsub213sd xmm3, xmm6, xmm7

%ifdef ERROR
	vfnmsub213sd xmm15, xmm12, xmm15
	vfnmsub213sd xmm8, xmm12, xmm12
	vfnmsub213sd xmm20, xmm22, xmm26
	vfnmsub213sd xmm25, xmm20, xmm28
%endif
