	vandps xmm3, xmm4, xmm2
	vandps xmm3, xmm2
	vandps xmm3, xmm0, oword [0x75f]
	vandps ymm5, ymm3, ymm5
	vandps ymm5, ymm5
	vandps ymm4, ymm0, ymm1
	vandps xmm4, xmm6, xmm6
	vandps xmm4, xmm6
	vandps xmm2, xmm6, xmm7
	vandps ymm2, ymm5, yword [0xd78]
	vandps ymm2, yword [0xd78]
	vandps ymm3, ymm3, ymm2

%ifdef ERROR
	vandps xmm14, xmm11, xmm15
	vandps ymm10, ymm9, ymm12
	vandps xmm9, xmm15, xmm14
	vandps ymm9, ymm11, ymm11
	vandps xmm31, xmm16, xmm24
	vandps ymm27, ymm28, ymm24
	vandps xmm17, xmm28, xmm26
	vandps ymm19, ymm29, ymm20
%endif
