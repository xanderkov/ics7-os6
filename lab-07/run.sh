#!/bin/bash
set -Eeuxo pipefail

make
sudo insmod seqfile.ko
lsmod | grep seqfile

sudo dmesg | grep seqfile

echo "123" > /proc/seqfiles/seqfile
echo "chto-to" > /proc/seqfiles/seqfile
echo "aa bbb ccccc jjjjjjjjjjj" > /proc/seqfiles/seqfile

cat /proc/seqfiles/seqfile
cat /proc/seqfiles/seqfile
cat /proc/seqfiles/seqfile

ls -ail /proc | grep seqfile

sudo rmmod seqfile

sudo dmesg | grep seqfile

make clean