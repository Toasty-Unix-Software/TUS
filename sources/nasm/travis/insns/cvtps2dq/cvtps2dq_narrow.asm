	cvtps2dq xmm0, oword [0x40b]
	cvtps2dq xmm7, xmm0

%ifdef ERROR
	cvtps2dq xmm9, xmm11
%endif
