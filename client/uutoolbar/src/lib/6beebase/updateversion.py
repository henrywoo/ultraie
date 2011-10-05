# UltraIE - Yet Another IE Add-on
# Copyright (C) 2006-2010
# Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
# Homepage: http:#www.linkedin.com/in/simonwoo
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.

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
