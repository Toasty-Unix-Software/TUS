	roundps xmm0, xmm4, 0x8a
	roundps xmm7, oword [0x3bf], 0xc4

%ifdef ERROR
	roundps xmm14, xmm12, 0xe2
%endif
