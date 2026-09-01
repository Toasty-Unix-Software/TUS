	v4fnmaddps zmm7, zmm3, [0xbfa]
	v4fnmaddps zmm3, zmm1, [0xfd1]

%ifdef ERROR
	v4fnmaddps zmm11, zmm13, [0xd86]
	v4fnmaddps zmm16, zmm24, [0xdca]
%endif
