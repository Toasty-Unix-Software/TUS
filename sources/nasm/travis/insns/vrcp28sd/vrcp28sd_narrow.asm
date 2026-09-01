	vrcp28sd xmm3, xmm4, qword [0x7c2]
	vrcp28sd xmm3, qword [0x7c2]
	vrcp28sd xmm6, xmm2, qword [0x675]

%ifdef ERROR
	vrcp28sd xmm10, xmm12, xmm15
	vrcp28sd xmm31, xmm27, xmm31
%endif
