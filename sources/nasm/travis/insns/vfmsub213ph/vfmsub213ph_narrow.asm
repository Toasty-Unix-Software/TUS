	vfmsub213ph xmm5, xmm4, xmm5
	vfmsub213ph xmm5, xmm5
	vfmsub213ph xmm0, xmm7, xmm5
	vfmsub213ph ymm5, ymm7, ymm2
	vfmsub213ph ymm5, ymm2
	vfmsub213ph ymm2, ymm5, yword [0xa62]
	vfmsub213ph zmm3, zmm1, zmm6
	vfmsub213ph zmm3, zmm6
	vfmsub213ph zmm1, zmm3, zword [0x5ca]

%ifdef ERROR
	vfmsub213ph xmm10, xmm10, xmm12
	vfmsub213ph ymm15, ymm15, ymm14
	vfmsub213ph zmm14, zmm13, zmm10
	vfmsub213ph xmm28, xmm22, xmm30
	vfmsub213ph ymm29, ymm23, ymm31
	vfmsub213ph zmm18, zmm21, zmm26
%endif
