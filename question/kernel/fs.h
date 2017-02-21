#ifndef __FS_H
#define __FS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "disk.h"
#include "bitmap.h"

// Inode constatnts
#define MAX_INODE_BLOCKS 6

// Super Block constants
#define MAX_NUMBER_OF_BLOCKS 65536
#define BLOCK_BITMAP_SIZE ((int) (MAX_NUMBER_OF_BLOCKS) / (BITS_PER_WORD))

// inode types
#define INODE_UNALLOCATED 0
#define INODE_FILE 1
#define INODE_DIRECTORY 2

typedef struct {
    int disk_block_num;
    int disk_block_length;
    int inode_num;
    uint32_t inode_start;
    uint32_t data_block_start;
    uint32_t free_block_bitmap[BLOCK_BITMAP_SIZE];
} superblock_t;

typedef struct {
    int id;
    int type;
    uint32_t size;
    uint32_t blocks_allocated;
    uint32_t creation_time;
    uint32_t modification_time;
    uint32_t blocks[MAX_INODE_BLOCKS];
} inode_t;

#endif
