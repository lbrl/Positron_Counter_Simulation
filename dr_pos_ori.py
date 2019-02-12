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



def read_tubs():
    finname = 'beamtest2017B-tubs.tsv'
    fin = open( finname )
    out = []
    for line in fin:
        if len( line ) < 3:
            continue
        if '#' == line[0]:
            continue
        lin = line.rstrip( '\n' ).rstrip( '\r' ).split( '\t' )
        if lin[3] == '':
            continue
        x = {}
        try:
            x['name'] = lin[9]
            x['rin'] = float( lin[1] )
            x['rout'] = float( lin[2] )
            x['length'] = float( lin[4] )
            x['material'] = lin[5]
            x['zcentre'] = float( lin[6] )
            x['zupstream'] = float( lin[7] )
            x['zdownstream'] = float( lin[8] )
            out.append( x )
        except ValueError:
            print 'ERROR'
            print lin
    return out


def getCanvas( name, cw=600, ch=600, margins=-1 ):
    c1 = r.TCanvas( name, name, cw, ch )
    if not '-b' in sys.argv:
        c1.SetWindowSize(cw + (cw - c1.GetWw()), ch + (ch - c1.GetWh()));
    else:
        c1.SetCanvasSize(cw, ch)
    if margins != -1:
        c1.SetLeftMargin( margins[0] )
        c1.SetBottomMargin( margins[1] )
        c1.SetRightMargin( margins[2] )
        c1.SetTopMargin( margins[3] )
    return c1


hp = r.TH1I( 'hp', 'hp', 1, 0, 1 )
ht = r.TH1I( 'hp', 'hp', 1, 0, 1 )
def getEffErr( p, t ):
    hp.SetBinContent( 1, p )
    ht.SetBinContent( 1, t )
    eff = r.TEfficiency( hp, ht )
    eu = eff.GetEfficiencyErrorUp( 1 )
    el = eff.GetEfficiencyErrorLow( 1 )
    return [el, eu]


def getHits( t, strict='loose', isTot=False, af=-1 ):
    # cutplate = '10<OriginX&&OriginX<60 && -20<OriginY&&OriginY<20 && 10<OriginZ&&OriginZ<60'
    # cutplate = '10<OriginX&&OriginX<60 && -20<OriginY&&OriginY<20'
    if strict == 'loose':
        cutplate = '10<OriginY&&OriginY<60 && -20<OriginX&&OriginX<20'
    elif strict == 'normal':
        cutplate = '20<OriginY&&OriginY<50 && -10<OriginX&&OriginX<13'
    else:
        return 2
    print strict
    cut1 = 'PC1Edep>200'
    cut2 = 'PC2Edep>200'
    cut3 = 'PC3Edep>200'
    #
    n1 = t.Draw( '', cut1, 'goff' )
    n1p = t.Draw( '', cut1+' && '+cutplate, 'goff' )
    n2 = t.Draw( '', cut2, 'goff' )
    n2p = t.Draw( '', cut2+' && '+cutplate, 'goff' )
    n3 = t.Draw( '', cut3, 'goff' )
    n3p = t.Draw( '', cut3+' && '+cutplate, 'goff' )
    #
    n12 = t.Draw( '', cut1+' && '+cut2, 'goff' )
    n12p = t.Draw( '', cut1+' && '+cut2+' && '+cutplate, 'goff' )
    n13 = t.Draw( '', cut1+' && '+cut3, 'goff' )
    n13p = t.Draw( '', cut1+' && '+cut3+' && '+cutplate, 'goff' )
    n23 = t.Draw( '', cut2+' && '+cut3, 'goff' )
    n23p = t.Draw( '', cut2+' && '+cut3+' && '+cutplate, 'goff' )
    #
    n123 = t.Draw( '', cut1+' && '+cut2+' && '+cut3, 'goff' )
    n123p = t.Draw( '', cut1+' && '+cut2+' && '+cut3+' && '+cutplate, 'goff' )
    #
    foilhit = isTot
    if isTot:
        e1 = getEffErr(n1p, foilhit)
        e2 = getEffErr(n2p, foilhit)
        e3 = getEffErr(n3p, foilhit)
        e12 = getEffErr(n12p, foilhit)
        e13 = getEffErr(n13p, foilhit)
        e23 = getEffErr(n23p, foilhit)
        e123 = getEffErr(n123p, foilhit)
    else:
        e1 = getEffErr(n1p, n1)
        e2 = getEffErr(n2p, n2)
        e3 = getEffErr(n3p, n3)
        e12 = getEffErr(n12p, n12)
        e13 = getEffErr(n13p, n13)
        e23 = getEffErr(n23p, n23)
        e123 = getEffErr(n123p, n123)
    #
    if isTot:
        pri( '1', n1p, foilhit, 100.*n1p/foilhit, 100.*e1[0], 100.*e1[1], isTot, af=af )
        pri( '2', n2p, foilhit, 100.*n2p/foilhit, 100.*e2[0], 100.*e2[1], isTot, af=af )
        pri( '3', n3p, foilhit, 100.*n3p/foilhit, 100.*e3[0], 100.*e3[1], isTot, af=af )
        pri( '12', n12p, foilhit, 100.*n12p/foilhit, 100.*e12[0], 100.*e12[1], isTot, af=af )
        pri( '13', n13p, foilhit, 100.*n13p/foilhit, 100.*e13[0], 100.*e13[1], isTot, af=af )
        pri( '23', n23p, foilhit, 100.*n23p/foilhit, 100.*e23[0], 100.*e23[1], isTot, af=af )
        pri( '123', n123p, foilhit, 100.*n123p/foilhit, 100.*e123[0], 100.*e123[1], isTot, af=af )
    else:
        pri( '1', n1p, n1, 100.*n1p/n1, 100.*e1[0], 100.*e1[1], isTot )
        pri( '2', n2p, n2, 100.*n2p/n2, 100.*e2[0], 100.*e2[1], isTot )
        pri( '3', n3p, n3, 100.*n3p/n3, 100.*e3[0], 100.*e3[1], isTot )
        pri( '12', n12p, n12, 100.*n12p/n12, 100.*e12[0], 100.*e12[1], isTot )
        pri( '13', n13p, n13, 100.*n13p/n13, 100.*e13[0], 100.*e13[1], isTot )
        pri( '23', n23p, n23, 100.*n23p/n23, 100.*e23[0], 100.*e23[1], isTot )
        pri( '123', n123p, n123, 100.*n123p/n123, 100.*e123[0], 100.*e123[1], isTot )


def pri( name, np, n, npn, el, eu, isTot=False, isLatex=False, af=-1):
    if 'latex' in sys.argv:
        isLatex = True
    amper = CGREEN + '&' + ENDC
    if isLatex:
        out = '{:>10} {amper} '.format( name, amper=amper )
        out += '\\num{{ {} }} {amper} \\num{{ {} }} {amper} '.format(np, n, amper=amper)
        out += '$'
        if af < 0:
            out += '{:.3f}_{{-{:.3f}}}^{{+{:.3f}}}'.format(npn, el, eu)
        else:
            out += '{:.{af}f}_{{-{:.{af}f}}}^{{+{:.{af}f}}}'.format(npn, el, eu, af=af)
        out += r' \, \si{\percent}$ \\'
    else:
        out = '{:>10} : '.format( name )
        if isTot:
            if af < 0:
                out += '{}  /  {}  =  {:.3f} - {:.3f} + {:.3f}  %%'.format(np, n, 10*npn, 10*el, 10*eu)
            else:
                out += '{}  /  {}  =  {:.{af}f} - {:.{af}f} + {:.{af}f}  %%'.format(np, n, 10*npn, 10*el, 10*eu, af=af)
        else:
            out += '{}  /  {}  =  {:.2f} - {:.2f} + {:.2f}  %'.format(np, n, npn, el, eu)
    print out
    return
    out = '{:>10} : '.format( name )
    out += '{:>10} / {:<10} = {:>10.2f}'.format( np, n, npn )
    le = len(out)
    outu = ' '*le + '+ {:<10.2f}'.format( eu )
    outl = ' '*le + '- {:<10.2f}'.format( el )
    print outu
    print out + '        %'
    print outl


def main():
    tubs = read_tubs()
    Fin = r.TFile( 'build/Simulation.root' )
    t = Fin.Get( 'PC' )
    #
    getHits( t, 'loose' )
    getHits( t, 'normal' )
    getHits( t, 'loose', 32677 )
    getHits( t, 'normal', 32677 )
    getHits( t, 'loose', 1632287, af=5 )
    getHits( t, 'normal', 1632287, af=5 )
    #
    # c1 = getCanvas( 'c1', 9*120, 9*110, [.1/(12./11), .1, .1/(12./11), .1] )
    c1 = getCanvas( 'c1', 9*120, 9*90, [.1/(12./9), .1, .1/(12./9), .1] )
    #
    cut = ''
    sel = 'OriginX:OriginY'
    hopt = '(120,-500,700,90,-500,400)'
    hopt = '(120,-250,350,90,-250,200)'
    t.Draw( '{sel}>>h{hopt}'.format( sel=sel, hopt=hopt ), cut )
    #
    cut = 'PC1Edep>200 || PC2Edep>200 || PC3Edep>200'
    t.SetMarkerStyle( 7 )
    t.SetMarkerColor( r.kMagenta )
    t.Draw( sel, cut, 'same' )
    #
    cut = 'PC2Edep>200 && PC3Edep>200'
    t.SetMarkerColor( r.kBlue )
    t.SetMarkerStyle( 24 )
    t.Draw( sel, cut, 'same' )
    #
    cut = 'PC1Edep>200 && PC2Edep>200 && PC3Edep>200'
    t.SetMarkerColor( r.kGreen )
    t.SetMarkerStyle( 24 )
    t.Draw( sel, cut, 'same' )
    #
    line = r.TLine()
    line.SetLineColor( r.kRed )
    line.SetLineWidth( 2 )
    z0 = 1136.7+34.55+9.2+5.2
    k = 1
    for tub in tubs:
        rin = tub['rin']
        rout = tub['rout']
        length=tub['length']
        z = tub['zcentre']
        # Top
        line.DrawLine( k*(z-z0-length/2.), rin*k, k*(z-z0-length/2.), rout*k )
        line.DrawLine( k*(z-z0-length/2.), rout*k, k*(z-z0+length/2.), rout*k )
        line.DrawLine( k*(z-z0+length/2.), rin*k, k*(z-z0+length/2.), rout*k )
        line.DrawLine( k*(z-z0-length/2.), rin*k, k*(z-z0+length/2.), rin*k )
        # Bottom
        line.DrawLine( k*(z-z0-length/2.), -rin*k, k*(z-z0-length/2.), -rout*k )
        line.DrawLine( k*(z-z0-length/2.), -rout*k, k*(z-z0+length/2.), -rout*k )
        line.DrawLine( k*(z-z0+length/2.), -rin*k, k*(z-z0+length/2.), -rout*k )
        line.DrawLine( k*(z-z0-length/2.), -rin*k, k*(z-z0+length/2.), -rin*k )
    lat = r.TLatex()
    lat.SetTextFont( 12 )
    lat.SetTextSize( .03 )
    # lat.DrawLatexNDC( .2, .92, 'Positron origin, 10 million e^{+} are simulated' )
    lat.DrawLatexNDC( .2, .92, 'Positron origin' )
    lat.DrawLatexNDC( .32, .31, 'All positrons' )
    lat.SetTextColor( r.kGreen+1 )
    lat.DrawLatexNDC( .32, .28, r'Positrons which hit all three positron counters with E_{dep, i}>200 keV' )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .32, .25, r'Beam pipe' )
    lat.SetTextColor( r.kBlack )
    lat.DrawLatexNDC( .6, .025, 'Beam/horizontal direction, mm' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .025, .6, 'Vertical direction, mm' )
    c1.Update()
    #
    c2 = getCanvas( 'c2' )
    r.gStyle.SetOptStat( 0 )
    t.SetLineWidth( 2 )
    t.SetLineColor( r.kGreen+2 )
    t.Draw( 'PC2Edep >> h2(100,0,3000)', 'PC2Edep>0' )
    t.SetLineColor( r.kRed )
    t.Draw( 'PC1Edep', 'PC1Edep>0', 'same' )
    t.SetLineColor( r.kBlue )
    t.Draw( 'PC3Edep', 'PC3Edep>0', 'same' )
    lat.SetTextColor( r.kBlack )
    lat.SetTextSize( .04 )
    lat.SetTextAngle( 0 )
    lat.DrawLatexNDC( .8, .025, r'E_{dep}, keV' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .0225, .8, 'Entries' )
    lat.SetTextAngle( 0 )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .7, .85, 'PC1, small' )
    lat.SetTextColor( r.kGreen+2 )
    lat.DrawLatexNDC( .7, .80, 'PC2, thick' )
    lat.SetTextColor( r.kBlue )
    lat.DrawLatexNDC( .7, .75, 'PC3, thin' )
    c2.Update()
    #
    c3 = getCanvas( 'c3' )
    r.gStyle.SetOptStat( 0 )
    t.SetLineWidth( 2 )
    t.SetLineColor( r.kGreen+2 )
    t.Draw( 'PC2Time >> h3(100,0,15000)', 'PC2Edep>0' )
    t.SetLineColor( r.kRed )
    t.Draw( 'PC1Time', 'PC1Edep>0', 'same' )
    t.SetLineColor( r.kBlue )
    t.Draw( 'PC3Time', 'PC3Edep>0', 'same' )
    lat.SetTextColor( r.kBlack )
    lat.SetTextSize( .04 )
    lat.SetTextAngle( 0 )
    lat.DrawLatexNDC( .8, .025, r'Time, ns' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .0225, .8, 'Entries' )
    lat.SetTextAngle( 0 )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .7, .85, 'PC1, small' )
    lat.SetTextColor( r.kGreen+2 )
    lat.DrawLatexNDC( .7, .80, 'PC2, thick' )
    lat.SetTextColor( r.kBlue )
    lat.DrawLatexNDC( .7, .75, 'PC3, thin' )
    c3.Update()
    #
    if not '-b' in sys.argv:
        raw_input( 'Press ENTER to continue, please.' )
    if 'save' in sys.argv:
        c1.SaveAs( 'originx_originy.png' )
        c2.SaveAs( 'edep.png' )
        c3.SaveAs( 'time.png' )


if __name__ == '__main__':
    main()
