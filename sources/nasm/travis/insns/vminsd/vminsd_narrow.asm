	vminsd xmm2, xmm7, qword [0xb83]
	vminsd xmm2, qword [0xb83]
	vminsd xmm4, xmm4, xmm4
	vminsd xmm3, xmm2, xmm0
	vminsd xmm3, xmm0
	vminsd xmm2, xmm3, xmm7

%ifdef ERROR
	vminsd xmm9, xmm14, xmm10
	vminsd xmm13, xmm12, xmm9
	vminsd xmm18, xmm31, xmm27
	vminsd xmm30, xmm16, xmm26
%endif
