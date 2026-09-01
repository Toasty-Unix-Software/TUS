default rel
	psubusb mm6, qword [0x4a1]
	psubusb mm1, qword [0x917]
	psubusb xmm4, xmm3
	psubusb xmm7, oword [0xfb9]
	psubusb xmm9, xmm15
	psubusb mm0, qword [eax+1]
	psubusb mm4, qword [eax+64]
	psubusb xmm1, oword [eax+1]
	psubusb xmm5, oword [eax+64]
	psubusb mm0, [0xd10]
	psubusb xmm1, [0xb3b]
