from __future__ import division
import matplotlib.pyplot as plt

uftd = []
ftd = []

def t2f(inp):
  fq = []
  for val in inp:
    fq.append(1/(val/1000000))
  return fq

def mafilter(inp):
  fbuflen = 210
  fbuf = [0] * fbuflen
  ftdbuf = [] 
  i = 0
  for val in inp:  
    fbuf[i%fbuflen] = int(val)
    i += 1
    average = sum(fbuf)/fbuflen
    ftdbuf.append(average)
  return ftdbuf

with open("log_ftd.txt", 'r') as f:
  lines = f.readlines()
  for line in lines:
    if len(line.split(",")) == 2:
      uftd.append(int(line.split(",")[0]))
      ftd.append(int(line.split(",")[1]))

#print uftd
#print ftd 

#plt.plot(mafilter(uftd), label='py_ftd')
#plt.plot(uftd, label='uftd')
#plt.plot(ftd, label='ard_ftd')

plt.plot(t2f(uftd), label='uftd_fq')
plt.plot(t2f(ftd), label='ard_ftd_fq')

plt.legend()
plt.show()
