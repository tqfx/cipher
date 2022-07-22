#!/usr/bin/env bash

usage()
{
    echo "USAGE: $0 <path-to-cipher>"
    exit
}

if [ $# == 0 ]; then
    usage
fi

bin=$1
cwd=$(dirname $bin)
base=$(basename $bin)
json=$cwd/$base.json
name=$cwd/$base.db

rm -f $name
$bin -f $name -s -k* -p*
$bin -f $name -c -k k0 -p p0 -t 0 -h h0 -l 0xFF -a MD5
$bin -f $name -c -k k1 -p p1 -t 1 -h h1 -l 0xFF -a SHA1
$bin -f $name -c -k k2 -p p2 -t 2 -h h2 -l 0xFF -a BLAKE2B -m m
$bin -f $name -s -k* -p*
$bin -f $name -d -k k2 -p p2
$bin -f $name -di -k k2 -p p2
$bin -f $name -di -k k1 -p p1
$bin -f $name -si -k0 -p0

rm -f $name
$bin -f $name -c -k k0 -p p0 -t 0 -h h0 -l 0xFF -a MD5
$bin -f $name -c -k k1 -p p1 -t 1 -h h1 -l 0xFF -a SHA1
$bin -f $name -c -k k2 -p p2 -t 2 -h h2 -l 0xFF -a BLAKE2B -m m
$bin -f $name --export $json

rm -f $name
$bin -f $name --import $json
$bin -f $name -s -k* -p*

rm -f $name
