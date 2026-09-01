	vgf2p8mulb xmm5, xmm1, xmm5
	vgf2p8mulb xmm5, xmm5
	vgf2p8mulb xmm6, xmm4, xmm2
	vgf2p8mulb ymm1, ymm5, yword [0x4e4]
	vgf2p8mulb ymm1, yword [0x4e4]
	vgf2p8mulb ymm2, ymm3, ymm4
	vgf2p8mulb xmm0, xmm4, xmm3
	vgf2p8mulb xmm0, xmm3
	vgf2p8mulb xmm3, xmm6, xmm5
	vgf2p8mulb ymm6, ymm0, ymm7
	vgf2p8mulb ymm6, ymm7
	vgf2p8mulb ymm5, ymm7, ymm1

%ifdef ERROR
	vgf2p8mulb xmm14, xmm9, xmm9
	vgf2p8mulb ymm13, ymm14, ymm9
	vgf2p8mulb xmm12, xmm13, xmm12
	vgf2p8mulb ymm14, ymm11, ymm9
	vgf2p8mulb xmm29, xmm24, xmm21
	vgf2p8mulb ymm29, ymm20, ymm20
	vgf2p8mulb xmm29, xmm28, xmm29
	vgf2p8mulb ymm21, ymm18, ymm18
%endif
