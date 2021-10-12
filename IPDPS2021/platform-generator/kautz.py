import os
import sys
import argparse
parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--degree', type=int, default=3,
                    help='Enter degree')


args = parser.parse_args()
graph = "kautz"
d = args.degree
hosts = []
edges = []

hostlist = [] #hostlist to write to xml file
linklist = [] #linklist  to write to xml file
routelist = [] #routelist to write to xml file

DEFAULT_BANDWIDTH   = "12.5GBps"
DEFAULT_LATENCY     = "1us"
DEFAULT_SPEED       = "100Gf"
def host(hostname, speed = DEFAULT_SPEED):
    return "\t\t<host id=\"%s\" speed=\"%s\"/>\n" % (hostname, speed)

def link(id, bandwidth = DEFAULT_BANDWIDTH, latency = DEFAULT_LATENCY):
    return "\t\t<link id=\"%s\" bandwidth=\"%s\" latency=\"%s\"/>\n" % (id, bandwidth, latency)    

def route_directlink(src, dest, _link):
    return "\t\t<route src=\"%s\" dst=\"%s\" symmetrical=\"NO\">\n\t\t\t<link_ctn id=\"%s\"/>\n\t\t</route>\n"% (src, dest, _link)

#        <route src="src" dst="dst" symmetrical="NO">
#            <link_ctn id="host_src_to_host_int"/>
#            <link_ctn id="host_int_to_host_dst"/>
#        </route>
def route_disjoint(src, dst):  
    src_b = src.split(',')[1]
    dst_a = dst.split(',')[0]
    int_node = src_b + "," + dst_a
    result =  "\t\t<route src=\"%s\" dst=\"%s\" symmetrical=\"NO\">\n \
            \t\t<link_ctn id=\"%s_to_%s\"/>\n\
            \t\t<link_ctn id=\"%s_to_%s\"/>\n\
            \t</route>\n" % (src, dst, src, int_node, int_node, dst)
    return result

def edge2linkname(edge):        
    return "%s_to_%s" % (edge[0], edge[1])


for i in range(d+1):
    for j in range(d+1):
        if (i != j):
            hostname = "%s,%s" % (i, j)
            #print(hostname, "\n")
            hosts.append(hostname)

for src in hosts:
    src_a = src.split(',')[0]
    src_b = src.split(',')[1]
    for dst in hosts:
        dst_a = dst.split(',')[0]
        dst_b = dst.split(',')[1]
        if (src_b == dst_a):
            edges.append((src,dst))
            #print(edges[len(edges) - 1], "\n")

            # Add link and route to linklist and routelist
            linklist.append(link(edge2linkname((src, dst))))
            routelist.append(route_directlink(src, dst, edge2linkname((src, dst))))
            #print(linklist[len(linklist) - 1], routelist[len(routelist) - 1])

# Disjoint link
for src in hosts:
    for dst in hosts:
        src_b = str(src).split(",")[1]
        dst_a = str(dst).split(",")[0]
        if src_b != dst_a: # if disjoint link
            routelist.append(route_disjoint(src, dst))


assert len(edges)== d*d*(d+1), "Compute wrong number of edge"

for h in hosts:
    hostlist.append(host(h))
    #print(hostlist[len(hostlist) - 1])

path = "../platforms/%s" % graph
if not os.path.exists(path):
    os.makedirs(path)

#print(os.getcwd(), "\n")
output_filename = "%s/%s%s-%d.xml" % (path, graph, d, 2)# platforms/kautz/kautz2-2.xml
f = open(output_filename, "w")
header =     ["<?xml version='1.0'?>\n", 
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


path = "hostfiles/%s" % graph
if not os.path.exists(path):
    os.makedirs(path)

output_filename = "%s/%s%s-%d.txt" % (path, graph, d, 2)# hostfiles/kautz/kautz2-2
f = open(output_filename, "w")
for i in hosts:
    f.write("%s\n"% i)
f.close()

