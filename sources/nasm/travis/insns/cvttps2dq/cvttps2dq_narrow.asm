	cvttps2dq xmm4, xmm2
	cvttps2dq xmm3, oword [0x7d4]

%ifdef ERROR
	cvttps2dq xmm11, xmm10
%endif
