	vinsertps xmm2, xmm2, xmm4, 0xda
	vinsertps xmm2, xmm4, 0xda
	vinsertps xmm7, xmm7, xmm4, 0x3c

%ifdef ERROR
	vinsertps xmm10, xmm12, xmm8, 0x70
	vinsertps xmm16, xmm18, xmm19, 0xde
%endif
