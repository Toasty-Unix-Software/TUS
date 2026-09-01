	vcmppd xmm7, xmm1, xmm4, 0xcf
	vcmppd xmm7, xmm4, 0xcf
	vcmppd xmm0, xmm5, xmm1, 0x3c
	vcmppd ymm1, ymm2, ymm2, 0xa8
	vcmppd ymm1, ymm2, 0xa8
	vcmppd ymm5, ymm3, ymm5, 0xd9
	vcmppd k2, xmm1, xmm4, 0x5d
	vcmppd k1, xmm2, oword [0x253], 0x44
	vcmppd k1, ymm1, ymm5, 0x8
	vcmppd k6, ymm5, ymm7, 0xe

%ifdef ERROR
	vcmppd xmm14, xmm10, xmm11, 0xf4
	vcmppd ymm8, ymm8, ymm10, 0xb7
	vcmppd k7, xmm15, xmm9, 0xfa
	vcmppd k5, ymm13, ymm15, 0x8a
%endif
