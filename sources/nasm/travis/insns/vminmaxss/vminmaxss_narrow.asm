	vminmaxss xmm5, xmm0, dword [0x309], 0xbf
	vminmaxss xmm0, xmm6, xmm4, 0x4d

%ifdef ERROR
	vminmaxss xmm15, xmm9, xmm13, 0x30
	vminmaxss xmm25, xmm20, xmm18, 0x89
%endif
