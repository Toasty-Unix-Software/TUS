	bndcn bnd1, [0xd76]
	bndcn bnd0, [0x5ea]
	bndcn bnd2, esi
	bndcn bnd3, esi

%ifdef ERROR
	bndcn bnd1, rbx
	bndcn bnd0, rbx
%endif
