	aesimc xmm6, oword [0x810]
	aesimc xmm4, oword [0xa82]

%ifdef ERROR
	aesimc xmm15, xmm12
%endif
