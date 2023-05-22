#!/bin/bash

make
sudo insmod vfs.ko
lsmod | grep vfs

sudo dmesg | grep vfs

sudo cat /proc/slabinfo | grep SLABcache

touch image
mkdir dir
sudo mount -o loop -t my_fs ./image ./dir 

mkdir dir2
sudo mount -o loop -t my_fs ./image ./dir2

stat --format=%t --file-system ./dir

sudo umount ./dir
sudo umount ./dir2

sudo rmmod vfs

sudo dmesg | grep MYFS

make clean