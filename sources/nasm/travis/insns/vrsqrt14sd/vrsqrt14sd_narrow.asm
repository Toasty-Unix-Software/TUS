	vrsqrt14sd xmm3, xmm0, qword [0xe52]
	vrsqrt14sd xmm3, qword [0xe52]
	vrsqrt14sd xmm6, xmm5, xmm1

%ifdef ERROR
	vrsqrt14sd xmm15, xmm13, xmm14
	vrsqrt14sd xmm21, xmm26, xmm21
%endif
