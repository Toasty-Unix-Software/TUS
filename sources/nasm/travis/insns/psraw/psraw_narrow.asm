	psraw mm0, qword [0xef0]
	psraw mm0, mm6
	psraw mm6, 0x74
	psraw mm5, 0x65
	psraw xmm6, oword [0x775]
	psraw xmm7, xmm2
	psraw xmm1, 0x5f
	psraw xmm7, 0x21

%ifdef ERROR
	psraw xmm11, xmm14
	psraw xmm9, 0x46
%endif
