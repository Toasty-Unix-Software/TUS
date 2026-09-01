default rel
	vfnmsub213ph xmm5, xmm4, oword [0xddf]
	vfnmsub213ph xmm5, oword [0xddf]
	vfnmsub213ph xmm7, xmm4, oword [0xdad]
	vfnmsub213ph ymm5, ymm3, ymm5
	vfnmsub213ph ymm5, ymm5
	vfnmsub213ph ymm7, ymm5, ymm0
	vfnmsub213ph zmm0, zmm2, zmm2
	vfnmsub213ph zmm0, zmm2
	vfnmsub213ph zmm4, zmm0, zword [0x337]
	vfnmsub213ph xmm9, xmm9, xmm15
	vfnmsub213ph ymm12, ymm10, ymm12
	vfnmsub213ph zmm10, zmm12, zmm9
	vfnmsub213ph xmm19, xmm25, xmm20
	vfnmsub213ph ymm23, ymm27, ymm21
	vfnmsub213ph zmm31, zmm17, zmm18
	vfnmsub213ph xmm7{k2}, xmm7, oword [0x859]
	vfnmsub213ph ymm6{k4}, ymm0, yword [0x622]
	vfnmsub213ph zmm0{k3}, zmm7, zmm3
	vfnmsub213ph xmm4{k2}{z}, xmm6, oword [0x3a3]
	vfnmsub213ph ymm1{k6}{z}, ymm7, ymm7
	vfnmsub213ph zmm1{k5}{z}, zmm2, zmm2
	vfnmsub213ph zmm3, zmm1, zmm7, {ru-sae}
	vfnmsub213ph xmm5, xmm3, oword [eax+1]
	vfnmsub213ph xmm5, xmm7, oword [eax+64]
	vfnmsub213ph ymm0, ymm2, yword [eax+1]
	vfnmsub213ph ymm2, ymm7, yword [eax+64]
	vfnmsub213ph zmm7, zmm5, zword [eax+1]
	vfnmsub213ph zmm3, zmm6, zword [eax+64]
	vfnmsub213ph xmm5, xmm0, [0x2a5]
	vfnmsub213ph ymm4, ymm6, [0x8fa]
	vfnmsub213ph zmm7, zmm0, [0xda3]
