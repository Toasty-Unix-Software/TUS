	vcmpsh k4, xmm5, word [0xc8f], 0xfa
	vcmpsh k4, word [0xc8f], 0xfa
	vcmpsh k3, xmm5, word [0xfd4], 0x7d

%ifdef ERROR
	vcmpsh k1, xmm14, xmm9, 0x33
	vcmpsh k2, xmm20, xmm23, 0xba
%endif
