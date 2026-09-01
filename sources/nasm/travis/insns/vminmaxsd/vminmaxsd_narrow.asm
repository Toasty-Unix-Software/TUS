	vminmaxsd xmm0, xmm3, qword [0xf58], 0x91
	vminmaxsd xmm0, xmm6, qword [0x41e], 0xbf

%ifdef ERROR
	vminmaxsd xmm8, xmm9, xmm9, 0x39
	vminmaxsd xmm19, xmm23, xmm30, 0x68
%endif
