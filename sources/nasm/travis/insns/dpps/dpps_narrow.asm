	dpps xmm6, oword [0xdff], 0xe0
	dpps xmm7, oword [0xe49], 0xbe

%ifdef ERROR
	dpps xmm10, xmm14, 0xd9
%endif
