default rel
	vfnmsub213ps xmm1, xmm5, xmm0
	vfnmsub213ps xmm4, xmm0, xmm6
	vfnmsub213ps ymm7, ymm6, yword [0xf76]
	vfnmsub213ps ymm4, ymm1, ymm5
	vfnmsub213ps xmm7, xmm1, oword [0x962]
	vfnmsub213ps xmm2, xmm4, xmm5
	vfnmsub213ps ymm6, ymm1, ymm7
	vfnmsub213ps ymm0, ymm0, ymm5
	vfnmsub213ps xmm14, xmm11, xmm12
	vfnmsub213ps ymm15, ymm11, ymm13
	vfnmsub213ps xmm13, xmm11, xmm8
	vfnmsub213ps ymm11, ymm15, ymm12
	vfnmsub213ps xmm31, xmm27, xmm24
	vfnmsub213ps ymm23, ymm30, ymm21
	vfnmsub213ps xmm30, xmm23, xmm21
	vfnmsub213ps ymm16, ymm16, ymm23
	vfnmsub213ps xmm2{k4}, xmm0, xmm3
	vfnmsub213ps ymm3{k3}, ymm1, ymm0
	vfnmsub213ps zmm2{k4}, zmm3, zmm7
	vfnmsub213ps xmm4{k1}{z}, xmm6, oword [0x140]
	vfnmsub213ps ymm2{k6}{z}, ymm7, ymm2
	vfnmsub213ps zmm0{k6}{z}, zmm5, zmm1
	vfnmsub213ps zmm7, zmm0, zmm7, {rd-sae}
	vfnmsub213ps xmm1, xmm7, oword [eax+1]
	vfnmsub213ps xmm3, xmm4, oword [eax+64]
	vfnmsub213ps ymm5, ymm6, yword [eax+1]
	vfnmsub213ps ymm1, ymm7, yword [eax+64]
	vfnmsub213ps xmm1, xmm2, oword [eax+1]
	vfnmsub213ps xmm5, xmm4, oword [eax+64]
	vfnmsub213ps ymm2, ymm5, yword [eax+1]
	vfnmsub213ps ymm2, ymm5, yword [eax+64]
	vfnmsub213ps zmm6, zmm2, zword [eax+1]
	vfnmsub213ps zmm7, zmm1, zword [eax+64]
	vfnmsub213ps xmm2, xmm3, [0xd1a]
	vfnmsub213ps ymm7, ymm3, [0x5f9]
	vfnmsub213ps xmm4, xmm3, [0xab2]
	vfnmsub213ps ymm7, ymm5, [0x5fa]
	vfnmsub213ps zmm0, zmm7, [0x823]
