	sha1msg2 xmm6, oword [0xcd9]
	sha1msg2 xmm6, xmm2

%ifdef ERROR
	sha1msg2 xmm15, xmm14
%endif
