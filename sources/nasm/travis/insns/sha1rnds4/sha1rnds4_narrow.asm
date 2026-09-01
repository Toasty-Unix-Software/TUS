	sha1rnds4 xmm4, xmm3, 0x3e
	sha1rnds4 xmm0, xmm4, 0xe5

%ifdef ERROR
	sha1rnds4 xmm12, xmm13, 0x87
%endif
