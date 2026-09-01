default rel
	vpord xmm2, xmm2, xmm0
	vpord xmm2, xmm0
	vpord xmm0, xmm3, xmm1
	vpord ymm6, ymm1, ymm6
	vpord ymm6, ymm6
	vpord ymm6, ymm2, ymm6
	vpord zmm5, zmm0, zmm4
	vpord zmm5, zmm4
	vpord zmm7, zmm1, zmm6
	vpord xmm8, xmm15, xmm11
	vpord ymm15, ymm11, ymm12
	vpord zmm15, zmm12, zmm10
	vpord xmm28, xmm26, xmm21
	vpord ymm26, ymm16, ymm25
	vpord zmm21, zmm17, zmm27
	vpord xmm7{k6}, xmm2, oword [0x906]
	vpord ymm0{k2}, ymm1, ymm4
	vpord zmm2{k6}, zmm7, zmm0
	vpord xmm2{k1}{z}, xmm3, xmm1
	vpord ymm3{k3}{z}, ymm4, yword [0xe20]
	vpord zmm4{k1}{z}, zmm6, zmm6
	vpord xmm1, xmm0, oword [eax+1]
	vpord xmm0, xmm3, oword [eax+64]
	vpord ymm7, ymm0, yword [eax+1]
	vpord ymm2, ymm3, yword [eax+64]
	vpord zmm5, zmm3, zword [eax+1]
	vpord zmm5, zmm5, zword [eax+64]
	vpord xmm0, xmm0, [0xf1a]
	vpord ymm7, ymm6, [0xa41]
	vpord zmm6, zmm6, [0xef5]
