/*******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Jean-David Gadina - www.xs-labs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#ifndef UB_FAT_FUNCTIONS_HPP
#define UB_FAT_FUNCTIONS_HPP

#include <cstdint>

namespace UB
{
    namespace FAT
    {
        class MBR;

        struct DAP
        {
            uint8_t  size;
            uint8_t  zero;
            uint16_t numberOfSectors;
            uint32_t segmentOffsetPointer;
            uint64_t logicalBlockAddress;
        };

        struct CHS
        {
            uint16_t cylinder;
            uint8_t  head;
            uint8_t  sector;
        };

        uint64_t   chsToLBA( const MBR & mbr, uint8_t cylinder, uint8_t sector, uint8_t head );
        struct CHS lbaToCHS( const MBR & mbr, uint64_t lba );
    }
}

#endif /* UB_FAT_FUNCTIONS_HPP */
