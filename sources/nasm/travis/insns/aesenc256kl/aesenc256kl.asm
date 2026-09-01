default rel
	aesenc256kl xmm6, zword [0x6ff]
	aesenc256kl xmm2, zword [0x11a]
	aesenc256kl xmm15, zword [0xf6e]
	aesenc256kl xmm4, zword [eax+1]
	aesenc256kl xmm5, zword [eax+64]
	aesenc256kl xmm2, [0xe30]
