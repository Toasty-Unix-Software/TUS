	vfnmsub123sd xmm0, xmm6, qword [0x281]
	vfnmsub123sd xmm4, xmm4, xmm5

%ifdef ERROR
	vfnmsub123sd xmm13, xmm11, xmm13
%endif
