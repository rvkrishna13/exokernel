mfc0 kO, cO_cause
nop
and k1, k0, ~(8<<2) & 255
beq k1, zero, sys_or_not

lui k1, HI(PSW_ENV)
lw k1, LO(PSW_ENV)(k1)

and k0, k0, 254
add k0, k1, k0
lw k1, GENX_SAVE_AREA(k1)
lw k0, ENV_XH(k0)

sw a0, A0_SAVE(k1)
sw a1, A1_SAVE(k1)
sw a2, A2_SAVE(k1)

mfc0 a0, c0_epc
mfc0 a1, c0_tlbbad
mfc0 a2, c0_cause

j k0
rfe