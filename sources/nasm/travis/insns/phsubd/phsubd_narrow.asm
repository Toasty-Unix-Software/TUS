	phsubd mm3, qword [0x3ed]
	phsubd mm5, qword [0x208]
	phsubd xmm3, xmm7
	phsubd xmm5, xmm2

%ifdef ERROR
	phsubd xmm12, xmm15
%endif
