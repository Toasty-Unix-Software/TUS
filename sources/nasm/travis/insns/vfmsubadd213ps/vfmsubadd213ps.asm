default rel
	vfmsubadd213ps xmm0, xmm1, oword [0x6f1]
	vfmsubadd213ps xmm3, xmm6, xmm7
	vfmsubadd213ps ymm1, ymm5, ymm2
	vfmsubadd213ps ymm0, ymm4, ymm7
	vfmsubadd213ps xmm1, xmm3, oword [0xaef]
	vfmsubadd213ps xmm6, xmm6, xmm1
	vfmsubadd213ps ymm6, ymm1, yword [0xdc6]
	vfmsubadd213ps ymm7, ymm6, ymm3
	vfmsubadd213ps xmm9, xmm8, xmm9
	vfmsubadd213ps ymm13, ymm10, ymm10
	vfmsubadd213ps xmm10, xmm8, xmm11
	vfmsubadd213ps ymm13, ymm8, ymm9
	vfmsubadd213ps xmm31, xmm18, xmm30
	vfmsubadd213ps ymm30, ymm28, ymm26
	vfmsubadd213ps xmm27, xmm21, xmm18
	vfmsubadd213ps ymm20, ymm28, ymm29
	vfmsubadd213ps xmm2{k7}, xmm3, xmm0
	vfmsubadd213ps ymm3{k5}, ymm6, ymm1
	vfmsubadd213ps zmm3{k2}, zmm4, zmm7
	vfmsubadd213ps xmm7{k6}{z}, xmm2, oword [0x667]
	vfmsubadd213ps ymm6{k5}{z}, ymm5, ymm5
	vfmsubadd213ps zmm6{k6}{z}, zmm1, zmm5
	vfmsubadd213ps zmm6, zmm0, zmm7, {rn-sae}
	vfmsubadd213ps xmm4, xmm2, oword [eax+1]
	vfmsubadd213ps xmm4, xmm1, oword [eax+64]
	vfmsubadd213ps ymm3, ymm6, yword [eax+1]
	vfmsubadd213ps ymm1, ymm4, yword [eax+64]
	vfmsubadd213ps xmm7, xmm5, oword [eax+1]
	vfmsubadd213ps xmm0, xmm7, oword [eax+64]
	vfmsubadd213ps ymm2, ymm1, yword [eax+1]
	vfmsubadd213ps ymm5, ymm7, yword [eax+64]
	vfmsubadd213ps zmm1, zmm0, zword [eax+1]
	vfmsubadd213ps zmm3, zmm5, zword [eax+64]
	vfmsubadd213ps xmm6, xmm4, [0x51f]
	vfmsubadd213ps ymm0, ymm4, [0x538]
	vfmsubadd213ps xmm5, xmm4, [0x2fb]
	vfmsubadd213ps ymm5, ymm3, [0xbb4]
	vfmsubadd213ps zmm6, zmm6, [0x28c]
