	vcmpps xmm0, xmm1, xmm1, 0x9b
	vcmpps xmm0, xmm1, 0x9b
	vcmpps xmm2, xmm1, xmm7, 0xd4
	vcmpps ymm4, ymm5, ymm5, 0xc3
	vcmpps ymm4, ymm5, 0xc3
	vcmpps ymm7, ymm4, ymm0, 0x32
	vcmpps k3, xmm3, xmm7, 0x86
	vcmpps k6, xmm3, oword [0xc8d], 0x8
	vcmpps k7, ymm0, ymm2, 0xc9
	vcmpps k5, ymm5, ymm5, 0xe7

%ifdef ERROR
	vcmpps xmm12, xmm12, xmm15, 0xfa
	vcmpps ymm8, ymm12, ymm8, 0x47
	vcmpps k3, xmm13, xmm13, 0x4b
	vcmpps k1, ymm13, ymm15, 0x27
%endif
