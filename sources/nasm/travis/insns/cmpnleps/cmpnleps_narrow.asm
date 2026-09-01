	cmpnleps xmm6, xmm4
	cmpnleps xmm4, oword [0x65d]

%ifdef ERROR
	cmpnleps xmm12, xmm9
%endif
