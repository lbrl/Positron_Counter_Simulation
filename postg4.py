#! /usr/bin/env python

# from ROOT import RooFit as rf
# from ROOT import *
import ROOT as r
import sys
import os
# import glob
# import math
# import datetime
import numpy as np
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


def mkbranch(t, nomo, tipo, longo=1, longo_nomo=1):
    x = np.zeros( longo, tipo )
    if tipo == int:
        tipo_nomo = 'I'
    else:
        tipo_nomo = 'D'
    print 'tipo_nomo', tipo_nomo
    if longo == 1:
        t.Branch( nomo, x, '{}/{}'.format( nomo, tipo_nomo ) )
    else:
        t.Branch( nomo, x, '{}[{}]/{}'.format( nomo, longo_nomo, tipo_nomo ) )
    return x


def main():
    for i, a in enumerate( sys.argv ):
        if a == '-ig4':
            g4_name = sys.argv[i+1]
        if a == '-ig4bl':
            g4bl_name = sys.argv[i+1]
        if a == '-o':
            comb_name = sys.argv[i+1]
    #
    print 'g4_name', g4_name
    print 'g4bl_name', g4bl_name
    print 'comb_name', comb_name
    #
    g4_f = r.TFile( g4_name )
    g4bl_f = r.TFile( g4bl_name )
    g4_t = g4_f.Get( 'PC' )
    g4bl_t = g4bl_f.Get( 't' )
    print 'g4_t', g4_t
    print 'g4bl_t', g4bl_t
    #
    comb_f = r.TFile( comb_name, 'recreate' )
    comb_t = r.TTree( 'PC', 'PC + g4bl' )
    dsl3_x = mkbranch( comb_t, 'g4bl_x', float, 5, '5' )
    dsl3_y = mkbranch( comb_t, 'g4bl_y', float, 5, '5' )
    dsl3_z = mkbranch( comb_t, 'g4bl_z', float, 5, '5' )
    OriginX = mkbranch( comb_t, 'OriginX', float )
    OriginY = mkbranch( comb_t, 'OriginY', float )
    OriginZ = mkbranch( comb_t, 'OriginZ', float )
    PC1Time = mkbranch( comb_t, 'PC1Time', float )
    PC2Time = mkbranch( comb_t, 'PC2Time', float )
    PC3Time = mkbranch( comb_t, 'PC3Time', float )
    PC1Edep = mkbranch( comb_t, 'PC1Edep', float )
    PC2Edep = mkbranch( comb_t, 'PC2Edep', float )
    PC3Edep = mkbranch( comb_t, 'PC3Edep', float )
    TargetEdep = mkbranch( comb_t, 'TargetEdep', float )
    TargetX = mkbranch( comb_t, 'TargetX', float )
    TargetY = mkbranch( comb_t, 'TargetY', float )
    TargetZ = mkbranch( comb_t, 'TargetZ', float )
    #
    for i in xrange( g4_t.GetEntries() ):
        g4_t.GetEntry( i )
        g4bl_t.GetEntry( i )
        for j in xrange( 5 ):
            dsl3_x[j] = g4bl_t.x[j]
            dsl3_y[j] = g4bl_t.y[j]
            dsl3_z[j] = g4bl_t.z[j]
        OriginX[0] = g4_t.OriginX
        OriginY[0] = g4_t.OriginY
        OriginZ[0] = g4_t.OriginZ
        PC1Time[0] = g4_t.PC1Time
        PC2Time[0] = g4_t.PC2Time
        PC3Time[0] = g4_t.PC3Time
        PC1Edep[0] = g4_t.PC1Edep
        PC2Edep[0] = g4_t.PC2Edep
        PC3Edep[0] = g4_t.PC3Edep
        TargetEdep[0] = g4_t.TargetEdep
        TargetX[0] = g4_t.TargetX
        TargetY[0] = g4_t.TargetY
        TargetZ[0] = g4_t.TargetZ
        comb_t.Fill()
    #
    comb_t.Write()
    comb_f.Close()



if __name__ == '__main__':
    main()
