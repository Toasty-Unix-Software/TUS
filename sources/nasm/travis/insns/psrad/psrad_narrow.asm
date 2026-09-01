	psrad mm3, qword [0x228]
	psrad mm6, mm2
	psrad mm2, 0x7f
	psrad mm0, 0x4
	psrad xmm5, oword [0x9fa]
	psrad xmm7, xmm4
	psrad xmm2, 0x43
	psrad xmm6, 0x34

%ifdef ERROR
	psrad xmm12, xmm8
	psrad xmm12, 0x33
%endif
