	psignb mm0, mm0
	psignb mm2, qword [0x6cf]
	psignb xmm4, xmm7
	psignb xmm2, oword [0xd35]

%ifdef ERROR
	psignb xmm11, xmm13
%endif
