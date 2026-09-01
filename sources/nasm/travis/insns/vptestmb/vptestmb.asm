default rel
	vptestmb k6, xmm1, oword [0xb8b]
	vptestmb k5, xmm5, xmm5
	vptestmb k4, ymm5, ymm3
	vptestmb k4, ymm1, ymm4
	vptestmb k4, zmm5, zword [0x3c4]
	vptestmb k7, zmm6, zmm0
	vptestmb k2, xmm8, xmm11
	vptestmb k6, ymm8, ymm14
	vptestmb k7, zmm11, zmm14
	vptestmb k3, xmm20, xmm21
	vptestmb k2, ymm18, ymm23
	vptestmb k5, zmm30, zmm26
	vptestmb k6{k3}, xmm6, xmm6
	vptestmb k5{k5}, ymm3, ymm2
	vptestmb k3{k2}, zmm0, zmm0
	vptestmb k4, xmm3, oword [eax+1]
	vptestmb k2, xmm1, oword [eax+64]
	vptestmb k7, ymm7, yword [eax+1]
	vptestmb k7, ymm5, yword [eax+64]
	vptestmb k1, zmm2, zword [eax+1]
	vptestmb k6, zmm5, zword [eax+64]
	vptestmb k5, xmm3, [0xf92]
	vptestmb k2, ymm5, [0x8c4]
	vptestmb k1, zmm6, [0x6c4]
