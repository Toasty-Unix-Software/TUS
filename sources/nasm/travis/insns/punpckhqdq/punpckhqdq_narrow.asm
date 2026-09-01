	punpckhqdq xmm3, xmm2
	punpckhqdq xmm0, oword [0xfee]

%ifdef ERROR
	punpckhqdq xmm8, xmm13
%endif
