	vdpps xmm6, xmm6, xmm2, 0x14
	vdpps xmm6, xmm2, 0x14
	vdpps xmm0, xmm1, xmm6, 0x7e
	vdpps ymm6, ymm6, ymm7, 0xd2
	vdpps ymm6, ymm7, 0xd2
	vdpps ymm0, ymm2, yword [0x5f8], 0x58

%ifdef ERROR
	vdpps xmm11, xmm13, xmm15, 0x50
	vdpps ymm10, ymm12, ymm13, 0x5c
%endif
