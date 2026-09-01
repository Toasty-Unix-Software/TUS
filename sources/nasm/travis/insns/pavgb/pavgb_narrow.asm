	pavgb mm7, qword [0x880]
	pavgb mm0, mm7
	pavgb xmm2, xmm0
	pavgb xmm4, oword [0xac6]

%ifdef ERROR
	pavgb xmm9, xmm8
%endif
