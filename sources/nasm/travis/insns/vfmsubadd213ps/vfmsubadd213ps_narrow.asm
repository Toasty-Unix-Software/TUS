	vfmsubadd213ps xmm0, xmm1, oword [0x6f1]
	vfmsubadd213ps xmm3, xmm6, xmm7
	vfmsubadd213ps ymm1, ymm5, ymm2
	vfmsubadd213ps ymm0, ymm4, ymm7
	vfmsubadd213ps xmm1, xmm3, oword [0xaef]
	vfmsubadd213ps xmm6, xmm6, xmm1
	vfmsubadd213ps ymm6, ymm1, yword [0xdc6]
	vfmsubadd213ps ymm7, ymm6, ymm3

%ifdef ERROR
	vfmsubadd213ps xmm9, xmm8, xmm9
	vfmsubadd213ps ymm13, ymm10, ymm10
	vfmsubadd213ps xmm10, xmm8, xmm11
	vfmsubadd213ps ymm13, ymm8, ymm9
	vfmsubadd213ps xmm31, xmm18, xmm30
	vfmsubadd213ps ymm30, ymm28, ymm26
	vfmsubadd213ps xmm27, xmm21, xmm18
	vfmsubadd213ps ymm20, ymm28, ymm29
%endif
