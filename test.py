CPI = 0.8
BCPI = 0.2
MR = 0.07
SPC = 0.004

INSNS = 10_000_000_000

ET = INSNS * CPI * SPC
print(ET)

ET_BM = INSNS * (CPI - (CPI * BCPI * MR)) * SPC
print(ET_BM)

print(ET_BM / ET)