	vrcpss xmm2, xmm2, xmm6
	vrcpss xmm2, xmm6
	vrcpss xmm2, xmm1, dword [0x97e]

%ifdef ERROR
	vrcpss xmm8, xmm15, xmm12
%endif
