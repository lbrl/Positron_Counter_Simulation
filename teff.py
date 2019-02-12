#! /usr/bin/env python

# from ROOT import RooFit as rf
# from ROOT import *
import ROOT as r
import sys
import os
# import glob
import math
# import datetime
# import numpy as np
# from array import array
# import re
# import matplotlib as ml
# import matplotlib.pyplot as plt
# from PIL import Image

# R.gSystem.Load('libRooFit')


HEADER = '\033[95m'
OKBLUE = '\033[94m'
OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'
CBLACK  = '\33[30m'
CRED    = '\33[31m'
CGREEN  = '\33[32m'
CYELLOW = '\33[33m'
CBLUE   = '\33[34m'
CVIOLET = '\33[35m'
CBEIGE  = '\33[36m'
CWHITE  = '\33[37m'
CBLACKBG  = '\33[40m'
CREDBG    = '\33[41m'
CGREENBG  = '\33[42m'
CYELLOWBG = '\33[43m'
CBLUEBG   = '\33[44m'
CVIOLETBG = '\33[45m'
CBEIGEBG  = '\33[46m'
CWHITEBG  = '\33[47m'


# foilhit = 32677
def getEffErr( p, t ):
    hp = r.TH1I( 'hp', 'hp', 1, 0, 1 )
    ht = r.TH1I( 'hp', 'hp', 1, 0, 1 )
    hp.SetBinContent( 1, p )
    ht.SetBinContent( 1, t )
    eff = r.TEfficiency( hp, ht )
    eu = eff.GetEfficiencyErrorUp( 1 )
    el = eff.GetEfficiencyErrorLow( 1 )
    val = eff.GetEfficiency( 1 )
    return [el, eu, val]


def main():
    p = int( sys.argv[1] )
    t = int( sys.argv[2] )
    eff = getEffErr( p, t )
    print '{:>10} +{:<10.8f}'.format( '', abs(eff[1]) )
    print '{:>10.8f}'.format( eff[2] )
    print '{:>10} -{:<10.8f}'.format( '', abs(eff[0]) )


if __name__ == '__main__':
    main()
