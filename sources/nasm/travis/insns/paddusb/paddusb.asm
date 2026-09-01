default rel
	paddusb mm4, qword [0xb3f]
	paddusb mm0, mm5
	paddusb xmm1, xmm1
	paddusb xmm5, oword [0xcfe]
	paddusb xmm9, xmm11
	paddusb mm1, qword [eax+1]
	paddusb mm3, qword [eax+64]
	paddusb xmm1, oword [eax+1]
	paddusb xmm6, oword [eax+64]
	paddusb mm7, [0x4de]
	paddusb xmm7, [0xd23]
