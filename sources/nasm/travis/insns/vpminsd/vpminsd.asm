default rel
	vpminsd xmm1, xmm2, xmm6
	vpminsd xmm1, xmm6
	vpminsd xmm4, xmm4, xmm5
	vpminsd ymm3, ymm2, ymm5
	vpminsd ymm3, ymm5
	vpminsd ymm1, ymm0, yword [0x9bf]
	vpminsd xmm3, xmm1, oword [0x29c]
	vpminsd xmm3, oword [0x29c]
	vpminsd xmm0, xmm1, xmm6
	vpminsd ymm5, ymm5, ymm2
	vpminsd ymm5, ymm2
	vpminsd ymm6, ymm5, ymm5
	vpminsd xmm13, xmm13, xmm15
	vpminsd ymm15, ymm9, ymm10
	vpminsd xmm12, xmm13, xmm13
	vpminsd ymm11, ymm12, ymm11
	vpminsd xmm27, xmm17, xmm16
	vpminsd ymm21, ymm21, ymm25
	vpminsd xmm17, xmm30, xmm19
	vpminsd ymm18, ymm25, ymm24
	vpminsd xmm2{k2}, xmm3, xmm5
	vpminsd ymm2{k1}, ymm1, ymm3
	vpminsd zmm3{k2}, zmm1, zword [0x2a8]
	vpminsd xmm3{k7}{z}, xmm0, oword [0xfe4]
	vpminsd ymm1{k3}{z}, ymm2, ymm2
	vpminsd zmm3{k1}{z}, zmm2, zmm6
	vpminsd xmm6, xmm7, oword [eax+1]
	vpminsd xmm7, xmm7, oword [eax+64]
	vpminsd ymm7, ymm3, yword [eax+1]
	vpminsd ymm5, ymm6, yword [eax+64]
	vpminsd xmm7, xmm4, oword [eax+1]
	vpminsd xmm5, xmm2, oword [eax+64]
	vpminsd ymm1, ymm7, yword [eax+1]
	vpminsd ymm3, ymm6, yword [eax+64]
	vpminsd zmm7, zmm4, zword [eax+1]
	vpminsd zmm2, zmm0, zword [eax+64]
	vpminsd xmm1, xmm6, [0x68b]
	vpminsd ymm2, ymm6, [0x68c]
	vpminsd xmm6, xmm7, [0x1bc]
	vpminsd ymm1, ymm1, [0x8e8]
	vpminsd zmm3, zmm2, [0xd22]
