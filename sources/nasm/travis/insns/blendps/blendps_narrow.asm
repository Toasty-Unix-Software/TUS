	blendps xmm3, oword [0x610], 0x1f
	blendps xmm0, xmm6, 0xfc

%ifdef ERROR
	blendps xmm14, xmm8, 0xc
%endif
