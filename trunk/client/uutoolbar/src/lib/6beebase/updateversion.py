# python code for version update

f=open(r'S:\uutoolbar\src\lib\6beebase\versiondef.h','r')
b = f.readline();
f.close()
a=b[2:len(b)-1]
c=a.split('.')
d=c[len(c)-1]
e = int(d)+1

x=''
for i in c[:-1]:
	x = x+'.'+ i

ultra_filever_str = x[1:]+'.'+str(e)
print "New Version:" + ultra_filever_str
###############################################

y=''
for i in c[:-1]:
	y = y+','+ i
ultra_filever = y[1:]+','+str(e)
###############################################

content = "//"+ ultra_filever_str + "\n" + """// Do not edit the first line unless you know what you are doing.
#ifndef __VERSIONDEF__
#define __VERSIONDEF__

#define ultra_filever """+ ultra_filever +'''
#define ultra_filever_str "''' + ultra_filever_str + '''"

#define ultra_prodver 1,0
#define ultra_prodver_str "1.0"

#define ultra_companyname "6BeeNetworks Pte. Ltd."
#define ultra_copyright "2009 @ 6BeeNetworks Pte. Ltd.  All rights reserved."
#define ultra_prodname "6BeeNetworks Tool Kit"

#endif

'''

f=open(r'S:\uutoolbar\src\lib\6beebase\versiondef.h','w')
f.write(content)
f.close()
