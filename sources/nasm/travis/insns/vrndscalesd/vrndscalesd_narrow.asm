	vrndscalesd xmm5, xmm7, xmm7, 0x93
	vrndscalesd xmm5, xmm7, 0x93
	vrndscalesd xmm5, xmm0, qword [0x81b], 0x65

%ifdef ERROR
	vrndscalesd xmm13, xmm8, xmm9, 0x6a
	vrndscalesd xmm25, xmm19, xmm19, 0xfd
%endif
