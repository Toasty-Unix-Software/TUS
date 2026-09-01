	phaddw mm7, qword [0x5e5]
	phaddw mm0, qword [0x32a]
	phaddw xmm4, xmm0
	phaddw xmm2, oword [0x59d]

%ifdef ERROR
	phaddw xmm8, xmm12
%endif
