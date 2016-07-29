load("gdcficonn.info.sage")

for i in range(10,44):
    N=DCellN(k=2,n=i,alpha=DCELL_ALPHA)
    print "topo=gdcficonn_2_%d_0 failure_rate=0.0 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_1 failure_rate=0.0 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_0 failure_rate=0.1 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_1 failure_rate=0.1 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_0 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_1 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_0 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_1 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)

for i in [j in range(18,87) if j%2==0]:
    N=DCellN(k=2,n=i,alpha=FICONN)
    print "topo=gdcficonn_2_%d_2 failure_rate=0.0 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_2 failure_rate=0.1 routing=gdcficonn-proxy_0_2 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_2 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_2_%d_2 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)

for i in range(3,7):
    N=DCellN(k=3,n=i,alpha=DCELL_ALPHA)
    print "topo=gdcficonn_3_%d_0 failure_rate=0.0 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_1 failure_rate=0.0 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_0 failure_rate=0.1 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_1 failure_rate=0.1 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_0 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_1 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_0 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_1 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)

for i in [j in range(8,17) if j%2==0]:
    N=DCellN(k=3,n=i,alpha=FICONN)
    print "topo=gdcficonn_3_%d_2 failure_rate=0.0 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_2 failure_rate=0.1 routing=gdcficonn-proxy_0_3 " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_2 failure_rate=0.0 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
    print "topo=gdcficonn_3_%d_2 failure_rate=0.1 routing=gdcficonn-dimensional " \
        "tpattern=manyall2allrnd_%d"%(i,100000000/N)
