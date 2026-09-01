	vmovntpd oword [0x5d4], xmm1
	vmovntpd oword [0x191], xmm4
	vmovntpd yword [0x6e8], ymm4
	vmovntpd yword [0x81a], ymm3
	vmovntpd zword [0xbab], zmm0
	vmovntpd zword [0xf8b], zmm6

%ifdef ERROR
	vmovntpd oword [0xd62], xmm15
	vmovntpd yword [0x9eb], ymm14
	vmovntpd zword [0xd38], zmm9
	vmovntpd oword [0x76e], xmm27
	vmovntpd yword [0x9a4], ymm27
	vmovntpd zword [0x816], zmm22
%endif
