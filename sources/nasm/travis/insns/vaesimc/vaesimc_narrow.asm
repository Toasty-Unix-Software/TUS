	vaesimc xmm3, oword [0x5a6]
	vaesimc xmm3, oword [0x419]

%ifdef ERROR
	vaesimc xmm11, xmm12
%endif
