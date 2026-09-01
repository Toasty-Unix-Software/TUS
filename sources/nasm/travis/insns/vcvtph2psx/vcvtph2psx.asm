default rel
	vcvtph2psx xmm2, qword [0x3d6]
	vcvtph2psx xmm7, xmm2
	vcvtph2psx ymm2, oword [0xe0c]
	vcvtph2psx ymm1, xmm5
	vcvtph2psx zmm6, ymm0
	vcvtph2psx zmm3, ymm7
	vcvtph2psx xmm9, xmm14
	vcvtph2psx ymm13, xmm15
	vcvtph2psx zmm15, ymm12
	vcvtph2psx xmm29, xmm29
	vcvtph2psx ymm31, xmm16
	vcvtph2psx zmm28, ymm28
	vcvtph2psx xmm3{k3}, xmm1
	vcvtph2psx ymm4{k1}, xmm7
	vcvtph2psx zmm4{k6}, ymm6
	vcvtph2psx xmm1{k6}{z}, qword [0xca5]
	vcvtph2psx ymm7{k2}{z}, xmm2
	vcvtph2psx zmm6{k1}{z}, ymm7
	vcvtph2psx zmm0, ymm1, {sae}
	vcvtph2psx xmm5, qword [eax+1]
	vcvtph2psx xmm1, qword [eax+64]
	vcvtph2psx ymm3, oword [eax+1]
	vcvtph2psx ymm1, oword [eax+64]
	vcvtph2psx zmm0, yword [eax+1]
	vcvtph2psx zmm6, yword [eax+64]
	vcvtph2psx xmm4, [0x6d3]
	vcvtph2psx ymm3, [0x739]
	vcvtph2psx zmm6, [0xf26]
