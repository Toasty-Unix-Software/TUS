	vgetmantpd xmm3, xmm3, 0x35
	vgetmantpd xmm4, xmm4, 0x72
	vgetmantpd ymm3, ymm5, 0xef
	vgetmantpd ymm1, yword [0xf86], 0xa0
	vgetmantpd zmm4, zmm3, 0xd7
	vgetmantpd zmm1, zmm3, 0x25

%ifdef ERROR
	vgetmantpd xmm8, xmm8, 0x59
	vgetmantpd ymm13, ymm11, 0xd
	vgetmantpd zmm11, zmm15, 0xc1
	vgetmantpd xmm22, xmm18, 0x34
	vgetmantpd ymm16, ymm29, 0x79
	vgetmantpd zmm21, zmm22, 0xb5
%endif
