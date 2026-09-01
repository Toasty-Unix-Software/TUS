	vbroadcastsd ymm1, qword [0x3c5]
	vbroadcastsd ymm3, qword [0x7c0]
	vbroadcastsd ymm0, xmm5
	vbroadcastsd ymm7, xmm0
	vbroadcastsd ymm6, qword [0xa9d]
	vbroadcastsd ymm0, qword [0xa4a]
	vbroadcastsd zmm4, qword [0x596]
	vbroadcastsd zmm1, qword [0x184]

%ifdef ERROR
	vbroadcastsd ymm14, qword [0x325]
	vbroadcastsd ymm15, xmm12
	vbroadcastsd ymm10, qword [0x3d2]
	vbroadcastsd zmm9, qword [0x568]
	vbroadcastsd ymm17, qword [0x925]
	vbroadcastsd ymm21, xmm30
	vbroadcastsd ymm29, qword [0x818]
	vbroadcastsd zmm16, qword [0xc55]
%endif
