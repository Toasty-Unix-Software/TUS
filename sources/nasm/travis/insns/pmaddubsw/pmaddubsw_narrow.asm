	pmaddubsw mm3, qword [0x365]
	pmaddubsw mm2, mm1
	pmaddubsw xmm7, oword [0x9a7]
	pmaddubsw xmm3, xmm5

%ifdef ERROR
	pmaddubsw xmm9, xmm15
%endif
