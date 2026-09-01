	andnps xmm3, xmm7
	andnps xmm3, oword [0x590]

%ifdef ERROR
	andnps xmm11, xmm11
%endif
