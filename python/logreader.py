#!/usr/bin/python

import io
import sys

def read_number( fi ):

    s = ''
    nextchar = fi.peek( 1 )[0] # peek 1 character
    while nextchar.isdigit():
        s += nextchar
        fi.read( 1 ) # consume the character
        nextchar = fi.peek( 1 )[0]

    return int(s)

def process_line( fi, line ):

    of = sys.stdout

    if line[0] == '@':
        of.write( "command line:%s" % line )
        nextchar = fi.read( 1 )
        if nextchar == '#':
            num = read_number( fi )
            data = fi.read( num )
            of.write( "\t(data:%s)\n" % data )
        else:
            of.write( "\t(no data)\n" )
    else:
        of.write( "skipping line:%s" % line )

if __name__ == "__main__":

    f = io.open( "testfile", "rb" )
    fi = io.BufferedReader( f ) # BufferedReader provides peek function

    for line in fi:
        process_line( fi, line )
