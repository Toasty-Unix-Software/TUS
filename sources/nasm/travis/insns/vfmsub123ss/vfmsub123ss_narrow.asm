	vfmsub123ss xmm3, xmm2, xmm4
	vfmsub123ss xmm2, xmm7, dword [0x3cc]

%ifdef ERROR
	vfmsub123ss xmm8, xmm12, xmm9
%endif
