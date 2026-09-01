	vfrczpd xmm3, xmm0
	vfrczpd xmm3
	vfrczpd xmm1, xmm7
	vfrczpd ymm7, yword [0xa82]
	vfrczpd ymm7
	vfrczpd ymm6, ymm5

%ifdef ERROR
	vfrczpd xmm8, xmm8
	vfrczpd ymm15, ymm11
%endif
