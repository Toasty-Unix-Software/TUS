	vfmsubadd213ph xmm4, xmm7, xmm4
	vfmsubadd213ph xmm4, xmm4
	vfmsubadd213ph xmm4, xmm0, xmm3
	vfmsubadd213ph ymm6, ymm0, yword [0x166]
	vfmsubadd213ph ymm6, yword [0x166]
	vfmsubadd213ph ymm3, ymm2, yword [0xd35]
	vfmsubadd213ph zmm6, zmm4, zmm7
	vfmsubadd213ph zmm6, zmm7
	vfmsubadd213ph zmm5, zmm3, zword [0x621]

%ifdef ERROR
	vfmsubadd213ph xmm10, xmm10, xmm8
	vfmsubadd213ph ymm11, ymm11, ymm14
	vfmsubadd213ph zmm14, zmm10, zmm9
	vfmsubadd213ph xmm19, xmm23, xmm29
	vfmsubadd213ph ymm24, ymm31, ymm26
	vfmsubadd213ph zmm18, zmm20, zmm24
%endif
