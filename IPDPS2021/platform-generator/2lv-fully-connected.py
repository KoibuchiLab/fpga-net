import numpy as np
import sys
import os

if len(sys.argv) == 3: 
	NUM_OF_GROUPS=int(sys.argv[1])
	NUM_OF_NODES_IN_GROUP=int(sys.argv[2])
else:
	NUM_OF_GROUPS=3
	NUM_OF_NODES_IN_GROUP=3

NUM_OF_NODES=NUM_OF_GROUPS * NUM_OF_NODES_IN_GROUP
NUM_OF_LINKS=int(NUM_OF_GROUPS * NUM_OF_NODES_IN_GROUP*(NUM_OF_NODES_IN_GROUP - 1)/2 + NUM_OF_GROUPS*(NUM_OF_GROUPS - 1))

path = "../platforms/2lvfc/"
OUTPUT_FILENAME = "%s%dx%d.xml" % (path, NUM_OF_GROUPS, NUM_OF_NODES_IN_GROUP)

if not os.path.exists(path):
    os.makedirs(path)

DEFAULT_BANDWIDTH = "12.5GBps"
DEFAULT_LATENCY = "1us"
DEFAULT_SPEED = "100Gf"
def host(hostname, speed = DEFAULT_SPEED):
	return "\t\t<host id=\"%s\" speed=\"%s\"/>\n" % (hostname, speed)

def link(id, bandwidth = DEFAULT_BANDWIDTH, latency = DEFAULT_LATENCY):
	return "\t\t<link id=\"%s\" bandwidth=\"%s\" latency=\"%s\"/>\n" % (id, bandwidth, latency)	

def route_directlink(src, dest, _link):
	return "\t\t<route src=\"%s\" dst=\"%s\">\n\t\t\t<link_ctn id=\"%s\"/>\n\t\t</route>\n"% (src, dest, _link)

def isgreater(src, dst):
	src_a = int(src.split('_')[0])
	src_b = int(src.split('_')[1])
	dst_a = int(dst.split('_')[0])
	dst_b = int(dst.split('_')[1])
	if src_a > dst_a :
		return True
	elif src_a == dst_a and src_b > dst_b:
		return True
	else:
		return False

def route_disjoint(src, dst):  
	src_b = src.split('_')[1]
	dst_a = dst.split('_')[0]
	int_node = dst_a + "_" + src_b
	link1 = ""
	link2 =""
	if isgreater(src, int_node):
		link1 = "%s_to_%s" % (int_node, src)
	else:
		link1 = "%s_to_%s" % (src, int_node)
	
	if isgreater(int_node, dst):
		link2 = "%s_to_%s" % (dst, int_node)
	else:
		link2 = "%s_to_%s" % (int_node, dst)

	result =  "\t\t<route src=\"%s\" dst=\"%s\">\n \
		\t<link_ctn id=\"%s\"/>\n\
		\t<link_ctn id=\"%s\"/>\n\
		</route>\n" % (src, dst, link1, link2)
	return result

def edge2linkid(edge):
	return "%d_to_%d" % (edge[0], edge[1])

hostIDlist = []
hostlist = []
linklist = []
routelist = []

#Add hostname to host list
for group in range(NUM_OF_GROUPS):
	for node in range(NUM_OF_NODES_IN_GROUP):
		cur_hostname = "%d_%d" %(group, node)
		hostIDlist.append(cur_hostname)
		hostlist.append(host(cur_hostname))
	
assert(len(hostlist) == NUM_OF_NODES)

for i in range(len(hostIDlist)):
	for j in range(i + 1, len(hostIDlist), 1):
		src = hostIDlist[i]
		dst = hostIDlist[j]
		src_a = src.split('_')[0]
		src_b = src.split('_')[1]
		dst_a = dst.split('_')[0]
		dst_b = dst.split('_')[1]
		if src_a == dst_a or src_b == dst_b: #internal group #external group direct path
			linkID = "%s_to_%s" % (src, dst)
			linklist.append(link(linkID))
			
			routelist.append(route_directlink(src, dst, linkID))
		else: #disjoint link
			routelist.append(route_disjoint(src, dst))


f = open(OUTPUT_FILENAME, "w")
header = 	["<?xml version='1.0'?>\n", 
			"<!DOCTYPE platform SYSTEM \'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\'>\n",
			"<platform version=\"4.1\">\n", 
			]

f.writelines(header)
f.write("\t<zone  id=\"AS0\"  routing=\"Full\">\n")
f.writelines(hostlist)
f.writelines("\n")
f.writelines(linklist)
f.writelines("\n")
f.writelines(routelist)
f.write("\t</zone>\n")
f.write("</platform>")

f.close()
path = "hostfiles/2lvfc/"
OUTPUT_FILENAME_H = "%s%dx%d.txt" % (path, NUM_OF_GROUPS, NUM_OF_NODES_IN_GROUP)
if not os.path.exists(path):
    os.makedirs(path)


f = open(OUTPUT_FILENAME_H, "w")
for i in range(len(hostIDlist)):
	f.writelines(hostIDlist[i])
	f.writelines("\n")
f.close()
