	vucomxsd xmm1, xmm3
	vucomxsd xmm4, qword [0x38f]

%ifdef ERROR
	vucomxsd xmm15, xmm13
	vucomxsd xmm16, xmm22
%endif
