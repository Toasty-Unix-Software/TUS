	vdpphps xmm4, xmm6, oword [0x9f4]
	vdpphps xmm6, xmm4, xmm3
	vdpphps ymm0, ymm6, yword [0x70b]
	vdpphps ymm5, ymm1, ymm7
	vdpphps zmm2, zmm2, zmm6
	vdpphps zmm1, zmm2, zmm7

%ifdef ERROR
	vdpphps xmm10, xmm10, xmm14
	vdpphps ymm13, ymm10, ymm11
	vdpphps zmm12, zmm9, zmm8
	vdpphps xmm28, xmm25, xmm28
	vdpphps ymm18, ymm21, ymm18
	vdpphps zmm21, zmm29, zmm22
%endif
