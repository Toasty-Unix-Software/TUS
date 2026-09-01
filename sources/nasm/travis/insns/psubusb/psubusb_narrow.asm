	psubusb mm6, qword [0x4a1]
	psubusb mm1, qword [0x917]
	psubusb xmm4, xmm3
	psubusb xmm7, oword [0xfb9]

%ifdef ERROR
	psubusb xmm9, xmm15
%endif
