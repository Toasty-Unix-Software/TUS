default rel
	aesdec256kl xmm3, zword [0xef2]
	aesdec256kl xmm4, zword [0xff0]
	aesdec256kl xmm13, zword [0x67d]
	aesdec256kl xmm7, zword [eax+1]
	aesdec256kl xmm4, zword [eax+64]
	aesdec256kl xmm7, [0xd4c]
