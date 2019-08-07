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

#include "UB/FAT/Functions.hpp"
#include "UB/FAT/MBR.hpp"
#include "UB/Casts.hpp"

namespace UB
{
    namespace FAT
    {
        uint64_t chsToLBA( const MBR & mbr, uint8_t cylinder, uint8_t sector, uint8_t head )
        {
            uint16_t hpc( mbr.headsPerCylinder() );
            uint16_t spt( mbr.sectorsPerTrack() );

            return ( ( ( numeric_cast< uint64_t >( cylinder ) * numeric_cast< uint64_t >( hpc ) ) + numeric_cast< uint64_t >( head ) ) * numeric_cast< uint64_t >( spt ) ) + ( numeric_cast< uint64_t >( sector ) - 1 );
        }

        struct CHS lbaToCHS( const MBR & mbr, uint64_t lba )
        {
            uint16_t hpc( mbr.headsPerCylinder() );
            uint16_t spt( mbr.sectorsPerTrack() );

            struct CHS chs;

            uint64_t temp = lba / numeric_cast<uint64_t>( spt );
            chs.sector    = numeric_cast<uint8_t>(( lba % numeric_cast<uint64_t>( spt ) ) + 1);
            chs.head      = numeric_cast<uint8_t>( temp & numeric_cast<uint64_t>( hpc ) );
            chs.cylinder  = numeric_cast<uint8_t>( temp / numeric_cast<uint64_t>( hpc ) );

            return chs;
        }
    }
}
