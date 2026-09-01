	andnpd xmm1, oword [0x780]
	andnpd xmm2, oword [0xaef]

%ifdef ERROR
	andnpd xmm15, xmm13
%endif
