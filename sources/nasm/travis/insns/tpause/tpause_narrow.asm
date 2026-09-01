	tpause eax
	tpause esi
	tpause ecx, edx, eax
	tpause edi, edx, eax

%ifdef ERROR
	tpause r8d
	tpause r9d, edx, eax
	tpause r25d
	tpause r30d, edx, eax
%endif
