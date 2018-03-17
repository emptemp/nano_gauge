from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
from math import factorial

def savitzky_golay(y, window_size, order, deriv=0, rate=1):
  try:
      window_size = np.abs(np.int(window_size))
      order = np.abs(np.int(order))
  except ValueError, msg:
      raise ValueError("window_size and order have to be of type int")
  if window_size % 2 != 1 or window_size < 1:
      raise TypeError("window_size size must be a positive odd number")
  if window_size < order + 2:
      raise TypeError("window_size is too small for the polynomials order")
  order_range = range(order+1)
  half_window = (window_size -1) // 2
  # precompute coefficients
  b = np.mat([[k**i for i in order_range] for k in range(-half_window, half_window+1)])
  m = np.linalg.pinv(b).A[deriv] * rate**deriv * factorial(deriv)
  # pad the signal at the extremes with
  # values taken from the signal itself
  #firstvals = y[0] - np.abs( y[1:half_window+1][::-1] - y[0] )
  #lastvals = y[-1] + np.abs(y[-half_window-1:-1][::-1] - y[-1])
  #y = np.concatenate((firstvals, y, lastvals))
  return np.convolve( m[::-1], y, mode='valid')

def t2f(inp):
  fq = []
  for val in inp:
    fq.append(1/(val/1000000))
  return fq

def mafilter(inp):
  fbuflen = 50
  fbuf = [0] * fbuflen
  ftdbuf = [] 
  i = 0
  for val in inp:  
    fbuf[i%fbuflen] = int(val)
    i += 1
    average = sum(fbuf)/fbuflen
    ftdbuf.append(average)
  return ftdbuf

def openlog(fd):
  uftd = []
  ftd = []
  with open(fd, 'r') as f:
    lines = f.readlines()
    for line in lines:
      if len(line.split(",")) == 2:
        uftd.append(int(line.split(",")[0]))
        ftd.append(int(line.split(",")[1]))
  return uftd, ftd

#print uftd
#print ftd 

uftd = openlog('log_ftd_50.txt')[0]


plt.plot(openlog('log_ftd_50.txt')[0], label='uftd_50')
plt.plot(openlog('log_ftd_50.txt')[1], label='ftd_50')
#plt.plot(mafilter(openlog('log_ftd_50.txt')[0]), label='py_ftd')
plt.plot(savitzky_golay(uftd, 123,2), label = 'savitzky_golay')

#plt.plot(openlog('log_ftd_100.txt')[0], label='uftd_100')
#plt.plot(openlog('log_ftd_100.txt')[1], label='ftd_100')

#plt.plot(mafilter(uftd), label='py_ftd')
#plt.plot(uftd, label='uftd')
#plt.plot(ftd, label='ard_ftd')

#plt.plot(t2f(uftd), label='uftd_fq')
#plt.plot(t2f(ftd), label='ard_ftd_fq')

plt.legend()
plt.show()
