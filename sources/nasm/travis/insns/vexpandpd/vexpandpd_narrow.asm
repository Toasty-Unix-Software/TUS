	vexpandpd xmm0, oword [0x92b]
	vexpandpd xmm5, oword [0xedb]
	vexpandpd ymm6, yword [0x778]
	vexpandpd ymm2, yword [0x728]
	vexpandpd zmm5, zword [0x538]
	vexpandpd zmm0, zword [0x677]
	vexpandpd xmm1, xmm2
	vexpandpd xmm6, xmm0

%ifdef ERROR
	vexpandpd xmm11, oword [0x51f]
	vexpandpd ymm13, yword [0xeae]
	vexpandpd zmm13, zword [0xd75]
	vexpandpd xmm10, xmm11
	vexpandpd xmm30, oword [0x2c2]
	vexpandpd ymm23, yword [0xb8a]
	vexpandpd zmm25, zword [0xd92]
	vexpandpd xmm17, xmm18
%endif
