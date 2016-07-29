#!/opt/local/bin/python

import sys, os, math

def ReadHistogram(fs):
    H=-1
    if os.path.exists(fs):
        with open(fs, 'r') as infile:
            try:
                lines = infile.readlines()
                H=dict([[int(a.strip()) for a in i.split()] for i in lines])
                infile.close()
            except:
                H=-1
    return H

##################################################
# BinHistogram() takes as input a dictionary of pairs [x, y(x)] and outputs
# dictionary with elements x1:y(x1,x2), where y(x1,x2) is the sum
# of the histogram values in the range (x1,x2).
#
# inputs: specify (either) the number of `bins' or the `bin_size' (but
# not both) to determine the size and number of bins.
#
#
def BinHistogram(H,bins=10,bin_size=-1):
    if bins>0 and bin_size>0:
        print 'WARNING: You have specified both a bin_size ' \
            'and number of bins in BinHistogram.  I\'ll use'\
            ' bin_size and discard bins'
    maximum = max(H.keys())
    if bin_size > 0:
        bins=int(math.ceil((maximum+1)*1.0/(bin_size)))
    else:
        bin_size=int(math.ceil(1.0*(maximum+1)/bins))
    bHistogram=[0 for i in range(bins)]
    for k in H.keys():
        bHistogram[k/bin_size]=bHistogram[k/bin_size]+H[k]
    bHistogram=dict(zip([i*bin_size for i in \
                     range(bins)],bHistogram))
    return bHistogram
    
def GraphicalHistogram(H,bins=10,bin_size=-1,norm=40):
    if bins>0 and bin_size>0:
        print 'WARNING: You have specified both a bin_size ' \
            'and number of bins in DrawHistogram.  I\'ll use'\
            ' bin_size and discard bins'
    s = ''
    maximum = max(H.keys())
    H=BinHistogram(H,bins=bins,bin_size=bin_size)        
    if bin_size > 0:
        bins=int(math.ceil((maximum+1)*1.0/(bin_size)))
    else:
        bin_size=int(math.ceil(1.0*(maximum+1)/bins))
    maxval=max(H.values())
    sortedkeys=sorted(H.keys())
    format_string='[%'+ '%d'%(len(str(maximum))) + 'd - %' + '%d'%(len(str(maximum)))+'d)'
    format_maxval=':%' + '%d'%len(str(maxval)) + 'd|'
    for idx,k in enumerate(sortedkeys):
        s=s + format_string%(k,k+bin_size) + format_maxval%H[k] + \
            ''.join(['#' for i in range(int(norm*1.0*H[k]/maxval))]) + '\n'
    return s

def NumericHistogram(H, bins=10, bin_size=-1, norm=1):
    if bins>0 and bin_size>0:
        print 'WARNING: You have specified both a bin_size ' \
            'and number of bins in DrawHistogram.  I\'ll use'\
            ' bin_size and discard bins'
    maximum = max(H.keys())
    H=BinHistogram(H,bins=bins,bin_size=bin_size)
    if bin_size > 0:
        bins=int(math.ceil((maximum+1)*1.0/(bin_size)))
    else:
        bin_size=int(math.ceil(1.0*(maximum+1)/bins))
    totalvalues=sum(H.values())
    Hnorm={}
    for k in H.keys():
        Hnorm[k]=(H[k]*1.0/totalvalues)*norm
    sortedkeys=sorted(H.keys())
    s=''
    for k in sortedkeys:
        s=s+'%d'% k+ '\t' + '%d'%H[k] + '\t' + '%g'%Hnorm[k] + '\n'
    # bin_size=int(math.ceil(1.0*(maximum+1)/bins))
    s=s+'%d'% (k+bin_size)+ '\t' + '0' + '\t' + '0.0'
    return s


arglist=sys.argv
if( len(arglist) < 5 or len(arglist) > 6):
    print "Expected 4 or 5 arguments arguments: file 'n|s'"
    print "nbins|bin_size 'graphical [maxbar] | numeric'"
    exit()

H=ReadHistogram(arglist[1])
if( H==-1 ):
    print 'Cannot open file ' + arglist[1]
    exit()
if(sys.argv[2] == 'n'):
    bin_size=0
    n_bins=int(sys.argv[3])
elif(sys.argv[2] == 's'):
    bin_size=int(sys.argv[3])
    n_bins=0
else:
    print "Expected 2nd input to be 'n|s' for number or size of bins."
output=sys.argv[4]
if(output=='graphical'):
    if( len(arglist) == 6):
        maxbar=int(sys.argv[5])
    else:
        maxbar=40
    print GraphicalHistogram(H,bins=n_bins, bin_size=bin_size, norm=maxbar)
elif(output=='numeric'):
    print NumericHistogram(H,bins=n_bins, bin_size=bin_size)
else:
    print 'Last required argument should be "graphical|numeric"'
