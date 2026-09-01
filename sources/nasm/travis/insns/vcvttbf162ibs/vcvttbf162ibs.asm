default rel
	vcvttbf162ibs xmm0, xmm4
	vcvttbf162ibs xmm6, oword [0x14a]
	vcvttbf162ibs ymm6, ymm5
	vcvttbf162ibs ymm3, ymm0
	vcvttbf162ibs zmm5, zmm2
	vcvttbf162ibs zmm0, zmm3
	vcvttbf162ibs xmm15, xmm15
	vcvttbf162ibs ymm9, ymm10
	vcvttbf162ibs zmm8, zmm12
	vcvttbf162ibs xmm17, xmm21
	vcvttbf162ibs ymm18, ymm19
	vcvttbf162ibs zmm29, zmm29
	vcvttbf162ibs xmm1{k3}, xmm7
	vcvttbf162ibs ymm4{k3}, ymm7
	vcvttbf162ibs zmm2{k6}, zword [0x3bb]
	vcvttbf162ibs xmm7{k2}{z}, xmm3
	vcvttbf162ibs ymm2{k7}{z}, ymm5
	vcvttbf162ibs zmm6{k7}{z}, zword [0x921]
	vcvttbf162ibs xmm1, oword [eax+1]
	vcvttbf162ibs xmm3, oword [eax+64]
	vcvttbf162ibs ymm0, yword [eax+1]
	vcvttbf162ibs ymm4, yword [eax+64]
	vcvttbf162ibs zmm2, zword [eax+1]
	vcvttbf162ibs zmm1, zword [eax+64]
	vcvttbf162ibs xmm6, [0xfea]
	vcvttbf162ibs ymm1, [0x69f]
	vcvttbf162ibs zmm3, [0xca8]
