	sqrtpd xmm7, oword [0xb6f]
	sqrtpd xmm2, oword [0x5ff]

%ifdef ERROR
	sqrtpd xmm14, xmm13
%endif
