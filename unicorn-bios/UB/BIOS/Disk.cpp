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

#include "UB/BIOS/Disk.hpp"
#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/String.hpp"
#include "UB/Casts.hpp"
#include "UB/FAT/Functions.hpp"

namespace UB
{
    namespace BIOS
    {
        namespace Disk
        {
            bool reset( const Machine & machine, Engine & engine )
            {
                machine.ui().debug() << "Resetting drive " << String::toHex( engine.dl() ) << std::endl;

                engine.cf( false );
                engine.ah( 0 );

                return true;
            }

            bool readSectors( const Machine & machine, Engine & engine )
            {
                uint8_t    driveNumber( engine.dl() );
                uint8_t    sectors(     engine.al() );
                uint8_t    cylinder(    engine.ch() );
                uint8_t    sector(      engine.cl() );
                uint8_t    head(        engine.dh() );
                uint64_t   destination( Engine::getAddress( engine.es(), engine.bx() ) );
                FAT::Image image(       machine.bootImage() );

                if( driveNumber != 0x00 )
                {
                    machine.ui().debug() << "[ ERROR ]> Reading from drive " << String::toHex( driveNumber ) << " is not supported" << std::endl;

                    goto error;
                }

                machine.ui().debug() << "Reading " << static_cast< unsigned int >( sectors ) << " sector" << ( ( sectors > 1 ) ? "s" : "" ) << " from drive " << String::toHex( driveNumber )
                                     << std::endl
                                     << "    - Cylinder:    " << String::toHex( cylinder )
                                     << std::endl
                                     << "    - Head:        " << String::toHex( head )
                                     << std::endl
                                     << "    - Sector:      " << String::toHex( sector )
                                     << std::endl
                                     << "    - LBA:         " << String::toHex( FAT::chsToLBA( image.mbr(), cylinder, sector, head ) )
                                     << std::endl
                                     << "    - Destination: " << String::toHex( destination ) << " (" << String::toHex( engine.es() ) << ":" << String::toHex( engine.bx() ) << ")"
                                     << std::endl;

                {
                    std::vector< uint8_t > bytes( image.read( cylinder, head, sector, sectors ) );

                    if( bytes.size() == 0 )
                    {
                        machine.ui().debug() << "[ ERROR ]> No data received" << std::endl;

                        goto error;
                    }

                    engine.write( destination, bytes );

                    machine.ui().debug() << "[ SUCCESS ]> Wrote "
                                         << bytes.size()
                                         << " bytes at "
                                         << String::toHex( destination )
                                         << " -> "
                                         << String::toHex( destination + bytes.size() )
                                         << std::endl;

                    engine.cf( false );
                    engine.ah( 0 );
                    engine.al( sectors );

                    return true;
                }

                error:

                    engine.cf( true );
                    engine.ah( 1 );
                    engine.al( 0 );

                    return true;
            }

            bool checkExtensions( const Machine & machine, Engine & engine ) {
                machine.ui().debug() << "Checking if INT13h extensions are supported" << std::endl;

                engine.cf( false );
                engine.ah( 0 );

                return true;
            }

            bool extendedReadSectors( const Machine & machine, Engine & engine )
            {
                uint8_t    driveNumber( engine.dl() );
                uint64_t   destination( Engine::getAddress( engine.es(), engine.bx() ) );
                uint64_t   dapAddress(  Engine::getAddress( engine.ds(), engine.si() ) );
                FAT::Image image(       machine.bootImage() );
                FAT::MBR   mbr(         image.mbr() );

                std::vector< uint8_t > dapBytes ( engine.read( dapAddress, sizeof( struct FAT::DAP ) ) );

                struct FAT::DAP * dap = reinterpret_cast< struct FAT::DAP * >( dapBytes.data() );

                struct FAT::CHS chs = FAT::lbaToCHS( image.mbr(), dap->logicalBlockAddress );

                uint32_t logicalBlockAddress = numeric_cast< uint32_t >( dap->logicalBlockAddress );
                uint8_t  numberOfSectors     = numeric_cast< uint8_t  >( dap->numberOfSectors );
                uint8_t  cylinder            = numeric_cast< uint8_t  >( chs.cylinder );
                uint8_t  head                = numeric_cast< uint8_t  >( chs.head );
                uint8_t  sector              = numeric_cast< uint8_t  >( chs.sector );

                if( driveNumber != 0x00 )
                {
                    machine.ui().debug() << "[ ERROR ]> Reading from drive " << String::toHex( driveNumber ) << " is not supported" << std::endl;

                    goto error;
                }

                machine.ui().debug() << "Reading DAP " << static_cast< unsigned int >( dapAddress ) << " from drive " << String::toHex( driveNumber )
                                     << std::endl
                                     << "    - LBA:         " << String::toHex( logicalBlockAddress )
                                     << std::endl
                                     << "    - DAP Address: " << String::toHex( dapAddress )  << " (" << String::toHex( engine.ds() ) << ":" << String::toHex( engine.si() ) << ")"
                                     << "    - Cylinder:    " << String::toHex( cylinder )
                                     << std::endl
                                     << "    - Head:        " << String::toHex( head )
                                     << std::endl
                                     << "    - Sector:      " << String::toHex( sector )
                                     << std::endl
                                     << "    - LBA:         " << String::toHex( logicalBlockAddress )
                                     << std::endl
                                     << "    - Destination: " << String::toHex( destination ) << " (" << String::toHex( engine.es() ) << ":" << String::toHex( engine.bx() ) << ")"
                                     << std::endl;

                {
                    std::vector< uint8_t > bytes( image.read( cylinder, head, sector, numberOfSectors ) );

                    if( bytes.size() == 0 )
                    {
                        machine.ui().debug() << "[ ERROR ]> No data received" << std::endl;

                        goto error;
                    }

                    engine.write( destination, bytes );

                    machine.ui().debug() << "[ SUCCESS ]> Wrote "
                                         << bytes.size()
                                         << " bytes at "
                                         << String::toHex( destination )
                                         << " -> "
                                         << String::toHex( destination + bytes.size() )
                                         << std::endl;

                    engine.cf( false );
                    engine.ah( 0 );
                    engine.al( numberOfSectors );

                    return true;
                }

                error:

                    engine.cf( true );
                    engine.ah( 1 );
                    engine.al( 0 );

                    return true;
            }
        }
    }
}
