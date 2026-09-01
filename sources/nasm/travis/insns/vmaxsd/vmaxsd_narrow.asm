	vmaxsd xmm2, xmm6, xmm1
	vmaxsd xmm2, xmm1
	vmaxsd xmm4, xmm1, qword [0xe4e]
	vmaxsd xmm3, xmm6, qword [0x65c]
	vmaxsd xmm3, qword [0x65c]
	vmaxsd xmm1, xmm4, qword [0xe8a]

%ifdef ERROR
	vmaxsd xmm14, xmm14, xmm8
	vmaxsd xmm9, xmm14, xmm9
	vmaxsd xmm23, xmm27, xmm19
	vmaxsd xmm29, xmm16, xmm28
%endif
