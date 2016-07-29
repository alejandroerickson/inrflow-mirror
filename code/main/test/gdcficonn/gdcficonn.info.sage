#
# Title: rri Experiments
#
# Author: Alejandro Erickson
#
# Date: 04 April 2015
#
VERSION=3.2
SANDBOX='sandbox/v3.2/'
OUTDIR='../../out/v3.2/'
#
# New in Version 3.2.  Fixed a bug where "tight" proxies not working when k=2
#
# New in Version 3.1.  hacked things so that they work for
# (FICONN,4,6) and (FICONN,4,8).  I don't think I broke k<4, but I'm
# not certain.
#
# New in Version 3.0.  implement smaller proxy candidates,
# ProxyCandidatesT0, where essentially we do $c$ such that $a^c-(a^c
# mod t_0) = a_2t_1+a_1t_0$, etc.  i.e., proxies such that at least
# one part of the proxy route is confined to a $D_0$, instead of a
# $D_1$.
#
# New in Version 2.1.  fixed bug in counting number of good_routes
#
# New in Version 2.0.  reuse random inputs, and much more.
#
# New in Version 1.2.  Fixed a bug in V1.1 which does not count all of
# the proxies tried at level k=2.  Specifically, they were only
# counted for the k=3 proxy that was _USED_, rather than the k=3
# proxies that were searched.
#
# New in Version 1.1.  MakeGK has been introduced to correct problems
# with FiConn (using tk where we should use gk).  Will run all
# experiments again.
#
# Short Description: Performs all experiments in rri paper, on finding short
# routes in DCell and FiConn, and outputs data.
#
# Long Description: Run the script in a Sage interpreter with
# sage: main(trials=1000)
# Optional arguments to main are OUTDIR='../../out/',trials=1

import random
import itertools
import os
from collections import Counter
DEBUG=False

RANDSEED=0

###################################
# Connection rule constants: These constants are used in various
# functions to determine which connection rule to use (in GetLink).
#
# DCELL_ALPHA is the original DCell connection rule
DCELL_ALPHA=0
# DCELL_BETA is the Generalized DCell beta rule.
DCELL_BETA=1
# FICONN is the original FiConn rule
FICONN=2

# Notation: 
#
# For DCell_{k,n} with n=2, k = 3, we have tk = [2, 6, 42, 1806], gk =
# [1, 3, 7, 43].  This means there are 2 servers at the 0th level, 3
# clusters at the 1st level, 7 clusters at the 2nd level, and 43
# clusters at the 3rd level.  Each 0th level cluster has 2 servers.
# Each 1st level cluster has 6 servers, then 42, and finally
# DCell_{3,2} has 1806 servers.

# Example server u = [0,1,4,42] u is the 0th server at level 0. 

#UID_i indicates which server this is in D_i.

#UID_1 = 0 + 1*2 = 2.
#UID_2 = 0 + 1*2 + 4*6 = 26
#UID_3 = 0 + 1*2 + 4*6 + 42*42 = 1790

#Server in D_k has label L=[a_0,a_1, ..., a_k], with L[i] < gk[i] for i > 0.
#UID_k=L[0]+L[1]*tk[0]+L[2]*tk[1]+ ... + L[k]*tk[k-1]

# For FiConn_{k,n} the notation is the same as above, but the
# coordinates of tk and gk are different

################################################################
# UIDi(L,i,tk) returns the UID of node L in its DCell_{i,n}
# (FiConn_{i,n}).
#
# inputs:
#
# L is the label of a node in a DCell_{k,n} (FiConn_{k,n}) with k>=i.
#
# tk=MakeTK(k,n)
#
# Tested?  Yes, against Coordinates(). On 27 December, 2014,
# allexp_tests.sage
def UIDi(L,i,tk):
    """Returns L[0]+L[1]*tk[0]+L[2]*tk[1]+ ... + L[i]*tk[i-1]"""
#L is for label
    uidi = L[0]
    for a in range(i):
        uidi = uidi + L[a+1]*tk[a]
    return uidi

################################################################    
# Coordinates(a,i,tk) returns the label of UID_i(a) in D_i, which is
# [x_0,x_1,...,x_i], where a=x_0+t_0x_1+t_1x_2 + ... + t_{i-1}x_i
#
# inputs:
#
# a is a UID in D_i, so 0\le a < tk[i]
#
# i\le k
#
# tk=MakeTK(k,n)
#
# Tested? Yes.  On 27 December, 2014, allexp_tests.sage
#
# Updated in Version 1.0
def Coordinates(a,i,tk):
    a = a%tk[i]
    x=[a%tk[0]]
    a = (a-x[0])
    for j in range(1,i+1):
        x.append((a%tk[j])/tk[j-1])
        a = a-x[j]*tk[j-1]
    return x

#####################################
# RandNode() returns the uid of a random node in a D_i, for 0<=i<=k
#
# input: tk=MakeTK(k,n)[k]
def RandNode(tk):
    return random.randint(0,tk-1)

################################################################        
# PathSuffix(suffix,P) is a utility which adds a suffix to extend a
# path within DCell_{i,n} to a path in a containing DCell_{k,n}, with
# i<k
#
# inputs: suffix is a list of integers
#
# P is a list of lists of integers of uniform length
#
# output: [P[0]+suffix,P[1]+suffix, ..., P[-1]+suffix]
#
# Tested? No, but not needed.
def PathSuffix(suffix,P):
    for i in range(len(P)):
        P[i] = P[i]+suffix
    return P

################################################################
# GetLink(alpha,a,b,l,tk) returns the edge between D_(l-1)^a and
# D_(l-1)^b, where these are DCell_(l-1) inside a DCell_{l} (or
# FiConn).  That is, a and b are the lth coordinates of some nodes
# u,v, and we return a list pair of the form [uid_{l-1},uid_{l-1}].
#
# inputs: alpha \in {DCELL_ALPHA, DCELL_BETA, FICONN}, according to
# which connection rule we are using.
#
# l is the level of link we are returning.  1 \le l \le k
#
# a,b are the indices of DCell_{l-1}s
#
# output: [x,y], where the link is ([a,x],[b,y])
#
# Tested? No, not by itself.
def GetLink(alpha,a,b,l,tk):
    if alpha==DCELL_ALPHA:
        return GetLink_alpha(a,b)
    if alpha==DCELL_BETA:
        return GetLink_beta(a,b,l,tk)
    if alpha==FICONN:
        return GetLink_FiConn(a,b,l)        

################################################################
# GetLink_alpha(a,b) is a utility for GetLink(alpha,a,b,l,tk)
# 
# Justification: From KliegLeeLi2010, [a,b-1] connects to [b,a] when
# a<b (and links are bidirectional.
#
# Tested? No, not needed.
def GetLink_alpha(a,b):    
    if b<a:
        val=GetLink_alpha(b,a)
        val.reverse()
        return val
    if b==a:
        print "ERROR"
        exit
    return [b-1,a] # node b-1 in D_l^a, node a in D_l^b

################################################################
# GetLink_beta(a,b,l,tk) is a utility for GetLink(alpha,a,b,l,tk)
#
# outputs: node b-a-1 in D_{l-1}^a, node N_{l-1}-b+a in D_{l-1}^b
#
# Justification: From KliegLeeLi2010, [a,B] connects to [a+B+1
# mod(N_{l-1}+1),N_{l-1}-1-B].  Let b=a+B+1 mod(N_{l-1}+1), and
# observe that 0\le a \le N_{l-1}, and 0\le B \le N_{l-1}-1, so 1 \le
# a+B+1\le 2N_{l-1}-1, so that a+N_{l-1}-1+1 mod (N_{l-1}+1) = a-1.
# Thus b=a+B+1 iff a+B+1\le N_{l-1} (and a<b), and
# b=a+B+1-N_{l-1}-1=a+B-N_{l-1} iff a+B+1>N_{l-1} (and b<a).  So if
# a<b, connect [a,b-a-1] with [b,N_{l-1}-1-b+a+1]=[b,N_{l-1}-b+a] (and
# note that the rule is symmetric).
#
# Tested? No, and probably not needed.
def GetLink_beta(a,b,l,tk):
    if b<a:
        val=GetLink_beta(b,a,l,tk)
        val.reverse()
        return val
    if b==a:
        print "ERROR"
        exit
    return [b-a-1,tk[l-1]-b+a] 
    
################################################################
# GetLink_FiConn(a,b,l,tk) is a utility for GetLink(alpha,a,b,l,tk)
# 
# Justification: From FiConn paper, connect [a,j1] with [b,j2], where
# a<b and j1=(b-1)2^l+2^{l-1}-1 and j2=a*2^l+2^{l-1}-1.  This follows
# directly from the connection algorithm and details are in
# Alejandro's notebook.  Once again, the connection rule is symmetric.
#
# Tested? No, and probably not needed.
def GetLink_FiConn(a,b,l):
    if b<a:
        val=GetLink_FiConn(b,a,l)
        val.reverse()
        return val
    if b==a:
        print "ERROR"
        exit
    k2=2^l
    val=(b-1)*k2+k2/2-1
    return [val,val-(b-1-a)*k2] #a*k2+k2/2-1

################################################################
# AreNeighbours(alpha,u,v,tk) returns True if there is a link between
# nodes u and v, under connection rule alpha, otherwise False
#
# inputs: u and v are nodes (coordinate lists)
#
# alpha is the connection rule required by GetLink
#
#tk=MakeTK(k,n)
#
# Tested? Yes, though it was more or less against a rewording of the
# same function.
def AreNeighbours(alpha,u,v,tk):
    m=GetCommSuffix(u,v)
    if m==-1:
        print "AreNeighbours False, inputs, m==-1", alpha,u,v,tk
        return False
    if m==0:
        return True
    #we want to know whether (u,v) is the link between D_{m-1}^u[m]
    #and D_{m-1}^v[m]
    if GetLink(alpha,u[m],v[m],m,tk)==[UIDi(u,m-1,tk),UIDi(v,m-1,tk)]:
        return True
    else:
        print "AreNeighbours False, inputs", alpha,\
            u,v,tk,GetLink(alpha,u[m],v[m],m,tk),\
            [UIDi(u,m-1,tk),UIDi(v,m-1,tk)]
        return False

################################################################
# GetCommSuffix(src,dst) returns i such that src[i+1:] == dst[i+1:]
# but src[i]!=dst[i]
#
# inputs: src and dst are nodes (coordinate lists).
#
# Tested?  Yes, visual inspection.
def GetCommSuffix(src,dst):
    i=len(src)-1
    if not i == len(dst) -1:
        print "BOOOOO, GetCommSuffix wrong lengths"
    while src[i]==dst[i] and i>=0:
        i=i-1
    return i #if i=-1 then src=dst



############################################
# DCellN(k=3,n=3,alpha=DCELL_ALPHA) returns the number of server nodes in
# a DCell_{k,n} (or FiConn)
#
# Tested? No, not needed.
def DCellN(k=-1,n=-1,alpha=-1):
    if k==-1 or n==-1 or alpha==-1:
        print "ERROR in DCellN.  Specify all three keyed parameters", k,n,alpha
    tk = n
    if(alpha==DCELL_ALPHA
       or alpha==DCELL_BETA):
        for i in range(k):
            tk = tk*(tk+1)
    if(alpha==FICONN):
        for i in range(k):
            tk = tk*(tk/2^(i+1)+1)
    return tk

############################################
# DCellD(k=3,n=3) returns the canonical upper bound on diameter for a
# DCell_{k,n} (or FiConn).
#
# Tested? No, not needed.
def DCellD(k=-1):
    if k==-1:
        print "ERROR in DCellD, specify keyed parameter", -1
    return 2^(k+1)-1

##################################################
# DCellE counts the number of links (edges) in a DCell_{k,n} (FiConn)
#
# These are bidirectional.
#
# Tested? No, but hand verified.
#
# Found a mistake here on 2015-03-20, where the level k edges were
# being double counted for k>0.  This affects histograms, but nothing
# else.
def DCellE(k=3,n=6,alpha=DCELL_ALPHA):
    tk=MakeTK(n=n,k=k,alpha=alpha)
    gk=map(lambda x,y: x/y,tk[1:],tk[:-1])
    s=n
    for t in gk:
        s=s*t+t*(t-1)/2
    return s

############################################
# MakeTK(n,k,alpha=DCELL_ALPHA) returns the list [t_0,t_1,..., t_k],
# where t_i is the number of servers in a DCell_{i,n} (also FiConn)
#
# Tested? Yes, by visual inspection and hand verification of output,
# 27 December, 2014, allexp_tests.sage
#
def MakeTK(n=-1,k=-1,alpha=-1):
    tk = range(k+1)
    tk[0] = n
    if n==-1 or k==-1 or alpha==-1:
        print "ERROR: need to specify three inputs, n,k,alpha", n,k,alpha
        return -1
    if(alpha==DCELL_ALPHA
       or alpha==DCELL_BETA):
        for i in range(k):
            tk[i+1] = tk[i]*(tk[i] + 1)
    if(alpha==FICONN):
        for i in range(k):
            tk[i+1] = tk[i]*(tk[i]/2^(i+1) + 1)
    return tk
##################################################
# MakeGK(n=-1,k=-1,alpha=-1,tk=-1) returns the list g_k (with g_0=n),
# where g_k is the number of DCell_{k-1} in a DCell_{k}
# (resp. FiConn).
# 
#
# Tested? Yes, by visual inspection, 28 December, 2014.
def MakeGK(tk=-1):
    return [tk[0]]+map(lambda x,y: x/y,tk[1:],tk[:-1])

##################################################
# DCellStats(k=-1,n=-1,alpha=-1) returns stats on the requested
# network, as values separated by spaces.  The values are (in this
# order):
#
# k n nservers nswitches nlinks diameter tk
#
# Tested?  Not used in experiments, but I did test this visually for
# building a table in the paper
def DCellStats(k=-1,n=-1,alpha=-1,sig=2,gmax=4):
    if n==-1 or k==-1 or alpha==-1:
        print "ERROR: need to specify three inputs, n,k,alpha", n,k,alpha
        exit()
    if sig==-1:
        d=1
    else:
        d=10^6
    tk = MakeTK(k=max(k,gmax),n=n,alpha=alpha)
    gk = MakeGK(tk=tk)
#    tk = tk + ['NaN' for i in range(4-len(tk))]
    N=DCellN(k=k,n=n,alpha=alpha)
    if alpha==FICONN:
        D='F'#'\\ficonn'
    if alpha==DCELL_ALPHA:
        D='D'#'\\dcell'
    if alpha==DCELL_BETA:
        D='B'#'\\dcellb'
    if d==1:
        return ['%s\\pms{%d}{%d}'%(D,k,n),
                N,
                N/n,
                DCellE(k=k,n=n,alpha=alpha),
                DCellD(k=k)]+gk[1:max(gmax,len(gk))]
    else:
        return ['%s\\pms{%d}{%d}'%(D,k,n),
                round(N/d,sig),
                round(N/n/(d/10),sig),
                round(DCellE(k=k,n=n,alpha=alpha)/d,sig),
                DCellD(k=k)]+gk[1:max(gmax,len(gk))]
#    return '%d %d %d %d %d %d %s'%(k,n,N,N/n,DCellE(k=k,n=n,alpha=alpha),DCellD(k=k),join(map(str,tk),' '))

FICONN_K2=[(2,36,FICONN),(2,48,FICONN)]
FICONN_K3=[(3,10,FICONN),(3,16,FICONN)]
DCELL_K2_A=[(2,18,DCELL_ALPHA),(2,43,DCELL_ALPHA)]
DCELL_K2_B=[(2,18,DCELL_BETA),(2,43,DCELL_BETA)]
DCELL_K3_A=[(3,3,DCELL_ALPHA),(3,6,DCELL_ALPHA)]
DCELL_K3_B=[(3,6,DCELL_BETA)]
def DCellStatsTable(TESTS=(FICONN_K2+
                           FICONN_K3+
                           [(4,6,FICONN),(4,8,FICONN)]+
                           DCELL_K2_A+
                           DCELL_K3_A
                           #DCELL_K2_B+
                           #DCELL_K3_B
                       ), sig=-1,gmax=4):
    if sig==-1:
        s=[['DCN',
            '$N$',
            '$N/n$',
            '$|E|$',
            '$d$']+['$g_%d$'%(i+1) for i in range(gmax)]] #,['\\hline']]
    else:    
        s=[['DCN',
            '$N^\\star$',
            '$N/n^\\star$',
            '$|E|^\\star$',
            '$d$','$g_1$',
            '$g_2$',
        '$g_3$']] #,['\\hline']]

    for t in TESTS:
        s.append(DCellStats(k=t[0],n=t[1],alpha=t[2],sig=sig,gmax=gmax))
    print latex(table(s))
    return s

