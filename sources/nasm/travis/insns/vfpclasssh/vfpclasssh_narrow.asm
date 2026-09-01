	vfpclasssh k5, xmm0, 0xb7
	vfpclasssh k2, word [0xc1e], 0x5c

%ifdef ERROR
	vfpclasssh k6, xmm11, 0x4e
	vfpclasssh k2, xmm16, 0x11
%endif
