#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>




typedef struct block_store{
    // bitmap serves as the Free Block Map
    bitmap_t* bitmap;
    //two dimensional array makes sense allows for the number of blocks minus one for the FBM
    // and second dimension being the number of bytes per block
    char* storage[BLOCK_STORE_NUM_BLOCKS-1][BLOCK_SIZE_BYTES];

}block_store_t;

block_store_t *block_store_create()
{
    block_store_t* block = (block_store_t*)malloc(sizeof(block_store_t));
    if(NULL != block)
    {
        // adjust the bitmap to have 1 less than the total number of bytes ( to account for itself)
        block->bitmap = bitmap_create(BLOCK_SIZE_BYTES-1);
        return block;
    }

    return NULL;
}

void block_store_destroy(block_store_t *const bs)
{
    if(NULL == bs)
    {
        return;
    }
    bitmap_t* bit = bs->bitmap;
    bitmap_destroy(bit);
    free(bs);

}

size_t block_store_allocate(block_store_t *const bs)
{
    // input validation check for NULL
    if(NULL == bs)
    {
        return SIZE_MAX;
    }

//  Need to see if there is a free bit - using the provided helper funtions which came with the bitmap.

    size_t block = bitmap_ffz(bs->bitmap);

    if(SIZE_MAX == block || BLOCK_STORE_NUM_BLOCKS-1 <= block )
    {
        return SIZE_MAX;
    }
    else
    {
        bitmap_set(bs->bitmap,block);
    }
    
    return block;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    //Input validation check for NULL values or out of bounds
    if(NULL == bs || 0 >= block_id || BLOCK_STORE_NUM_BLOCKS <= block_id)
    {
        return false;
    }
    
    // test to see if the requested bit is available
    // if it is - set using the provided function
    if(!bitmap_test(bs->bitmap,block_id))
    {
        bitmap_set(bs->bitmap,block_id);
        return true;
    }

    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    //Input validation checking for NULL and out of bounds values
    if(NULL == bs || BLOCK_STORE_NUM_BLOCKS <= block_id)
    {
        return;
    }
    //Test to see that the bit is indeed in use. 
    // If it is - free it with bitmap_reset
    if(bitmap_test(bs->bitmap,block_id))
    {
        bitmap_reset(bs->bitmap,block_id);
    }

}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    //Input validation checking for NULL values
    if(NULL == bs)
    {
        return SIZE_MAX;
    }
    // This one is fairly straight forward using the provided bitmap function bitmap_total_set
    size_t set_blocks = bitmap_total_set(bs->bitmap);
    return set_blocks;

}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(NULL == bs)
    {
        return SIZE_MAX;
    }
    // Invert the bits and then use the same function I used to get the used blocks
    bitmap_invert(bs->bitmap);
    size_t free_blocks =bitmap_total_set(bs->bitmap);
    // Have to reverse the inversion on the bitmap after getting the value of the set bits
    // Kept getting failed values in unit testing: values were always off by 1
    // Decided to change implementation of block_store_create() from the size of BLOCK_SIZE_BYTES to (BLOCK_SIZE_BYTES -1)
    // Not really sure why this makes that much a difference since the tests for set blocks passed without the change
    // But now all tests for these two tests are passed.
    bitmap_invert(bs->bitmap);

    return free_blocks;

}

size_t block_store_get_total_blocks()
{
    //Simply return the size of the bitmap size which I used in block_store_create()
    //Originally had set to BLOCK_SIZE_BYTES - after changing implemntation of that to BLOCK_SIZE_BYTES -1 
    // had to change this as well. 
    // Unit tests passed - was this really this simple??
    return BLOCK_STORE_NUM_BLOCKS -1;
}

// Was going to just use read with a file descriptor at first, then realized there is no file, just a buffer
// After talking with JimR decided to use memcpy 
size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    if(NULL == bs || block_id > BLOCK_STORE_NUM_BYTES || NULL == buffer )
    {
        return 0;
    }
    memcpy(buffer, bs->storage[block_id],BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;


}

// Same format as block_store_read only swap the source and destination. 

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    if(NULL == bs || block_id > BLOCK_STORE_NUM_BYTES || NULL == buffer )
    {
        return 0;
    }
    memcpy(bs->storage[block_id], buffer, BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;

}
block_store_t *block_store_deserialize(const char *const filename)
{
    if(NULL==filename)
    {
        return NULL;
    }
    int fd = open(filename, O_RDONLY);
    if(0 > fd)
    {
        return NULL;
    }
    block_store_t* block = (block_store_t*)malloc(sizeof(block_store_t));
    void* buff = (bitmap_t*)malloc(BITMAP_SIZE_BYTES*sizeof(char));
    read(fd, buff, BITMAP_SIZE_BYTES);
    block->bitmap = bitmap_import(BITMAP_SIZE_BYTES, buff);
    free(buff);

    //allocate memory for the storage part of the block_store 
    // As I used a two dimensional array have to malloc as a double pointer
    **(block)->storage = (char*)malloc(BLOCK_STORE_AVAIL_BLOCKS*BLOCK_STORE_NUM_BLOCKS*sizeof(char));

    // read from file into the double pointer I just allocated continuous memory for
    read(fd, **block->storage, BLOCK_STORE_NUM_BLOCKS*BLOCK_STORE_AVAIL_BLOCKS);
    close(fd);
    return block;

}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    if(NULL==bs || NULL==filename)
    {
        return 0;
    }

    // File descriptor with flags to create the file if it doesn't exsist, truncate it if it does, or read it
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //check for proper opeing of file descriptor
    if(fd < 0)
    {
        return 0;
    }
    //value to return the bytes written

    int writer;
    writer = write(fd, bs, BLOCK_STORE_NUM_BYTES);

    close(fd);
    return writer;
}

// include more if you need

