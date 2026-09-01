	vfmaddsub213pd xmm4, xmm4, xmm4
	vfmaddsub213pd xmm4, xmm4, xmm2
	vfmaddsub213pd ymm5, ymm5, ymm1
	vfmaddsub213pd ymm0, ymm7, yword [0x2e2]
	vfmaddsub213pd xmm2, xmm0, oword [0xbbc]
	vfmaddsub213pd xmm1, xmm7, xmm6
	vfmaddsub213pd ymm6, ymm3, yword [0xc9e]
	vfmaddsub213pd ymm7, ymm0, ymm6

%ifdef ERROR
	vfmaddsub213pd xmm10, xmm11, xmm13
	vfmaddsub213pd ymm9, ymm12, ymm9
	vfmaddsub213pd xmm13, xmm15, xmm10
	vfmaddsub213pd ymm15, ymm9, ymm12
	vfmaddsub213pd xmm31, xmm27, xmm19
	vfmaddsub213pd ymm17, ymm23, ymm21
	vfmaddsub213pd xmm28, xmm30, xmm29
	vfmaddsub213pd ymm16, ymm23, ymm17
%endif
