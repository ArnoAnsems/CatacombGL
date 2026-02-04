// Copyright (C) 1993 - 2014 Flat Rock Software
// Copyright (C) 2018 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

// Based on LZHUF.C from the Catacomb Apocalypse Source Code.
// https://github.com/CatacombGames/CatacombApocalypse/blob/master/LZHUF.C

#include "Decompressor.h"
#include <string.h>
#include <cstring>

//===========================================================================
//
//											GLOBAL VARIABLES
//
//===========================================================================
/* pointing children nodes (child[], child[] + 1)*/

/* LZSS Parameters */

#define F				30									/* Size of look-ahead buffer */
#define THRESHOLD		2

/* Huffman coding parameters */

#define N_CHAR  		(256 - THRESHOLD + F)		/* character code (= 0..N_CHAR-1) */
#define TableSize 		(N_CHAR * 2 - 1)				/* Size of table */
#define RootPosition 	(TableSize - 1)							/* root position */
/* reaches to this value */

static int16_t child[TableSize];

//
// pointing parent nodes.
// area [T..(T + N_CHAR - 1)] are pointers for leaves
//

static int16_t parent[TableSize + N_CHAR];

static uint16_t freq[TableSize + 1];	/* cumulative freq table */

static uint8_t d_code[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
    0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
    0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
    0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
    0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
    0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
    0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

static uint8_t d_len[256] = {
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

static uint16_t getbuf = 0;
static uint8_t getlen = 0;


//---------------------------------------------------------------------------
//  StartHuff    /* initialize freq tree */
//---------------------------------------------------------------------------
static void StartHuff()
{
    int16_t i, j;

    for (i = 0; i < N_CHAR; i++) {
        freq[i] = 1;
        child[i] = i + TableSize;
        parent[i + TableSize] = i;
    }
    i = 0; j = N_CHAR;
    while (j <= RootPosition) {
        freq[j] = freq[i] + freq[i + 1];
        child[j] = i;
        parent[i] = parent[i + 1] = j;
        i += 2; j++;
    }
    freq[TableSize] = 0xffff;
    parent[RootPosition] = 0;
}

//---------------------------------------------------------------------------
//   reconst        /* reconstruct freq tree */
//---------------------------------------------------------------------------
static void reconst()
{
    uint16_t k;
    int16_t i, j;

    /* halven cumulative freq for leaf nodes */

    j = 0;

    for (i = 0; i < TableSize; i++)
    {
        if (child[i] >= TableSize)
        {
            freq[j] = (freq[i] + 1) / 2;
            child[j] = child[i];
            j++;
        }
    }

    /* make a tree : first, connect children nodes */

    for (i = 0, j = N_CHAR; j < TableSize; i += 2, j++)
    {
        k = i + 1;
        uint16_t f = freq[j] = freq[i] + freq[k];

        for (k = j - 1;f < freq[k]; k--);

        k++;
        const uint16_t length = (j - k) * 2;

        (void)memmove(&freq[k + 1], &freq[k], length);
        freq[k] = f;

        (void)memmove(&child[k + 1], &child[k], length);
        child[k] = i;
    }

    /* connect parent nodes */

    for (i = 0; i < TableSize; i++)
    {
        if ((k = child[i]) >= TableSize)
        {
            parent[k] = i;
        }
        else
        {
            parent[k] = parent[k + 1] = i;
        }
    }
}



//---------------------------------------------------------------------------
//  update()	 update freq tree
//---------------------------------------------------------------------------
static void update(int16_t c)
{
    const uint16_t MAX_FREQ = 0x8000;  // update when cumulative frequency
    if (freq[RootPosition] == MAX_FREQ)
    {
        reconst();
    }

    c = parent[c + TableSize];

    do {
        const int16_t k = ++freq[c];

        //
        // swap nodes to keep the tree freq-ordered
        //
        int16_t l = c + 1;
        if (k > freq[l])
        {
            while (k > freq[++l]);

            l--;
            freq[c] = freq[l];
            freq[l] = k;

            const int16_t i = child[c];
            parent[i] = l;
            if (i < TableSize)
                parent[i + 1] = l;

            const int16_t j = child[l];
            child[l] = i;

            parent[j] = c;
            if (j < TableSize)
                parent[j + 1] = c;

            child[c] = j;

            c = l;
        }
    } while ((c = parent[c]) != 0);	/* do it until reaching the root */
}

//---------------------------------------------------------------------------
// GetByte
//---------------------------------------------------------------------------
static int16_t GetByte(uint8_t **infile_ptr, uint32_t *CompressLength)
{
    uint16_t i;

    while (getlen <= 8)
    {
        if (*CompressLength)
        {
            i = (uint8_t)*((*infile_ptr)++);
            (*CompressLength)--;
        }
        else
            i = 0;

        getbuf |= i << (8 - getlen);
        getlen += 8;
    }

    i = getbuf;
    getbuf <<= 8;
    getlen -= 8;
    return i>>8;
}

//---------------------------------------------------------------------------
// GetBit
//---------------------------------------------------------------------------
static int16_t GetBit(uint8_t **infile_ptr, uint32_t *CompressLength)	/* get one bit */
{
    int16_t i;

    while (getlen <= 8)
    {
        if (*CompressLength)
        {
            i = (uint8_t)*((*infile_ptr)++);
            (*CompressLength)--;
        }
        else
            i = 0;

        getbuf |= i << (8 - getlen);
        getlen += 8;
    }

    i = getbuf;
    getbuf <<= 1;
    getlen--;
    return (i < 0);
}

//---------------------------------------------------------------------------
// DecodeChar
//---------------------------------------------------------------------------
static int16_t DecodeChar(uint8_t **infile_ptr, uint32_t *CompressLength)
{
	uint16_t c = child[RootPosition];

	/*
	 * start searching tree from the root to leaves.
	 * choose node #(son[]) if input bit == 0
	 * else choose #(son[]+1) (input bit == 1)
	 */

	while (c < TableSize)
	{
		c += GetBit(infile_ptr,CompressLength);
		c = child[c];
	}

	c -= TableSize;
	update(c);
	return c;
}

//---------------------------------------------------------------------------
// DecodePosition
//---------------------------------------------------------------------------
static int16_t DecodePosition(uint8_t **infile_ptr,uint32_t *CompressLength)
{
	//
	// decode upper 6 bits from given table
	//

	uint16_t i = GetByte(infile_ptr, CompressLength);
	const uint16_t c = (uint16_t)d_code[i] << 6;
	uint8_t j = d_len[i] - 2;

	//
	// input lower 6 bits directly
	//

	while (j--)
	{
		i = (i << 1) + GetBit(infile_ptr, CompressLength);
	}

	return c | i & 0x3f;
}

//---------------------------------------------------------------------------
// lzhDecompress()
//---------------------------------------------------------------------------
uint32_t Decompressor::lzhDecompress(uint8_t* infile, uint8_t* outfile, uint32_t OrginalLength, uint32_t CompressLength)
{
    uint32_t count;

    getbuf = 0;
    getlen = 0;

    if (OrginalLength == 0)
    {
        return 0;
    }

    StartHuff();

    const uint16_t N = 4096;	// Size of string buffer
    uint8_t text_buf[N + F - 1];
    memset(text_buf, ' ', N - F);

    int16_t r = N - F;

    for (count = 0; count < OrginalLength; )
    {
        const int16_t c = DecodeChar(&infile,&CompressLength);

        if (c < 256)
        {
            *outfile++ = (uint8_t)c;

            text_buf[r++] = (uint8_t)c;
            r &= (N - 1);
            count++;									// inc count of bytes written
        }
        else
        {
            const int16_t position = (r - DecodePosition(&infile,&CompressLength) - 1) & (N - 1);
            const int16_t j = c - 255 + THRESHOLD;

            for (int16_t k = 0; k < j; k++)
            {
                const uint8_t charFromBuffer = text_buf[(position + k) & (N - 1)];
                *outfile++ = charFromBuffer;

                text_buf[r++] = charFromBuffer;
                r &= (N - 1);
                count++;								// inc count of bytes written
            }
        }
    }

    return(count);
}


void Decompressor::lzwDecompress(uint8_t* infile, uint8_t* outfile, uint32_t OrginalLength, uint32_t CompressLength)
{
    uint32_t i, j, k, r, c;
    uint16_t flags;

    constexpr uint32_t n_lzw = 4096;
    constexpr uint32_t f_lzw = 18;

#define nextch(ptr)	*ptr++
    uint8_t text_buf[n_lzw + f_lzw - 1];

    for (i = 0; i < n_lzw - f_lzw; i++)
        text_buf[i] = ' ';

    r = n_lzw - f_lzw;
    flags = 0;

    for (; ; )
    {
        if (((flags >>= 1) & 256) == 0)
        {
            if (!(CompressLength--))
                break;
            c = nextch(infile);

            flags = c | 0xff00;      /* uses higher byte cleverly */
        }                                  /* to count eight */

        if (flags & 1)
        {
            if (!(CompressLength--))
                break;
            c = nextch(infile);

            *outfile++ = c;
            text_buf[r++] = c;
            r &= (n_lzw - 1);
        }
        else
        {
            if (!(CompressLength--))
                break;
            i = nextch(infile);

            if (!(CompressLength--))
                break;
            j = nextch(infile);

            i |= ((j & 0xf0) << 4);
            j = (j & 0x0f) + THRESHOLD;

            for (k = 0; k <= j; k++)
            {
                c = text_buf[(i + k) & (n_lzw - 1)];
                *outfile++ = c;
                text_buf[r++] = c;
                r &= (n_lzw - 1);
            }
        }
    }
}

FileChunk* Decompressor::RLEW_Decompress(
    const uint16_t* compressedWords,
    const uint16_t compressedSizeInWords,
    const uint16_t maxDecompressedSizeInWords,
    const uint16_t rlewtag)
{
    FileChunk* decompressedChunk = nullptr;
    uint16_t* source = (uint16_t*)compressedWords;

    uint16_t* tempDecompressedData = new uint16_t[maxDecompressedSizeInWords];
    uint16_t* destination = tempDecompressedData;
    uint16_t destinationIndex = 0u;
    uint16_t sourceIndex = 0u;

    do
    {
        uint16_t value = source[sourceIndex];
        sourceIndex++;
        if (value != rlewtag)
        {
            // Uncompressed
            destination[destinationIndex] = value;
            destinationIndex++;
        }
        else
        {
            // Compressed string
            const uint16_t count = source[sourceIndex];
            sourceIndex++;
            value = source[sourceIndex];
            sourceIndex++;
            for (uint16_t i = 0; i < count; i++)
            {
                destination[destinationIndex] = value;
                destinationIndex++;
            }
        }
    } while (sourceIndex < compressedSizeInWords && destinationIndex < maxDecompressedSizeInWords);

    const uint16_t decompressedSizeInBytes = destinationIndex * sizeof(uint16_t);
    decompressedChunk = new FileChunk(decompressedSizeInBytes);
    std::memcpy(decompressedChunk->GetChunk(), tempDecompressedData, decompressedSizeInBytes);
    delete[] tempDecompressedData;

    return decompressedChunk;
}

FileChunk* Decompressor::CarmackExpand(const uint8_t* compressedChunk)
{
    const uint16_t NEARTAG = 0xa7;
    const uint16_t FARTAG = 0xa8;

    uint8_t *inptr = (uint8_t*)compressedChunk;
    const uint16_t lengthInBytes = *(uint16_t*)compressedChunk;
    inptr += 2;

    FileChunk* decompressedChunk = new FileChunk(lengthInBytes);
    uint16_t *outptr = (uint16_t*)(decompressedChunk->GetChunk());

    uint16_t remainingLengthInWords = lengthInBytes / 2;

    while (remainingLengthInWords)
    {
        uint16_t ch = *(uint16_t*)(inptr);
        inptr += 2;
        const uint16_t chhigh = ch >> 8;
        uint16_t count = ch & 0xff;
        if (chhigh == NEARTAG)
        {
            if (!count)
            {
                // have to insert a word containing the tag byte
                ch |= *inptr++;
                *outptr++ = ch;
                remainingLengthInWords--;
            }
            else
            {
                const uint16_t offset = *inptr++;
                uint16_t *copyptr = outptr - offset;
                remainingLengthInWords -= count;
                while (count--)
                    *outptr++ = *copyptr++;
            }
        }
        else if (chhigh == FARTAG)
        {
            if (!count)
            {				// have to insert a word containing the tag byte
                ch |= *inptr++;
                *outptr++ = ch;
                remainingLengthInWords --;
            }
            else
            {
                const uint16_t offset = *(uint16_t*)(inptr);
                inptr += 2;
                uint16_t *copyptr = (uint16_t*)(decompressedChunk->GetChunk()) + offset;
                remainingLengthInWords -= count;
                while (count--)
                    *outptr++ = *copyptr++;
            }
        }
        else
        {
            *outptr++ = ch;
            remainingLengthInWords --;
        }
    }

    return decompressedChunk;
}