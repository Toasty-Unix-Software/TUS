	psubw mm2, mm1
	psubw mm1, qword [0x645]
	psubw xmm6, xmm2
	psubw xmm6, xmm7

%ifdef ERROR
	psubw xmm12, xmm11
%endif
