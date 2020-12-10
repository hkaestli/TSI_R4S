vd 2400 mV
id 200  mA
va 2400 mV
ia 300  mA

ibiasio  400 mV @ 4.7kOhm 130uA
voffset  539
ibiasro  525 mV @ 22kOhm 20uA

vprefb   881 
vn2      800
vn1      730
vn0      740
vcascn   850
vbiasd  2047

vcal     1200
vfb     1500
vbiasr  1800
vcascp  1350
vp0     1200
vp1     1280
vp2      900

v18     1800
vanan   1800
vddio   1800
vanap   1550
vdig    1800

adcdel 7
daqena 3

tspan 3000
tcal   2508
thold  2517

cal 32 1

d1 5 trigger
d2 0
a2 0
a1 1 roc4sens data out

pon
mdelay 500
getia
getid
go
