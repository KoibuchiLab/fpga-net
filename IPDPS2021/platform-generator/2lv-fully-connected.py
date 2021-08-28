import numpy as np
import sys

if len(sys.argv) == 3: 
	DEGREE=int(sys.argv[1])
	DEGREE_OUT=int(sys.argv[2])
else:
	DEGREE=4
	DEGREE_OUT=2
NUM_OF_GROUPS=DEGREE_OUT + 1
NUM_OF_NODES_IN_GROUP=DEGREE - DEGREE_OUT + 1
NUM_OF_NODES=NUM_OF_GROUPS * NUM_OF_NODES_IN_GROUP
NUM_OF_LINKS=int(NUM_OF_GROUPS * NUM_OF_NODES_IN_GROUP*(NUM_OF_NODES_IN_GROUP - 1)/2 + DEGREE_OUT*(DEGREE_OUT + 1))

OUTPUT_FILENAME = "test.xml"
DEFAULT_BANDWIDTH = "1GBps"
DEFAULT_LATENCY = "50us"
DEFAULT_SPEED = "1Gf"
def host(hostname, speed = DEFAULT_SPEED):
	return "\t\t<host id=\"%s\" speed=\"%s\"/>\n" % (hostname, speed)

def link(id, bandwidth = DEFAULT_BANDWIDTH, latency = DEFAULT_LATENCY):
	return "\t\t<link id=\"%s\" bandwidth=\"%s\" latency=\"%s\"/>\n" % (id, bandwidth, latency)	

def route(src, dest, _link):
	return "\t\t<route src=\"%s\" dst=\"%s\">\n\t\t\t<link_ctn id=\"%s\"/>\n\t\t</route>\n"% (src, dest, _link)

def edge2linkid(edge):
	return "%d_to_%d" % (edge[0], edge[1])


hostlist = []
linklist = []
routelist = []

#Add hostname to host list
for group in range(NUM_OF_GROUPS):
	for node in range(NUM_OF_NODES_IN_GROUP):
		cur_hostname = "%d-%d" %(group, node)
		hostlist.append(host(cur_hostname))
	
assert(len(hostlist) == NUM_OF_NODES)

#Add links for internal connections
for group in range(NUM_OF_GROUPS):
	for node in range(NUM_OF_NODES_IN_GROUP):
		cur_hostname = "%d-%d" %(group, node)
		for target in range(node + 1, NUM_OF_NODES_IN_GROUP):
			#link from node group-node to all other
			target_hostname = "%d-%d" % (group, target)
			linkID = "%s_to_%s" % (cur_hostname, target_hostname)
			linklist.append(link(linkID))
			#print(linkID)

			#add route
			routelist.append(route(cur_hostname, target_hostname, linkID))

#Add links for external connections
for node in range(NUM_OF_NODES_IN_GROUP):
	cur_hostname = "%d-%d" %(0, node)
	for group in range(1, NUM_OF_GROUPS):
		target_hostname = "%d-%d" % (group, node)
		linkID = "%s_to_%s" % (cur_hostname, target_hostname)
		linklist.append(link(linkID))
		#print(linkID)

		#add route
		routelist.append(route(cur_hostname, target_hostname, linkID))

assert(len(linklist) == NUM_OF_LINKS)
		

f = open(OUTPUT_FILENAME, "w")
header = 	["<?xml version='1.0'?>\n", 
			"<!DOCTYPE platform SYSTEM \"https://simgrid.org/simgrid.dtd\">\n",
			"<platform version=\"4.1\">\n", 
			]

f.writelines(header)
f.write("\t<zone  id=\"AS0\"  routing=\"Full\">\n")
f.writelines(hostlist)
f.writelines(linklist)
f.writelines(routelist)
f.write("\t</zone>\n")
f.write("</platform>")

f.close()
