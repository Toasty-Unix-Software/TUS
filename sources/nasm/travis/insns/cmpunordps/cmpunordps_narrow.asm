	cmpunordps xmm5, oword [0xc29]
	cmpunordps xmm2, oword [0x57c]

%ifdef ERROR
	cmpunordps xmm10, xmm10
%endif
