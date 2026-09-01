	psllw mm3, mm2
	psllw mm2, qword [0xf84]
	psllw mm2, 0x5a
	psllw mm5, 0x3c
	psllw xmm7, xmm7
	psllw xmm1, oword [0x11f]
	psllw xmm7, 0x7
	psllw xmm5, 0x5b

%ifdef ERROR
	psllw xmm13, xmm14
	psllw xmm9, 0x78
%endif
