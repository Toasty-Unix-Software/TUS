	aesdec256kl xmm3, zword [0xef2]
	aesdec256kl xmm4, zword [0xff0]

%ifdef ERROR
	aesdec256kl xmm13, zword [0x67d]
%endif
