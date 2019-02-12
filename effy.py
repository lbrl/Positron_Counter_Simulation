#! /usr/bin/env python

# from ROOT import RooFit as rf
# from ROOT import *
import ROOT as r
import sys
import os
import glob
import math
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


hp = r.TH1I( 'hp', 'hp', 1, 0, 1 )
ht = r.TH1I( 'ht', 'ht', 1, 0, 1 )
def getEffErr( p, t ):
    ht.Reset()
    hp.Reset()
    ht.SetBinContent( 1, t )
    hp.SetBinContent( 1, p )
    eff = r.TEfficiency( hp, ht )
    eu = eff.GetEfficiencyErrorUp( 1 )
    el = eff.GetEfficiencyErrorLow( 1 )
    e = eff.GetEfficiency( 1 )
    return [e, el, eu]


def main():
    strict = 'normal'
    if strict == 'loose':
        cutplate = '10<OriginY&&OriginY<60 && -20<OriginX&&OriginX<20'
    elif strict == 'normal':
        cutplate = '20<OriginY&&OriginY<50 && -10<OriginX&&OriginX<13'
    else:
        return 2
    print 'strict : ', strict
    #
    t = r.TChain( 'PC' )
    t.Add( 'build/Simulation_*comb.root' )
    print '# entries : ', t.GetEntries()
    #
    fout = r.TFile( 'effy.root', 'recreate' )
    tout = r.TTree( 't', 'posi effy' )
    bslit = mkbranch( tout, 'slit', float )
    bndet8 = mkbranch( tout, 'ndet8', float )
    bnmc = mkbranch( tout, 'nmc', float )
    bn = mkbranch( tout, 'n', float, 7, '7' )
    bnp = mkbranch( tout, 'np', float, 7, '7' )
    #
    slits = [60, 80]
    slits = [20, 40, 60, 80, 100, 120]
    slits = [10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100, 100, 120]
    cut1 = 'PC1Edep>200'
    cut2 = 'PC2Edep>200'
    cut3 = 'PC3Edep>200'
    for slit in slits:
        bslit[0] = float(slit)
        print CYELLOW+'slit {}'.format( slit )+ENDC
        cutslit = 'g4bl_x[2]-100<{slit} && g4bl_x[3]-100>-{slit}'.format( slit=slit )
        cutslit += ' && g4bl_y[0]<{slit} && g4bl_y[1]>-{slit}'.format( slit=slit )
        print CGREEN+cutslit+ENDC
        #
        n1 = t.Draw( '', cutslit+' && '+cut1, 'goff' )
        n1p = t.Draw( '', cutslit+' && '+cut1+' && '+cutplate, 'goff' )
        n2 = t.Draw( '', cutslit+' && '+cut2, 'goff' )
        n2p = t.Draw( '', cutslit+' && '+cut2+' && '+cutplate, 'goff' )
        n3 = t.Draw( '', cutslit+' && '+cut3, 'goff' )
        n3p = t.Draw( '', cutslit+' && '+cut3+' && '+cutplate, 'goff' )
        print 'n1 {} -> {}   n2 {} -> {}   n3 {} -> {}'.format(n1, n1p, n2, n2p, n3, n3p)
        #
        n12 = t.Draw( '', cutslit+' && '+cut1+' && '+cut2, 'goff' )
        n12p = t.Draw( '', cutslit+' && '+cut1+' && '+cut2+' && '+cutplate, 'goff' )
        n13 = t.Draw( '', cutslit+' && '+cut1+' && '+cut3, 'goff' )
        n13p = t.Draw( '', cutslit+' && '+cut1+' && '+cut3+' && '+cutplate, 'goff' )
        n23 = t.Draw( '', cutslit+' && '+cut2+' && '+cut3, 'goff' )
        n23p = t.Draw( '', cutslit+' && '+cut2+' && '+cut3+' && '+cutplate, 'goff' )
        #
        n123 = t.Draw( '', cutslit+' && '+cut1+' && '+cut2+' && '+cut3, 'goff' )
        n123p = t.Draw( '', cutslit+' && '+cut1+' && '+cut2+' && '+cut3+' && '+cutplate, 'goff' )
        #
        n = [n1, n2, n3, n12, n13, n23, n123]
        np = [n1p, n2p, n3p, n12p, n13p, n23p, n123p]
        #
        ndet8 = t.Draw( "", cutslit, "goff" )
        nmc = t.Draw( "", cutslit + " && TargetY != 0", "goff" )
        bnmc[0] = nmc
        bndet8[0] = ndet8
        print 'ndet8', ndet8
        print 'nmc', nmc
        #
        for i in xrange( len( n ) ):
            bn[i] = n[i]
            bnp[i] = np[i]
            e1 = getEffErr( np[i], n[i] )
            e2 = getEffErr( np[i], nmc )
            e3 = getEffErr( np[i], ndet8 )
            print e1, e2, e3
        tout.Fill()
        #
    tout.Write()
    fout.Close()


def getlist(t, sel, cut=''):
    n = t.Draw( sel, cut, 'goff' )
    x = t.GetV1()
    x.SetSize( n )
    x = list( x )
    return x


def draw():
    fin = r.TFile( 'effy.root' )
    t = fin.Get( 't' )
    t.SetMarkerStyle( 20 )
    c1 = r.TCanvas( 'c1', 'c1', 600, 600 )
    x = getlist( t, 'slit' )
    xe = getlist( t, '0' )
    y = getlist( t, 'nmc' )
    ye = getlist( t, 'nmc**.5' )
    g = r.TGraphErrors( len(x), np.array(x), np.array(y), np.array(xe), np.array(ye) )
    g.SetMarkerStyle( 20 )
    g.Draw( 'ap' )
    c1.Update()
    raw_input()


def auf( f, l, u ):
    fs0 = '{:.10f}'.format( f )
    ls0 = '{:.10f}'.format( l )
    us0 = '{:.10f}'.format( u )
    p = 2
    # 
    ls1 = ''
    flag = 0
    for c in ls0:
        ls1 += c
        if flag > 0 and c != '.':
            flag += 1
        if c in '123456789' and flag == 0:
            flag = 1
        if flag == p:
            break
    # 
    us1 = ''
    flag = 0
    for c in us0:
        us1 += c
        if flag > 0:
            flag += 1
        if (not c in ['0', '.']) and flag == 0:
            flag = 1
        if flag == p:
            break
    # 
    us1len = len( us1 )
    ls1len = len( ls1 )
    le = max( [us1len, ls1len] )-2
    #
    us2 = '{:.{le}f}'.format( u, le=le )
    ls2 = '{:.{le}f}'.format( l, le=le )
    fs2 = '{:.{le}f}'.format( f, le=le )
    #
    if ls2 == us2:
        out = '{} \pm {}'.format( fs2, ls2 )
    else:
        out = '{}_{{-{}}}^{{+{}}}'.format( fs2, ls2, us2 )
    return out


def table():
    tex = False
    if 'tex' in sys.argv:
        tex = True
    fin = r.TFile( 'effy.root' )
    t = fin.Get( 't' )
    #################################################################
    # Table 1
    print CYELLOW + 'Table 1' + ENDC
    colsep = ' & ' if tex else ''
    if tex:
        print r'\begin{tabulary}{\textwidth}{'+'C|C|'+'C'*7+'}'
        print r'DSL3, \si{\mm} &  & 1 & 2 & 3 & 12 & 13 & 23 & 123 \\'
    for eve in t:
        if not tex:
            print eve.slit
            precol0 = ''
            precol1 = ''
        else:
            print r'\hline'
            precol0 = '\multirow{5}{*}{'+ '{:>3.0f}'.format(eve.slit) +'} & '
            precol1 = ' & '
        o1 = '{:>14} '.format( 'Hits w/ target' )
        o2 = '{:>14} '.format( 'All hits' )
        o3 = '{:>14} '.format( 'eff' )
        o4 = '{:>14} '.format( '$N_i / N_3$' )
        o5 = '{:>14} '.format( '$N_i / N_{23}$' )
        for i in xrange(7):
            o1 += colsep + '{:>10.0f} '.format( eve.np[i] )
            o2 += colsep + '{:>10.0f} '.format( eve.n[i] )
            e = getEffErr( eve.np[i], eve.n[i] )
            if tex:
                # o3 += colsep + '${:.4f}_{{-{:.4f}}}^{{+{:.4f}}}$ '.format( e[0], e[1], e[2] )
                o3 += colsep + '${}$ '.format( auf(e[0], e[1], e[2]) )
            else:
                o3 += '{:>10.4f} '.format( e[0] )
            o4 += colsep + '{:>10.3f} '.format( eve.n[i] / eve.n[2] )
            o5 += colsep + '{:>10.3f} '.format( eve.n[i] / eve.n[5] )
        if tex:
            o1 = precol0 + o1 + r' \\'
            o2 = precol1 + o2 + r' \\'
            o3 = precol1 + o3 + r' \\'
            o4 = precol1 + o4 + r' \\'
            o5 = precol1 + o5 + r' \\'
        #
        oo = [o1, o2, o3, o4, o5]
        #
        for o in oo:
            o = o.replace( '&', CBLUE+'&'+ENDC )
            print o
    if tex:
        print r'\end{tabulary}'
    #################################################################
    # Table 2
    print CYELLOW + '\nTable 2' + ENDC
    p1, p2 = 3, 4
    if tex:
        print r'\begin{tabulary}{\textwidth}{'+'C|C|'+'C'*7+'}'
        print r'DSL3, \si{\mm} &  & 1 & 2 & 3 & 12 & 13 & 23 & 123 \\'
    for eve in t:
        if not tex:
            print eve.slit
            precol0 = ''
            precol1 = ''
        else:
            print r'\hline'
            precol0 = '\multirow{2}{*}{'+ '{:>3.0f}'.format(eve.slit) +'} & '
            precol1 = ' & '
        if tex:
            o1 = r'$\varepsilon_{\text{tar}} \times 10^{'+ '{}'.format(p1) +'}$ '
            o2 = r'$\varepsilon_{\text{tot}} \times 10^{'+ '{}'.format(p2) +'}$ '
        else:
            o1 = '{:>14} '.format( 'eff tar' )
            o2 = '{:>14} '.format( 'eff tot' )
        for i in xrange(7):
            e = getEffErr( eve.np[i], eve.nmc )
            if tex:
                # o1 += ' & ' + '${:.5f}_{{{:.5f}}}^{{{:.5f}}}$ '.format( e[0], e[1], e[2] )
                o1 += ' & ' + '${}$ '.format( auf(e[0]*10**p1, e[1]*10**p1, e[2]*10**p1) )
            else:
                # o1 += '{:>10.8f} '.format( e[0] )
                o1 += '{} '.format( f2s(e[0], 10, 8) )
            e = getEffErr( eve.np[i], eve.ndet8 )
            if tex:
                # o2 += ' & ' + '${:.7f}_{{{:.7f}}}^{{{:.7f}}}$ '.format( e[0], e[1], e[2] )
                o2 += ' & ' + '${}$ '.format( auf(e[0]*10**p2, e[1]*10**p2, e[2]*10**p2) )
            else:
                # o2 += '{:>10.8f} '.format( e[0] )
                o2 += '{} '.format( f2s(e[0], 10, 8) )
        if tex:
            o1 = precol0 + o1 + r' \\'
            o2 = precol1 + o2 + r' \\'
        oo = [o1, o2]
        for o in oo:
            o = o.replace( '&', CBLUE+'&'+ENDC )
            print o
    if tex:
        print r'\end{tabulary}'
    #################################################################
    # Table 3
    print CYELLOW + '\nTable 3' + ENDC
    if tex:
        print r'\begin{tabulary}{\textwidth}{'+'CC}'
        print r'DSL3, \si{\mm} & $\varepsilon_{\mu, \, \text{tar}}$ \\'
    for eve in t:
        o1 = '{:>5.0f}'.format( eve.slit )
        e = getEffErr( eve.nmc, eve.ndet8 )
        if tex:
            o1 += ' & ' + '${}$ '.format( auf(e[0], e[1], e[2]) ) + r' \\'
        else:
            o1 += '   {:>10}'.format( f2s(e[0], 9, 7) )
            o1 += ' (-{:>10}, +{:>10})'.format( f2s(e[1], 9, 7), f2s(e[2], 9, 7) )
        print o1 
    if tex:
        print r'\end{tabulary}'


def f2s(f, l0=10, l1=4):
    s0 = '{:>{l0}.{l1}f}'.format(f, l0=l0, l1=l1)
    s1 = ''
    flag = False
    for i, x in enumerate( s0 ):
        if x in ['0', '.'] and not flag:
            s1 += x
        elif not x in ['0', '.'] and not flag:
            s1 += CGREEN + x
            flag = True
        else:
            s1 += x
    s1 += ENDC
    return s1


if __name__ == '__main__':
    # print auf( 0.123456, 0.001235423, 0.000912333 )
    if 'do' in sys.argv:
        main()
    elif 'draw' in sys.argv:
        draw()
    elif 'table' in sys.argv:
        table()
