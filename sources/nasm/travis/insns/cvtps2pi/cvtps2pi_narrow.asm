	cvtps2pi mm6, qword [0x4b6]
	cvtps2pi mm1, qword [0x483]

%ifdef ERROR
	cvtps2pi mm6, xmm15
%endif
