	vfpclassss k2, dword [0xf56], 0x28
	vfpclassss k6, xmm4, 0xba

%ifdef ERROR
	vfpclassss k1, xmm15, 0x2b
	vfpclassss k5, xmm22, 0x76
%endif
