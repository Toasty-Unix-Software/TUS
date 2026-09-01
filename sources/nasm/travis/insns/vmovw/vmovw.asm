default rel
	vmovw xmm0, bx
	vmovw xmm0, word [0xc4f]
	vmovw word [0x682], xmm3
	vmovw cx, xmm5
	vmovw xmm1, word [0xe69]
	vmovw xmm2, xmm5
	vmovw word [0x9f9], xmm1
	vmovw xmm4, xmm7
	vmovw xmm14, r11w
	vmovw r14w, xmm10
	vmovw xmm11, xmm8
	vmovw xmm11, xmm12
	vmovw xmm25, r17w
	vmovw r24w, xmm30
	vmovw xmm23, xmm31
	vmovw xmm22, xmm25
	vmovw xmm2{k5}, bx
	vmovw xmm6{k2}{z}, bx
	vmovw xmm7, word [eax+1]
	vmovw xmm1, word [eax+64]
	vmovw word [eax+1], xmm4
	vmovw word [eax+64], xmm1
	vmovw xmm7, word [eax+1]
	vmovw xmm6, word [eax+64]
	vmovw word [eax+1], xmm5
	vmovw word [eax+64], xmm0
	vmovw xmm1, [0x123]
	vmovw [0x2e3], xmm4
	vmovw xmm1, [0x2e1]
	vmovw [0x7fb], xmm2
