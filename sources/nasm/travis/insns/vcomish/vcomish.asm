default rel
	vcomish xmm6, xmm0
	vcomish xmm5, word [0xd90]
	vcomish xmm10, xmm11
	vcomish xmm29, xmm28
	vcomish xmm5, xmm0, {sae}
	vcomish xmm2, word [eax+1]
	vcomish xmm5, word [eax+64]
	vcomish xmm5, [0x707]
