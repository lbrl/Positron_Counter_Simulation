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


def main():
    a = float( sys.argv[1] )
    b = float( sys.argv[2] )
    sa = float( sys.argv[3] )
    sb = float( sys.argv[4] )
    res = sa**2 / b**2
    res += a**2 * sb**2 / b**4
    res = res**.5
    print '{:.5g} +- {:.5g} ({:.3f}%)'.format( a/b, res, 100*res/(a/b) )


if __name__ == '__main__':
    main()
