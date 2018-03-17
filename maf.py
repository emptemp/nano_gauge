import matplotlib.pyplot as plt

fbuflen = 420
i = 0

fbuf = [0] * fbuflen
uftd = []
ftd = []

with open("log.txt", 'r') as f:
  lines = f.readlines()
  for line in lines:
    fbuf[i%fbuflen] = int(line)
    i += 1
    average = sum(fbuf)/fbuflen
    ftd.append(average)
    uftd.append(int(line))

#print ftd

plt.plot(ftd)
plt.plot(uftd)
plt.show()
