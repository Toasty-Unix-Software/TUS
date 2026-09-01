	aesenc256kl xmm6, zword [0x6ff]
	aesenc256kl xmm2, zword [0x11a]

%ifdef ERROR
	aesenc256kl xmm15, zword [0xf6e]
%endif
