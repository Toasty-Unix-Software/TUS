	vrcp14sd xmm3, xmm3, qword [0x9f2]
	vrcp14sd xmm3, qword [0x9f2]
	vrcp14sd xmm2, xmm3, qword [0x1e0]

%ifdef ERROR
	vrcp14sd xmm10, xmm8, xmm15
	vrcp14sd xmm22, xmm23, xmm20
%endif
