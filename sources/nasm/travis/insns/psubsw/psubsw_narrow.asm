	psubsw mm0, mm7
	psubsw mm0, mm3
	psubsw xmm2, oword [0xd52]
	psubsw xmm4, oword [0x8c2]

%ifdef ERROR
	psubsw xmm9, xmm12
%endif
