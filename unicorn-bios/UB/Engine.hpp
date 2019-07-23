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

#ifndef UB_ENGINE_HPP
#define UB_ENGINE_HPP

#include <memory>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <functional>

namespace UB
{
    class Engine
    {
        public:
            
            Engine( size_t memory );
            ~Engine( void );
            
            Engine( const Engine & o )              = delete;
            Engine( Engine && o )                   = delete;
            Engine & operator =( const Engine & o ) = delete;
            Engine & operator =( Engine && o )      = delete;
            
            uint16_t ax( void ) const;
            uint16_t bx( void ) const;
            uint16_t cx( void ) const;
            uint16_t dx( void ) const;
            uint16_t si( void ) const;
            uint16_t di( void ) const;
            uint16_t sp( void ) const;
            uint16_t bp( void ) const;
            uint16_t cs( void ) const;
            uint16_t ds( void ) const;
            uint16_t es( void ) const;
            uint16_t ss( void ) const;
            
            void ax( uint16_t value );
            void bx( uint16_t value );
            void cx( uint16_t value );
            void dx( uint16_t value );
            void si( uint16_t value );
            void di( uint16_t value );
            void sp( uint16_t value );
            void bp( uint16_t value );
            void cs( uint16_t value );
            void ds( uint16_t value );
            void es( uint16_t value );
            void ss( uint16_t value );
            
            void onInterrupt( uint32_t i, const std::function< bool( uint32_t i, Engine & ) > handler );
            void write( size_t address, const std::vector< uint8_t > & bytes );
            void start( size_t address );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_ENGINE_HPP */
