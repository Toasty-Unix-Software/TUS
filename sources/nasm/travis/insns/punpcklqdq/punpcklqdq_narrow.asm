	punpcklqdq xmm6, xmm1
	punpcklqdq xmm4, oword [0x674]

%ifdef ERROR
	punpcklqdq xmm15, xmm14
%endif
