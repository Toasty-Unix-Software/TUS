default rel
	vdpbf16ps xmm3, xmm1, xmm3
	vdpbf16ps xmm3, xmm3
	vdpbf16ps xmm2, xmm7, xmm4
	vdpbf16ps ymm4, ymm4, ymm4
	vdpbf16ps ymm4, ymm4
	vdpbf16ps ymm6, ymm6, ymm0
	vdpbf16ps zmm2, zmm7, zword [0xd70]
	vdpbf16ps zmm2, zword [0xd70]
	vdpbf16ps zmm2, zmm2, zword [0xebe]
	vdpbf16ps xmm15, xmm10, xmm15
	vdpbf16ps ymm15, ymm9, ymm8
	vdpbf16ps zmm10, zmm9, zmm8
	vdpbf16ps xmm24, xmm26, xmm25
	vdpbf16ps ymm17, ymm28, ymm19
	vdpbf16ps zmm18, zmm16, zmm23
	vdpbf16ps xmm4{k6}, xmm3, xmm4
	vdpbf16ps ymm7{k1}, ymm0, ymm3
	vdpbf16ps zmm0{k5}, zmm1, zmm5
	vdpbf16ps xmm1{k2}{z}, xmm5, xmm2
	vdpbf16ps ymm7{k5}{z}, ymm2, ymm5
	vdpbf16ps zmm3{k6}{z}, zmm0, zmm6
	vdpbf16ps xmm2, xmm4, oword [eax+1]
	vdpbf16ps xmm2, xmm0, oword [eax+64]
	vdpbf16ps ymm7, ymm3, yword [eax+1]
	vdpbf16ps ymm5, ymm2, yword [eax+64]
	vdpbf16ps zmm3, zmm1, zword [eax+1]
	vdpbf16ps zmm5, zmm3, zword [eax+64]
	vdpbf16ps xmm7, xmm7, [0x3f5]
	vdpbf16ps ymm6, ymm7, [0x9e3]
	vdpbf16ps zmm0, zmm2, [0x5fd]
