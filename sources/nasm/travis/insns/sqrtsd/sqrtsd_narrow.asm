	sqrtsd xmm0, xmm0
	sqrtsd xmm7, qword [0xbd9]

%ifdef ERROR
	sqrtsd xmm12, xmm9
%endif
