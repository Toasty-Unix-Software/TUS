	vgetexpsd xmm0, xmm7, xmm0
	vgetexpsd xmm1, xmm6, qword [0x121]

%ifdef ERROR
	vgetexpsd xmm12, xmm12, xmm10
	vgetexpsd xmm26, xmm25, xmm19
%endif
